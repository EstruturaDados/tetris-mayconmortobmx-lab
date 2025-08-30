#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FILA_CAP 5
#define PILHA_CAP 3

typedef struct {
    char tipo;  // tipo da peça: 'I', 'O', 'T', 'L', 'J', 'S', 'Z'
    int id;
} Peca;

// ---------------- Fila ----------------
typedef struct {
    Peca itens[FILA_CAP];
    int frente;
    int tras;
    int tamanho;
} Fila;

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = 0;
    f->tamanho = 0;
}

int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

int filaCheia(Fila *f) {
    return f->tamanho == FILA_CAP;
}

Peca gerarPeca(int id) {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    p.tipo = tipos[rand() % 7];
    p.id = id;
    return p;
}

void enqueue(Fila *f, Peca p) {
    if(filaCheia(f)) return;
    f->itens[f->tras] = p;
    f->tras = (f->tras + 1) % FILA_CAP;
    f->tamanho++;
}

Peca dequeue(Fila *f) {
    Peca p = {'X', -1};
    if(filaVazia(f)) return p;
    p = f->itens[f->frente];
    f->frente = (f->frente + 1) % FILA_CAP;
    f->tamanho--;
    return p;
}

void mostrarFila(Fila *f) {
    printf("Fila: ");
    for(int i=0, idx=f->frente; i < f->tamanho; i++, idx=(idx+1)%FILA_CAP) {
        printf("[%c-%d] ", f->itens[idx].tipo, f->itens[idx].id);
    }
    printf("\n");
}

// ---------------- Pilha ----------------
typedef struct {
    Peca itens[PILHA_CAP];
    int topo;
} Pilha;

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == PILHA_CAP-1;
}

void push(Pilha *p, Peca item) {
    if(pilhaCheia(p)) return;
    p->topo++;
    p->itens[p->topo] = item;
}

Peca pop(Pilha *p) {
    Peca item = {'X', -1};
    if(pilhaVazia(p)) return item;
    item = p->itens[p->topo];
    p->topo--;
    return item;
}

void mostrarPilha(Pilha *p) {
    printf("Pilha: ");
    for(int i=0; i <= p->topo; i++) {
        printf("[%c-%d] ", p->itens[i].tipo, p->itens[i].id);
    }
    printf("\n");
}

// ---------------- Trocas Mestre ----------------
void trocarFrenteTopo(Fila *f, Pilha *p) {
    if(filaVazia(f) || pilhaVazia(p)) {
        printf("Não é possível trocar. Fila ou Pilha vazia.\n");
        return;
    }
    Peca temp = f->itens[f->frente];
    f->itens[f->frente] = p->itens[p->topo];
    p->itens[p->topo] = temp;
    printf("Peça da frente da fila trocada com topo da pilha.\n");
}

void trocarTres(Fila *f, Pilha *p) {
    if(f->tamanho < 3 || p->topo < 2) {
        printf("Não é possível trocar. Fila ou pilha não têm 3 peças.\n");
        return;
    }
    for(int i=0; i<3; i++) {
        int idxFila = (f->frente + i) % FILA_CAP;
        Peca temp = f->itens[idxFila];
        f->itens[idxFila] = p->itens[i];
        p->itens[i] = temp;
    }
    printf("Os 3 primeiros da fila trocados com as 3 peças da pilha.\n");
}

// ---------------- Menu ----------------
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int idSeq = 1;
    for(int i=0; i<FILA_CAP; i++) enqueue(&fila, gerarPeca(idSeq++));

    int opcao;
    do {
        mostrarFila(&fila);
        mostrarPilha(&pilha);

        printf("\nMenu:\n");
        printf("1 - Jogar peça (remover da frente)\n");
        printf("2 - Enviar peça da fila para a reserva (pilha)\n");
        printf("3 - Usar peça da reserva (remover do topo)\n");
        printf("4 - Trocar peça da frente com topo da pilha\n");
        printf("5 - Trocar 3 primeiros da fila com os 3 da pilha\n");
        printf("0 - Sair\nEscolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: {
                Peca jogada = dequeue(&fila);
                printf("Jogou: [%c-%d]\n", jogada.tipo, jogada.id);
                enqueue(&fila, gerarPeca(idSeq++));
                break;
            }
            case 2: {
                if(!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca p = dequeue(&fila);
                    push(&pilha, p);
                    enqueue(&fila, gerarPeca(idSeq++));
                    printf("Peça enviada para reserva.\n");
                } else {
                    printf("Não é possível enviar para pilha.\n");
                }
                break;
            }
            case 3: {
                if(!pilhaVazia(&pilha)) {
                    Peca p = pop(&pilha);
                    printf("Usou peça da reserva: [%c-%d]\n", p.tipo, p.id);
                } else printf("Pilha vazia!\n");
                break;
            }
            case 4: trocarFrenteTopo(&fila, &pilha); break;
            case 5: trocarTres(&fila, &pilha); break;
            case 0: break;
            default: printf("Opção inválida!\n");
        }

    } while(opcao != 0);

    return 0;
}
