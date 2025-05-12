// Exemplo de uso barreira para sincronização de threads

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define THREAD_COUNT 3

// Barreira para sincronizar as threads
pthread_barrier_t mybarrier;

void* threadFn(void *id_ptr) {
    int thread_id = *(int*)id_ptr; // Recupera o ID passado
    free(id_ptr); // Libera a memória alocada dinamicamente

    // Tempo de espera aleatório entre 1 e 5 segundos
    int wait_sec = 1 + rand() % 5;
    printf("Thread %d: esperando %d segundos antes de estar pronta.\n", thread_id, wait_sec);
    sleep(wait_sec);
    printf("Thread %d: estou pronta!\n", thread_id);

    // Espera todas as threads e a main chegarem na barreira
    pthread_barrier_wait(&mybarrier);

    printf("Thread %d: todas prontas. Continuando execução!\n", thread_id);
    return NULL;
}

int main() {
    pthread_t ids[THREAD_COUNT];

    srand(time(NULL)); // Inicializa a semente para números aleatórios

    // Inicializa a barreira com THREAD_COUNT + 1 (threads + main)
    pthread_barrier_init(&mybarrier, NULL, THREAD_COUNT + 1);

    for (int i = 0; i < THREAD_COUNT; i++) {
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&ids[i], NULL, threadFn, arg);
    }

    printf("Main: esperando todas as threads ficarem prontas...\n");

    // A main também espera na barreira
    pthread_barrier_wait(&mybarrier);

    printf("Main: todas as threads prontas! Continuando...\n");

    // Aguarda todas as threads terminarem
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(ids[i], NULL);
    }

    pthread_barrier_destroy(&mybarrier);

    return 0;
}
