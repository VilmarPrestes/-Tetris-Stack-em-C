#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA   5   // capacidade fixa da fila circular
#define TAM_PILHA  3   // capacidade fixa da pilha

// ---------------------------
// Struct da peça do Tetris
// ---------------------------
typedef struct {
    char nome;  // 'I', 'O', 'T', 'L'
    int id;     // identificador único
} Peca;

// ---------------------------
// Fila circular de peças futuras
// ---------------------------
Peca fila[TAM_FILA];
int frente = 0;      // índice do primeiro elemento
int tras = 0;        // índice da próxima posição livre
int qtdFila = 0;     // quantidade de elementos na fila

// ---------------------------
// Pilha de peças reservadas
// ---------------------------
Peca pilha[TAM_PILHA];
int topo = -1;       // topo da pilha (-1 = vazia)

// ---------------------------
// Controle de geração de peças
// ---------------------------
int proximoId = 0;   // id único para cada peça gerada

// ---------------------------
// Funções utilitárias
// ---------------------------

// Gera uma nova peça aleatória com nome em {'I','O','T','L'} e id único
Peca gerarPeca() {
    Peca p;
    char tipos[] = { 'I', 'O', 'T', 'L' };
    int indice = rand() % 4;
    p.nome = tipos[indice];
    p.id = proximoId++;
    return p;
}

// Exibe uma peça no formato [X id]
void exibirPeca(Peca p) {
    printf("[%c %d] ", p.nome, p.id);
}

// ---------------------------
// Operações da fila circular
// ---------------------------

int filaCheia() {
    return qtdFila == TAM_FILA;
}

int filaVazia() {
    return qtdFila == 0;
}

// Enfileira uma peça no final da fila
int enfileirar(Peca p) {
    if (filaCheia()) {
        // Não deveria acontecer se mantivermos a lógica de sempre tirar antes de colocar,
        // mas a função trata esse caso por segurança.
        printf("[ERRO] Fila cheia, nao foi possivel enfileirar a peca.\n");
        return 0;
    }
    fila[tras] = p;
    tras = (tras + 1) % TAM_FILA;
    qtdFila++;
    return 1;
}

// Desenfileira a peça da frente e devolve via ponteiro
int desenfileirar(Peca* p) {
    if (filaVazia()) {
        printf("[ERRO] Fila vazia, nao ha peca para jogar.\n");
        return 0;
    }
    *p = fila[frente];
    frente = (frente + 1) % TAM_FILA;
    qtdFila--;
    return 1;
}

// Exibe o estado atual da fila
void exibirFila() {
    printf("Fila de pecas\t");
    if (filaVazia()) {
        printf("(vazia)");
    } else {
        for (int i = 0; i < qtdFila; i++) {
            int idx = (frente + i) % TAM_FILA;
            exibirPeca(fila[idx]);
        }
    }
    printf("\n");
}

// Preenche a fila inicialmente com TAM_FILA peças
void preencherFilaInicial() {
    for (int i = 0; i < TAM_FILA; i++) {
        Peca p = gerarPeca();
        enfileirar(p);
    }
}

// ---------------------------
// Operações da pilha
// ---------------------------

int pilhaCheia() {
    return topo == TAM_PILHA - 1;
}

int pilhaVazia() {
    return topo == -1;
}

// Empilha uma peça no topo
int empilhar(Peca p) {
    if (pilhaCheia()) {
        printf("[ERRO] Pilha cheia, nao foi possivel reservar a peca.\n");
        return 0;
    }
    topo++;
    pilha[topo] = p;
    return 1;
}

// Desempilha a peça do topo e devolve via ponteiro
int desempilhar(Peca* p) {
    if (pilhaVazia()) {
        printf("[ERRO] Pilha vazia, nao ha peca reservada para usar.\n");
        return 0;
    }
    *p = pilha[topo];
    topo--;
    return 1;
}

// Exibe o estado atual da pilha
void exibirPilha() {
    printf("Pilha de reserva\t(Topo -> base): ");
    if (pilhaVazia()) {
        printf("(vazia)");
    } else {
        for (int i = topo; i >= 0; i--) {
            exibirPeca(pilha[i]);
        }
    }
    printf("\n");
}

// ---------------------------
// Operações combinadas (fila + pilha)
// ---------------------------

// Troca a peça da frente da fila com o topo da pilha
void trocarFrenteComTopo() {
    if (filaVazia()) {
        printf("[ERRO] Nao ha peca na frente da fila para trocar.\n");
        return;
    }
    if (pilhaVazia()) {
        printf("[ERRO] Nao ha peca na pilha para trocar.\n");
        return;
    }

    int idxFrente = frente;
    Peca temp = fila[idxFrente];
    fila[idxFrente] = pilha[topo];
    pilha[topo] = temp;

    printf("Troca realizada entre a peca da frente da fila e o topo da pilha.\n");
}

// Troca os 3 primeiros elementos da fila com as 3 peças da pilha
// (se ambas tiverem pelo menos 3 peças)
void trocaMultipla3() {
    if (qtdFila < 3) {
        printf("[ERRO] Fila nao possui 3 pecas para troca multipla.\n");
        return;
    }
    if (topo + 1 < 3) {
        printf("[ERRO] Pilha nao possui 3 pecas para troca multipla.\n");
        return;
    }

    Peca tempFila[3];
    Peca tempPilha[3];

    // Guardar as 3 primeiras da fila
    for (int i = 0; i < 3; i++) {
        int idx = (frente + i) % TAM_FILA;
        tempFila[i] = fila[idx];
    }

    // Guardar as 3 da pilha (do topo para baixo)
    for (int i = 0; i < 3; i++) {
        tempPilha[i] = pilha[topo - i];
    }

    // Colocar as peças da pilha nas 3 primeiras posições da fila
    // Ordem: topo -> frente, topo-1 -> segundo da fila, etc.
    for (int i = 0; i < 3; i++) {
        int idx = (frente + i) % TAM_FILA;
        fila[idx] = tempPilha[i];
    }

    // Colocar as peças da fila na pilha
    // Para seguir o exemplo: topo recebe a 3a peça antiga da fila (index 2),
    // depois a 2a, depois a 1a (comportamento LIFO)
    pilha[topo]     = tempFila[2];
    pilha[topo - 1] = tempFila[1];
    pilha[topo - 2] = tempFila[0];

    printf("Troca multipla realizada entre as 3 primeiras da fila e as 3 da pilha.\n");
}

// ---------------------------
// Exibição do estado geral
// ---------------------------

void exibirEstadoAtual() {
    printf("\n=== ESTADO ATUAL ===\n");
    exibirFila();
    exibirPilha();
    printf("====================\n\n");
}

// ---------------------------
// Menu
// ---------------------------

void exibirMenu() {
    printf("Opcoes disponiveis:\n");
    printf("1 - Jogar peca da frente da fila\n");
    printf("2 - Enviar peca da fila para a pilha de reserva\n");
    printf("3 - Usar peca da pilha de reserva\n");
    printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
    printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
    printf("0 - Sair\n");
    printf("Opcao escolhida: ");
}

// ---------------------------
// Função principal
// ---------------------------

int main() {
    srand((unsigned int)time(NULL));

    // 1) Inicializa a fila com 5 peças
    preencherFilaInicial();

    // 2) Pilha começa vazia (topo = -1, já inicializado globalmente)

    int opcao;
    Peca p;

    printf("=== TETRIS STACK - FILA E PILHA EM C ===\n\n");

    do {
        exibirEstadoAtual();
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            // Tratando entrada inválida
            printf("Entrada invalida. Encerrando.\n");
            break;
        }

        switch (opcao) {
            case 1: // Jogar peca da frente da fila
                if (desenfileirar(&p)) {
                    printf("Acao: Jogar peca ");
                    exibirPeca(p);
                    printf("(removida da fila)\n");

                    // Gerar nova peca e enfileirar (mantendo a fila cheia, se possivel)
                    Peca nova = gerarPeca();
                    enfileirar(nova);
                }
                break;

            case 2: // Reservar peca (fila -> pilha)
                if (pilhaCheia()) {
                    printf("[ERRO] Pilha cheia, nao e possivel reservar mais pecas.\n");
                } else if (desenfileirar(&p)) {
                    if (empilhar(p)) {
                        printf("Acao: Reservar peca ");
                        exibirPeca(p);
                        printf("(movida da fila para a pilha)\n");

                        // Gerar nova peca e enfileirar
                        Peca nova = gerarPeca();
                        enfileirar(nova);
                    } else {
                        // Se nao conseguir empilhar (por algum erro), devolve a peça à fila (opcional)
                        printf("[ALERTA] Falha ao empilhar. Peca retornada para a fila.\n");
                        enfileirar(p);
                    }
                }
                break;

            case 3: // Usar peca reservada (pilha -> jogo)
                if (desempilhar(&p)) {
                    printf("Acao: Usar peca reservada ");
                    exibirPeca(p);
                    printf("(removida da pilha)\n");

                    // A cada acao, uma nova peca e gerada e tentamos manter a fila cheia.
                    Peca nova = gerarPeca();
                    if (!filaCheia()) {
                        enfileirar(nova);
                    } else {
                        // Fila ja esta cheia, peca extra "se perde" (nao volta para o jogo)
                        // Isso respeita a ideia de que pecas removidas nao retornam.
                    }
                }
                break;

            case 4: // Trocar frente da fila com topo da pilha
                trocarFrenteComTopo();
                break;

            case 5: // Troca multipla 3x3
                trocaMultipla3();
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("[ERRO] Opcao invalida. Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    printf("Fim do Tetris Stack.\n");
    return 0;
}
