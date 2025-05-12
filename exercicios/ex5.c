// Exemplo de uso de semáforo para sincronização de threads

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

sem_t mutex;

// Função que cada thread executa
void* thread(void* arg) {
    long id = (long)arg;

    // Região crítica protegida por semáforo
    sem_wait(&mutex);
    printf("Thread %ld: entrou na região crítica.\n", id);

    sleep(4); // Simula trabalho dentro da região crítica

    printf("Thread %ld: saindo da região crítica.\n", id);
    sem_post(&mutex);

    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Inicializa o semáforo com valor 1 (semáforo binário)
    sem_init(&mutex, 0, 1);

    // Aloca memória para passar IDs distintos a cada thread
    long *id1 = malloc(sizeof(*id1));
    long *id2 = malloc(sizeof(*id2));
    *id1 = 1;
    *id2 = 2;

    // Cria a primeira thread
    pthread_create(&t1, NULL, thread, id1);
    sleep(2); // Garante que t1 entre primeiro

    // Cria a segunda thread
    pthread_create(&t2, NULL, thread, id2);

    // Aguarda as threads terminarem
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Limpa o semáforo e a memória alocada
    sem_destroy(&mutex);
    free(id1);
    free(id2);

    return 0;
}
