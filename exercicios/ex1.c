// Implementar uma estrutura de dados tipo Pilha (de inteiros) e
// suas operações (push e pop) de modo que possam ser usadas
// em threads concorrentes. Supor existencia de semaforos e suas operações
// (mostrar e descrever as operações de semaforos que foram usadas).

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 100

// Estrutura da pilha
typedef struct {
    int dados[MAX];
    int topo;
    sem_t mutex;  // semáforo binário (tipo mutex)
} Pilha;

Pilha minhaPilha;

// Inicializa a pilha
void inicializa(Pilha *p) {
    p->topo = -1;
    sem_init(&(p->mutex), 0, 1); // inicia semáforo com 1 (liberado)
}

// Push (empilha)
void push(Pilha *p, int valor) {
    sem_wait(&(p->mutex));  // início da região crítica

    if (p->topo < MAX - 1) {
        p->topo++;
        p->dados[p->topo] = valor;
        printf("Push: %d\n", valor);
    } else {
        printf("Pilha cheia!\n");
    }

    sem_post(&(p->mutex));  // fim da região crítica
}

// Pop (desempilha)
int pop(Pilha *p) {
    sem_wait(&(p->mutex));  // início da região crítica

    int valor = -1;
    if (p->topo >= 0) {
        valor = p->dados[p->topo];
        p->topo--;
        printf("Pop: %d\n", valor);
    } else {
        printf("Pilha vazia!\n");
    }

    sem_post(&(p->mutex));  // fim da região crítica
    return valor;
}

void* produtor(void* arg) {
    for (int i = 0; i < 10; i++) {
        push(&minhaPilha, i);
    }
    return NULL;
}

void* consumidor(void* arg) {
    for (int i = 0; i < 10; i++) {
        pop(&minhaPilha);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    inicializa(&minhaPilha);

    pthread_create(&t1, NULL, produtor, NULL);
    pthread_create(&t2, NULL, consumidor, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&minhaPilha.mutex);

    return 0;
}
