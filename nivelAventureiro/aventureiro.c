#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ----------------------------------------------------------------------------
// ESTRUTURAS DE DADOS
// ----------------------------------------------------------------------------

/**
 * @brief Define a estrutura de um cômodo da mansão (nó da árvore binária do mapa).
 * * Cada sala possui um nome, uma pista (opcional) e dois ponteiros
 * * para os caminhos da esquerda e da direita.
 */
typedef struct Sala {
    char nome[50];
    char pista[100]; // Nova! Pista associada a este cômodo (pode ser vazia)
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/**
 * @brief Define a estrutura de um nó da Árvore Binária de Busca (BST) para as pistas.
 * * Cada nó guarda o conteúdo de uma pista e ponteiros para os filhos
 * * da esquerda e da direita na BST.
 */
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;


// ----------------------------------------------------------------------------
// PROTÓTIPOS DAS FUNÇÕES
// ----------------------------------------------------------------------------

// Funções para o Mapa da Mansão (Árvore Binária)
Sala* criarSala(const char* nome, const char* pista);
void liberarMapa(Sala* sala);

// Funções para a Árvore de Pistas (BST)
PistaNode* inserirPista(PistaNode* raiz, const char* novaPista);
void exibirPistas(PistaNode* raiz);
void liberarPistas(PistaNode* raiz);

// Função de Exploração e Interação
void explorarSalasComPistas(Sala* salaInicial, PistaNode** bstPistas);


// ----------------------------------------------------------------------------
// FUNÇÃO PRINCIPAL
// ----------------------------------------------------------------------------

/**
 * @brief Monta o mapa inicial da mansão com suas pistas e inicia a exploração.
 *
 * Esta função é o ponto de entrada do programa. Ela:
 * 1. Cria todas as salas da mansão, associando pistas a algumas delas.
 * 2. Conecta as salas para formar a árvore binária (mapa).
 * 3. Inicia a jornada do jogador a partir do Hall de Entrada, permitindo
 * a coleta e armazenamento de pistas em uma BST.
 * 4. Ao final, exibe todas as pistas coletadas em ordem alfabética.
 * 5. Libera toda a memória alocada para o mapa e para a BST de pistas.
 */
int main() {
    // --- Montagem do Mapa da Mansão (Árvore Binária) ---
    // A árvore é criada manualmente aqui, como solicitado, com pistas.

    // Nível 0 (Raiz)
    Sala* hall = criarSala("Hall de Entrada", "Um jornal velho sobre a mesa, com a data de 1920.");

    // Nível 1
    Sala* salaDeJantar = criarSala("Sala de Jantar", "Restos de um banquete suntuoso, mas sem talheres.");
    Sala* biblioteca = criarSala("Biblioteca", "Um livro de Sherlock Holmes aberto em uma pagina especifica.");
    
    // Nível 2
    Sala* cozinha = criarSala("Cozinha", "Uma faca de prata reluzente na pia.");
    Sala* despensa = criarSala("Despensa", "Um frasco de veneno vazio e etiquetado como 'Raticida'.");
    Sala* escritorio = criarSala("Escritorio", "Cartas rasgadas revelam um desentendimento familiar.");
    Sala* jardimSecreto = criarSala("Jardim Secreto", "Rastros de pegadas frescas no chao umido.");

    // Nível 3 (Novas salas para testar mais pistas e caminhos)
    Sala* quartoPrincipal = criarSala("Quarto Principal", "Um relogio de bolso parado as 03:15.");
    Sala* banheiro = criarSala("Banheiro", "Uma toalha molhada e suja de terra.");

    // --- Conectando as salas ---
    // Hall de Entrada leva para...
    hall->esquerda = salaDeJantar;
    hall->direita = biblioteca;

    // Sala de Jantar leva para...
    salaDeJantar->esquerda = cozinha;
    salaDeJantar->direita = despensa;

    // Biblioteca leva para...
    biblioteca->esquerda = escritorio;
    biblioteca->direita = jardimSecreto;

    // Conectando Nível 3
    cozinha->esquerda = quartoPrincipal;
    despensa->direita = banheiro;

    // --- Inicialização da BST de Pistas ---
    PistaNode* bstPistas = NULL; // A raiz da BST de pistas começa como NULL

    printf("=======================================\n");
    printf("        Bem-vindo ao Detective Quest!       \n");
    printf("=======================================\n");
    printf("Explore a mansao, colete pistas e desvende o misterio.\n");

    // Chama a função que controla a navegação e coleta de pistas
    explorarSalasComPistas(hall, &bstPistas); // Passa o endereço da raiz da BST

    printf("\n=======================================\n");
    printf("        Fim da exploracao da mansao.        \n");
    printf("=======================================\n");

    // --- Exibição das Pistas Coletadas ---
    printf("\nPistas coletadas em ordem alfabetica:\n");
    if (bstPistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(bstPistas);
    }

    // --- Limpeza ---
    // Libera a memória de todas as salas e de todos os nós da BST de pistas.
    liberarMapa(hall);
    liberarPistas(bstPistas); // Libera a BST de pistas

    return 0;
}


// ----------------------------------------------------------------------------
// IMPLEMENTAÇÃO DAS FUNÇÕES DO MAPA (ÁRVORE BINÁRIA)
// ----------------------------------------------------------------------------

/**
 * @brief Cria, de forma dinâmica, um cômodo (Sala) com nome e uma pista opcional.
 *
 * Aloca memória para uma nova sala, copia o nome e a pista fornecidos
 * para a estrutura e inicializa os caminhos (esquerda e direita) como nulos.
 * Se a pista for uma string vazia (""), significa que o cômodo não possui uma pista.
 *
 * @param nome O nome do cômodo a ser criado.
 * @param pista O conteúdo da pista associada a este cômodo. Use "" se não houver pista.
 * @return Um ponteiro para a sala recém-criada.
 */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro: Falha ao alocar memoria para uma sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista); // Copia a pista para a nova sala
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/**
 * @brief Libera recursivamente toda a memória alocada para o mapa da mansão.
 *
 * Percorre a árvore em pós-ordem, liberando primeiro os filhos e depois o próprio nó,
 * para evitar vazamento de memória.
 *
 * @param sala O nó (cômodo) atual a ser liberado.
 */
void liberarMapa(Sala* sala) {
    if (sala == NULL) {
        return;
    }
    liberarMapa(sala->esquerda);
    liberarMapa(sala->direita);
    free(sala);
}

// ----------------------------------------------------------------------------
// IMPLEMENTAÇÃO DAS FUNÇÕES DA ÁRVORE DE PISTAS (BST)
// ----------------------------------------------------------------------------

/**
 * @brief Insere uma nova pista na Árvore Binária de Busca (BST) de forma ordenada.
 *
 * Esta função é recursiva. Ela compara a nova pista com a pista do nó atual
 * para decidir se a inserção deve ocorrer à esquerda ou à direita. Se a pista
 * já existir, ela não é inserida novamente.
 *
 * @param raiz O nó raiz da sub-árvore atual.
 * @param novaPista A string contendo a pista a ser inserida.
 * @return A nova raiz da sub-árvore após a inserção.
 */
PistaNode* inserirPista(PistaNode* raiz, const char* novaPista) {
    // Caso base: Se a raiz for NULL, cria um novo nó e o retorna
    if (raiz == NULL) {
        PistaNode* novoNode = (PistaNode*) malloc(sizeof(PistaNode));
        if (novoNode == NULL) {
            printf("Erro: Falha ao alocar memoria para uma pista.\n");
            exit(1);
        }
        strcpy(novoNode->pista, novaPista);
        novoNode->esquerda = NULL;
        novoNode->direita = NULL;
        return novoNode;
    }

    // Compara a nova pista com a pista do nó atual
    int comparacao = strcmp(novaPista, raiz->pista);

    if (comparacao < 0) { // Se a nova pista é "menor" (vem antes no alfabeto)
        raiz->esquerda = inserirPista(raiz->esquerda, novaPista);
    } else if (comparacao > 0) { // Se a nova pista é "maior" (vem depois no alfabeto)
        raiz->direita = inserirPista(raiz->direita, novaPista);
    }
    // Se comparacao == 0, a pista já existe na árvore, então não faz nada.

    return raiz; // Retorna a raiz atual (ou a nova raiz se houve inserção na raiz)
}

/**
 * @brief Exibe todas as pistas coletadas em ordem alfabética (percurso Em Ordem).
 *
 * Esta função é recursiva e realiza um percurso Em Ordem (Inorder Traversal)
 * na BST, garantindo que as pistas sejam impressas em ordem crescente.
 *
 * @param raiz O nó raiz da sub-árvore atual.
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda); // Visita a sub-árvore esquerda
        printf("- %s\n", raiz->pista); // Visita o nó atual (imprime a pista)
        exibirPistas(raiz->direita);  // Visita a sub-árvore direita
    }
}

/**
 * @brief Libera recursivamente toda a memória alocada para a BST de pistas.
 *
 * Realiza um percurso em pós-ordem para liberar todos os nós da BST,
 * prevenindo vazamentos de memória.
 *
 * @param raiz O nó raiz da sub-árvore atual a ser liberada.
 */
void liberarPistas(PistaNode* raiz) {
    if (raiz == NULL) {
        return;
    }
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}


// ----------------------------------------------------------------------------
// IMPLEMENTAÇÃO DA FUNÇÃO DE EXPLORAÇÃO E INTERAÇÃO
// ----------------------------------------------------------------------------

/**
 * @brief Permite a navegação interativa do jogador pela mansão e coleta de pistas.
 *
 * A função recebe a sala inicial e um ponteiro para a raiz da BST de pistas.
 * Entra em um loop, mostrando ao jogador onde ele está, se há uma pista no local,
 * e quais caminhos pode seguir. O loop termina quando o jogador escolhe sair.
 * Pistas são automaticamente adicionadas à BST quando a sala é visitada.
 *
 * @param salaInicial O ponto de partida da exploração (a raiz da árvore do mapa).
 * @param bstPistas Um ponteiro para o ponteiro da raiz da BST de pistas,
 * permitindo que a BST seja modificada dentro da função.
 */
void explorarSalasComPistas(Sala* salaAtual, PistaNode** bstPistas) {
    char escolha;
    // Um array para rastrear as pistas já coletadas, para não adicionar duplicatas
    // Simplificando: uma lista de flags para as salas que possuem pistas únicas.
    // Em um sistema maior, usaria uma tabela hash ou uma lista ligada de pistas coletadas
    // para verificar duplicatas de forma mais robusta.
    // Para este nível, apenas adicionaremos pistas se a sala for visitada.
    // A BST por si só já evita duplicatas se o strcmp retornar 0.

    // A variável 'pistaColetada' é usada para exibir a mensagem "Pista encontrada!" uma única vez por sala.
    int pistaColetadaNestaSala = 0;

    while (salaAtual != NULL) {
        printf("\n---------------------------------------\n");
        printf("Voce esta em: %s\n", salaAtual->nome);

        // Se a sala tem uma pista e ela ainda não foi "coletada" (ou seja, adicionada à BST neste momento)
        // Note: A BST `inserirPista` já cuida de não adicionar duplicatas pelo `strcmp`.
        // A flag `pistaColetadaNestaSala` é apenas para a UX (User Experience).
        if (strlen(salaAtual->pista) > 0 && !pistaColetadaNestaSala) {
            *bstPistas = inserirPista(*bstPistas, salaAtual->pista);
            printf(">>> Pista encontrada: \"%s\" <<<\n", salaAtual->pista);
            pistaColetadaNestaSala = 1; // Marca que a pista desta sala foi processada
        } else if (strlen(salaAtual->pista) == 0) {
            printf("Nenhuma pista relevante neste comodo.\n");
        }


        // Verifica se é um beco sem saída (nó-folha)
        // NOTA: O requisito para este nível diz "o usuario deve optar por sair (s) a qualquer momento".
        // Portanto, a condição de nó-folha não encerra a exploração automaticamente aqui,
        // apenas informa que não há mais caminhos adiante.
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Este comodo nao tem mais saidas neste caminho.\n");
        }

        // Mostra as opções disponíveis
        printf("Para onde voce quer ir?\n");
        if (salaAtual->esquerda != NULL) {
            printf(" (e) - Esquerda (%s)\n", salaAtual->esquerda->nome);
        }
        if (salaAtual->direita != NULL) {
            printf(" (d) - Direita (%s)\n", salaAtual->direita->nome);
        }
        printf(" (s) - Sair da mansao e ver as pistas\n");
        printf("Escolha: ");

        scanf(" %c", &escolha); // Lê a escolha do jogador

        // Processa a escolha
        switch (escolha) {
            case 'e':
            case 'E':
                if (salaAtual->esquerda != NULL) {
                    salaAtual = salaAtual->esquerda; // Move o jogador
                    pistaColetadaNestaSala = 0; // Reseta a flag para a nova sala
                } else {
                    printf("Caminho bloqueado. Tente outra direcao.\n");
                }
                break;
            case 'd':
            case 'D':
                if (salaAtual->direita != NULL) {
                    salaAtual = salaAtual->direita; // Move o jogador
                    pistaColetadaNestaSala = 0; // Reseta a flag para a nova sala
                } else {
                    printf("Caminho bloqueado. Tente outra direcao.\n");
                }
                break;
            case 's':
            case 'S':
                printf("\nVoce decidiu sair da mansao para analisar as pistas. Ate a proxima!\n");
                salaAtual = NULL; // Força a saída do loop
                break;
            default:
                printf("Opcao invalida. Por favor, escolha um caminho existente ou 's' para sair.\n");
                break;
        }
    }
}