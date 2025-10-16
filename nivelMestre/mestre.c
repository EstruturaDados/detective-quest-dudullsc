#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ----------------------------------------------------------------------------
// CONSTANTES E ESTRUTURAS DE DADOS
// ----------------------------------------------------------------------------

#define HASH_SIZE 10 // Define o tamanho da nossa Tabela Hash

// Estrutura para os cômodos da mansão (Árvore Binária do Mapa)
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura para os nós da Árvore de Pistas (BST)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Estrutura para os nós da Tabela Hash (encadeamento para colisões)
typedef struct HashNode {
    char pista[100];    // Chave
    char suspeito[50];  // Valor
    struct HashNode *next;
} HashNode;


// ----------------------------------------------------------------------------
// PROTÓTIPOS DAS FUNÇÕES
// ----------------------------------------------------------------------------

// Funções do Mapa
Sala* criarSala(const char* nome, const char* pista);
void liberarMapa(Sala* sala);

// Funções da BST de Pistas
PistaNode* adicionarPista(PistaNode* raiz, const char* novaPista);
void exibirPistas(PistaNode* raiz);
void liberarPistas(PistaNode* raiz);

// Funções da Tabela Hash
unsigned int hashFunction(const char* str);
void inserirNaHash(HashNode* tabela[], const char* pista, const char* suspeito);
const char* encontrarSuspeito(HashNode* tabela[], const char* pista);
void liberarHash(HashNode* tabela[]);

// Funções de Lógica do Jogo
const char* getSuspeitoParaPista(const char* pista);
int contarPistasParaSuspeito(PistaNode* raizPistas, HashNode* tabelaHash[], const char* suspeito);
void explorarSalas(Sala* salaInicial, PistaNode** bstPistas, HashNode* tabelaHash[]);
void verificarSuspeitoFinal(PistaNode* bstPistas, HashNode* tabelaHash[]);


// ----------------------------------------------------------------------------
// FUNÇÃO PRINCIPAL
// ----------------------------------------------------------------------------

int main() {
    // --- Montagem do Mapa da Mansão (Árvore Binária) ---
    Sala* hall = criarSala("Hall de Entrada", "Um jornal velho sobre a mesa, com a data de 1920.");
    Sala* salaDeJantar = criarSala("Sala de Jantar", "Um candelabro de prata polido, fora do lugar.");
    Sala* biblioteca = criarSala("Biblioteca", "Um livro sobre venenos com uma pagina marcada.");
    Sala* cozinha = criarSala("Cozinha", "Uma faca de cozinha faltando no conjunto.");
    Sala* escritorio = criarSala("Escritorio", "Uma carta de ameaca enderecada a vitima.");
    Sala* jardimSecreto = criarSala("Jardim Secreto", "Pegadas de sapatos caros na lama.");

    hall->esquerda = salaDeJantar;
    hall->direita = biblioteca;
    salaDeJantar->esquerda = cozinha;
    biblioteca->esquerda = escritorio;
    biblioteca->direita = jardimSecreto;

    // --- Inicialização das Estruturas ---
    PistaNode* bstPistas = NULL; // Raiz da BST de pistas
    HashNode* tabelaHash[HASH_SIZE]; // Tabela Hash para Pista -> Suspeito

    // Inicializa todos os ponteiros da tabela hash como NULL
    for (int i = 0; i < HASH_SIZE; i++) {
        tabelaHash[i] = NULL;
    }

    printf("=======================================\n");
    printf("        Bem-vindo ao Detective Quest!       \n");
    printf("=======================================\n");
    printf("Explore a mansao, colete pistas, e descubra o culpado.\n");

    // Inicia a exploração
    explorarSalas(hall, &bstPistas, tabelaHash);

    // Inicia a fase de julgamento
    verificarSuspeitoFinal(bstPistas, tabelaHash);

    // --- Limpeza de Memória ---
    liberarMapa(hall);
    liberarPistas(bstPistas);
    liberarHash(tabelaHash);

    return 0;
}


// ----------------------------------------------------------------------------
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ----------------------------------------------------------------------------

/**
 * @brief Cria dinamicamente um cômodo com nome e uma pista opcional.
 */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    if (!novaSala) exit(1);
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = novaSala->direita = NULL;
    return novaSala;
}

/**
 * @brief Navega pela árvore da mansão, coleta pistas, as insere na BST e na Tabela Hash.
 */
void explorarSalas(Sala* salaAtual, PistaNode** bstPistas, HashNode* tabelaHash[]) {
    char escolha;
    while (salaAtual != NULL) {
        printf("\n---------------------------------------\n");
        printf("Voce esta em: %s\n", salaAtual->nome);

        // Verifica se há uma pista e a processa
        if (strlen(salaAtual->pista) > 0) {
            const char* suspeitoAssociado = getSuspeitoParaPista(salaAtual->pista);
            if (suspeitoAssociado) {
                printf(">>> Pista encontrada: \"%s\" <<<\n", salaAtual->pista);
                // Adiciona na BST e na Tabela Hash
                *bstPistas = adicionarPista(*bstPistas, salaAtual->pista);
                inserirNaHash(tabelaHash, salaAtual->pista, suspeitoAssociado);
                // "Remove" a pista da sala para não ser coletada novamente
                strcpy(salaAtual->pista, "");
            }
        } else {
            printf("Nenhuma pista nova neste comodo.\n");
        }
        
        printf("Para onde voce quer ir? (e/d/s): ");
        scanf(" %c", &escolha);

        switch (escolha) {
            case 'e': case 'E': salaAtual = salaAtual->esquerda; break;
            case 'd': case 'D': salaAtual = salaAtual->direita; break;
            case 's': case 'S': salaAtual = NULL; break; // Sair
            default: printf("Opcao invalida.\n"); break;
        }
    }
}

/**
 * @brief Insere a pista coletada na árvore de pistas (BST) de forma ordenada.
 */
PistaNode* adicionarPista(PistaNode* raiz, const char* novaPista) {
    if (raiz == NULL) {
        PistaNode* novoNode = (PistaNode*) malloc(sizeof(PistaNode));
        if (!novoNode) exit(1);
        strcpy(novoNode->pista, novaPista);
        novoNode->esquerda = novoNode->direita = NULL;
        return novoNode;
    }
    int cmp = strcmp(novaPista, raiz->pista);
    if (cmp < 0) {
        raiz->esquerda = adicionarPista(raiz->esquerda, novaPista);
    } else if (cmp > 0) {
        raiz->direita = adicionarPista(raiz->direita, novaPista);
    }
    return raiz;
}

/**
 * @brief Insere uma associação pista/suspeito na tabela hash.
 * Utiliza encadeamento para tratar colisões.
 */
void inserirNaHash(HashNode* tabela[], const char* pista, const char* suspeito) {
    unsigned int index = hashFunction(pista);

    HashNode* novoNode = (HashNode*) malloc(sizeof(HashNode));
    if (!novoNode) exit(1);
    strcpy(novoNode->pista, pista);
    strcpy(novoNode->suspeito, suspeito);
    
    // Insere no início da lista encadeada (mais simples)
    novoNode->next = tabela[index];
    tabela[index] = novoNode;
}

/**
 * @brief Consulta o suspeito correspondente a uma pista na tabela hash.
 * @return O nome do suspeito se encontrado, ou NULL caso contrário.
 */
const char* encontrarSuspeito(HashNode* tabela[], const char* pista) {
    unsigned int index = hashFunction(pista);
    HashNode* atual = tabela[index];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->next;
    }
    return NULL; // Não encontrado
}

/**
 * @brief Conduz a fase de julgamento final. Pede ao jogador uma acusação
 * e verifica se há evidências suficientes (>= 2 pistas).
 */
void verificarSuspeitoFinal(PistaNode* bstPistas, HashNode* tabelaHash[]) {
    printf("\n=======================================\n");
    printf("        J U L G A M E N T O\n");
    printf("=======================================\n");

    if (bstPistas == NULL) {
        printf("Voce nao coletou pistas suficientes para fazer uma acusacao. Caso encerrado.\n");
        return;
    }

    printf("Pistas coletadas em ordem alfabetica:\n");
    exibirPistas(bstPistas);

    printf("\nSuspeitos possiveis: Mordomo, Jardineiro, Cozinheira, Dama_da_noite\n");
    char acusado[50];
    printf("Quem voce acusa de ser o culpado? ");
    scanf("%49s", acusado);

    int contagemPistas = contarPistasParaSuspeito(bstPistas, tabelaHash, acusado);
    
    printf("\n--- Veredito ---\n");
    printf("Voce acusou %s. A investigacao revelou %d pista(s) contra esta pessoa.\n", acusado, contagemPistas);

    if (contagemPistas >= 2) {
        printf("Evidencias suficientes! Voce desvendou o misterio! PARABENS!\n");
    } else {
        printf("Evidencias insuficientes. O verdadeiro culpado escapou...\n");
    }
}

// --- Funções Auxiliares e de Limpeza ---

/**
 * @brief Função de espalhamento (hashing) simples baseada na soma dos valores ASCII.
 */
unsigned int hashFunction(const char* str) {
    unsigned long hash = 0;
    int c;
    while ((c = *str++)) {
        hash += c;
    }
    return hash % HASH_SIZE;
}

/**
 * @brief Simula uma base de dados que associa pistas a suspeitos.
 */
const char* getSuspeitoParaPista(const char* pista) {
    if (strcmp(pista, "Um candelabro de prata polido, fora do lugar.") == 0) return "Mordomo";
    if (strcmp(pista, "Pegadas de sapatos caros na lama.") == 0) return "Dama_da_noite";
    if (strcmp(pista, "Uma faca de cozinha faltando no conjunto.") == 0) return "Cozinheira";
    if (strcmp(pista, "Uma carta de ameaca enderecada a vitima.") == 0) return "Dama_da_noite";
    if (strcmp(pista, "Um livro sobre venenos com uma pagina marcada.") == 0) return "Mordomo";
    return NULL; // Pista sem suspeito claro
}

/**
 * @brief Conta recursivamente quantas pistas na BST apontam para um suspeito específico.
 */
int contarPistasParaSuspeito(PistaNode* raizPistas, HashNode* tabelaHash[], const char* suspeito) {
    if (raizPistas == NULL) {
        return 0;
    }

    int count = 0;
    const char* suspeitoEncontrado = encontrarSuspeito(tabelaHash, raizPistas->pista);

    if (suspeitoEncontrado && strcmp(suspeitoEncontrado, suspeito) == 0) {
        count = 1;
    }

    return count + contarPistasParaSuspeito(raizPistas->esquerda, tabelaHash, suspeito) +
           contarPistasParaSuspeito(raizPistas->direita, tabelaHash, suspeito);
}

/**
 * @brief Exibe as pistas da BST em ordem alfabética (percurso em ordem).
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

void liberarMapa(Sala* sala) {
    if (sala) {
        liberarMapa(sala->esquerda);
        liberarMapa(sala->direita);
        free(sala);
    }
}

void liberarPistas(PistaNode* raiz) {
    if (raiz) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

void liberarHash(HashNode* tabela[]) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* atual = tabela[i];
        while (atual != NULL) {
            HashNode* proximo = atual->next;
            free(atual);
            atual = proximo;
        }
    }
}