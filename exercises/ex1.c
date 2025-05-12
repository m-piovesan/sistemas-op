// Implementar uma estrutura de dados tipo Pilha (de inteiros) e
// suas operações (push e pop) de modo que possam ser usadas
// em threads concorrentes. Supor existencia de semaforos e suas operações 
// (mostrar e descrever as operações de semaforos que foram usadas).

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_SIZE 10

typedef struct {
    int data[MAX_SIZE];
    int top;
    sem_t mutex;     // Controla acesso à pilha (exclusão mútua)
    sem_t empty;     // Conta espaços vazios (bloqueia pop se 0)
    sem_t full;      // Conta itens na pilha (bloqueia push se 0)
} Stack;

// Inicializa a pilha
void init_stack(Stack *s) {
    s->top = -1;
    sem_init(&s->mutex, 0, 1);   // Mutex inicia liberado (1)
    sem_init(&s->empty, 0, MAX_SIZE);  // Inicialmente, MAX_SIZE espaços vazios
    sem_init(&s->full, 0, 0);    // Inicialmente, 0 itens na pilha
}

// Push: adiciona um item à pilha
void push(Stack *s, int item) {
    sem_wait(&s->empty);  // Aguarda se a pilha estiver cheia
    sem_wait(&s->mutex);  // Entra na região crítica

    s->data[++s->top] = item;
    printf("Push: %d (topo = %d)\n", item, s->top);

    sem_post(&s->mutex);  // Sai da região crítica
    sem_post(&s->full);   // Incrementa o contador de itens
}

// Pop: remove e retorna o item do topo
int pop(Stack *s) {
    sem_wait(&s->full);   // Aguarda se a pilha estiver vazia
    sem_wait(&s->mutex);  // Entra na região crítica

    int item = s->data[s->top--];
    printf("Pop: %d (topo = %d)\n", item, s->top);

    sem_post(&s->mutex);  // Sai da região crítica
    sem_post(&s->empty);  // Incrementa o contador de espaços vazios
    return item;
}

// Função executada pelas threads (teste)
void* thread_func(void *arg) {
    Stack *s = (Stack*) arg;
    for (int i = 0; i < 5; i++) {
        push(s, i);
        int item = pop(s);
    }
    return NULL;
}

int main() {
    Stack s;
    init_stack(&s);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread_func, &s);
    pthread_create(&t2, NULL, thread_func, &s);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&s.mutex);
    sem_destroy(&s.empty);
    sem_destroy(&s.full);

    return 0;
}