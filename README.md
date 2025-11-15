# 🧱 Tetris Stack em C — Fila Circular e Pilha de Peças

Este projeto foi desenvolvido como parte da disciplina de **Análise e Desenvolvimento de Sistemas**, simulando a lógica de gerenciamento de peças do jogo **Tetris Stack** utilizando as estruturas de dados **fila circular** e **pilha**.

O foco é controlar a entrada, saída, reserva e troca de peças de forma organizada, aplicando os conceitos de **fila (FIFO)** e **pilha (LIFO)** em linguagem C.

---

## 🎯 Objetivos

- Implementar uma **fila circular** de peças futuras com capacidade fixa (5 peças).
- Implementar uma **pilha** de peças reservadas com capacidade fixa (3 peças).
- Simular ações do jogador:
  - Jogar peças (remover da fila)
  - Reservar peças (mover da fila para a pilha)
  - Usar peças reservadas (remover da pilha)
  - Trocar peças entre fila e pilha (inclusive trocas múltiplas)
- Manter a **fila sempre cheia**, gerando novas peças automaticamente sempre que possível.
- Aplicar:
  - `struct`, arrays e variáveis
  - operadores e condicionais
  - funções, modularização lógica e fluxo de controle

---

## 🧱 Estrutura de Dados

### Struct `Peca`

```c
typedef struct {
    char nome;  // 'I', 'O', 'T', 'L'
    int id;     // identificador unico
} Peca;
