#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ----------------------------------------------------------------------------
// ESTRUTURA DE DADOS
// ----------------------------------------------------------------------------

/**
 * @brief Define a estrutura de um cômodo (nó da árvore).
 * * Cada sala possui um nome e dois ponteiros, um para o caminho
 * da esquerda e outro para o da direita.
 */
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;


// ----------------------------------------------------------------------------
// PROTÓTIPOS DAS FUNÇÕES
// ----------------------------------------------------------------------------

Sala* criarSala(const char* nome);
void explorarSalas(Sala* salaInicial);
void liberarMapa(Sala* sala);


// ----------------------------------------------------------------------------
// FUNÇÃO PRINCIPAL
// ----------------------------------------------------------------------------

/**
 * @brief Monta o mapa inicial da mansão e dá início à exploração.
 *
 * Esta função é o ponto de entrada do programa. Ela cria todas as salas
 * da mansão, conecta-as para formar a árvore (mapa) e, em seguida,
 * inicia a jornada do jogador a partir do Hall de Entrada. Ao final,
 * libera toda a memória alocada.
 */
int main() {
    // --- Montagem do Mapa da Mansão (Árvore Binária) ---
    // A árvore é criada manualmente aqui, como solicitado.

    // Nível 0 (Raiz)
    Sala* hall = criarSala("Hall de Entrada");

    // Nível 1
    Sala* salaDeJantar = criarSala("Sala de Jantar");
    Sala* biblioteca = criarSala("Biblioteca");
    
    // Nível 2
    Sala* cozinha = criarSala("Cozinha");
    Sala* despensa = criarSala("Despensa");
    Sala* escritorio = criarSala("Escritorio");
    Sala* jardimSecreto = criarSala("Jardim Secreto");

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

    // --- Início do Jogo ---
    printf("=======================================\n");
    printf("        Bem-vindo ao Detective Quest!       \n");
    printf("=======================================\n");
    printf("Explore a mansao e desvende seus misterios.\n");

    // Chama a função que controla a navegação do jogador
    explorarSalas(hall);

    // --- Limpeza ---
    // Libera a memória de todas as salas criadas para evitar vazamentos.
    liberarMapa(hall);

    return 0;
}


// ----------------------------------------------------------------------------
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ----------------------------------------------------------------------------

/**
 * @brief Cria, de forma dinâmica, uma sala com nome.
 *
 * Aloca memória para uma nova sala, copia o nome fornecido para a
 * estrutura e inicializa os caminhos (esquerda e direita) como nulos,
 * pois uma nova sala ainda não tem conexões.
 *
 * @param nome O nome do cômodo a ser criado.
 * @return Um ponteiro para a sala recém-criada.
 */
Sala* criarSala(const char* nome) {
    // 1. Aloca memória do tamanho da struct Sala
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));

    // 2. Verifica se a alocação de memória funcionou
    if (novaSala == NULL) {
        printf("Erro: Falha ao alocar memoria para uma sala.\n");
        exit(1); // Encerra o programa se não houver memória
    }

    // 3. Copia o nome recebido para a variável 'nome' da struct
    strcpy(novaSala->nome, nome);

    // 4. Inicializa os ponteiros da esquerda e direita como NULL
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    // 5. Retorna o endereço da sala criada
    return novaSala;
}

/**
 * @brief Permite a navegação interativa do jogador pela árvore (mansão).
 *
 * A função recebe a sala inicial e entra em um loop, mostrando ao jogador
 * onde ele está e quais caminhos pode seguir. O loop termina quando o
 * jogador chega a uma sala sem saídas (nó-folha) ou decide sair.
 *
 * @param salaInicial O ponto de partida da exploração (a raiz da árvore).
 */
void explorarSalas(Sala* salaAtual) {
    char escolha;

    // O loop continua enquanto houver uma sala válida para explorar
    while (salaAtual != NULL) {
        printf("\n---------------------------------------\n");
        printf("Voce esta em: %s\n", salaAtual->nome);

        // Verifica se é um beco sem saída (nó-folha)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Este comodo nao tem mais saidas. Fim da exploracao neste caminho!\n");
            break; // Sai do loop while
        }

        // Mostra as opções disponíveis
        printf("Para onde voce quer ir?\n");
        if (salaAtual->esquerda != NULL) {
            printf(" (e) - Esquerda (%s)\n", salaAtual->esquerda->nome);
        }
        if (salaAtual->direita != NULL) {
            printf(" (d) - Direita (%s)\n", salaAtual->direita->nome);
        }
        printf(" (s) - Sair da mansao\n");
        printf("Escolha: ");

        // Lê a escolha do jogador. O espaço antes de %c ignora quebras de linha anteriores.
        scanf(" %c", &escolha);

        // Processa a escolha
        switch (escolha) {
            case 'e':
            case 'E':
                if (salaAtual->esquerda != NULL) {
                    salaAtual = salaAtual->esquerda; // Move o jogador para a sala da esquerda
                } else {
                    printf("Caminho bloqueado. Tente outra direcao.\n");
                }
                break;
            case 'd':
            case 'D':
                if (salaAtual->direita != NULL) {
                    salaAtual = salaAtual->direita; // Move o jogador para a sala da direita
                } else {
                    printf("Caminho bloqueado. Tente outra direcao.\n");
                }
                break;
            case 's':
            case 'S':
                printf("\nVoce decidiu sair da mansao. Ate a proxima, detetive!\n");
                salaAtual = NULL; // Força a saída do loop
                break;
            default:
                printf("Opcao invalida. Por favor, escolha um caminho existente.\n");
                break;
        }
    }
    printf("=======================================\n");
}

/**
 * @brief Libera recursivamente toda a memória alocada para o mapa.
 *
 * Para evitar vazamento de memória (memory leak), esta função percorre
 * a árvore e libera cada um dos nós alocados com malloc. A abordagem é
 * "pós-ordem": primeiro libera os filhos (esquerda, direita) e depois o
 * próprio nó.
 *
 * @param sala O nó atual a ser liberado.
 */
void liberarMapa(Sala* sala) {
    // Condição de parada da recursão: se a sala não existe, não há nada a fazer.
    if (sala == NULL) {
        return;
    }

    // Libera recursivamente a sub-árvore da esquerda
    liberarMapa(sala->esquerda);
    // Libera recursivamente a sub-árvore da direita
    liberarMapa(sala->direita);
    // Finalmente, libera a sala atual
    free(sala);
}