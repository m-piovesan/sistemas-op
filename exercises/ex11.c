#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

int counter = 1;          // Próximo número a ser impresso
pthread_mutex_t mutex;    // Protege o contador
sem_t sem1, sem2;         // Sincroniza as threads

void *Thread1(void *arg) {
    while (1) {
        sem_wait(&sem1);  // Espera sua vez
        pthread_mutex_lock(&mutex);
        
        if (counter > 10) {  // Condição de parada
            pthread_mutex_unlock(&mutex);
            sem_post(&sem2); // Libera a outra thread para evitar deadlock
            break;
        }
        
        printf("Thread 1: %d\n", counter++);
        pthread_mutex_unlock(&mutex);
        sem_post(&sem2);    // Passa a vez para Thread 2
    }
    pthread_exit(NULL);
}

void *Thread2(void *arg) {
    while (1) {
        sem_wait(&sem2);  // Espera sua vez
        pthread_mutex_lock(&mutex);
        
        if (counter > 10) {  // Condição de parada
            pthread_mutex_unlock(&mutex);
            sem_post(&sem1); // Libera a outra thread para evitar deadlock
            break;
        }
        
        printf("Thread 2: %d\n", counter++);
        pthread_mutex_unlock(&mutex);
        sem_post(&sem1);    // Passa a vez para Thread 1
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2;
    
    // Inicializa semáforos e mutex
    sem_init(&sem1, 0, 1);  // Thread 1 começa primeiro
    sem_init(&sem2, 0, 0);  // Thread 2 bloqueada inicialmente
    pthread_mutex_init(&mutex, NULL);
    
    // Cria threads
    pthread_create(&t1, NULL, Thread1, NULL);
    pthread_create(&t2, NULL, Thread2, NULL);
    
    // Espera threads terminarem
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    // Libera recursos
    sem_destroy(&sem1);
    sem_destroy(&sem2);
    pthread_mutex_destroy(&mutex);
    
    printf("Main: finalizando\n");
    return 0;
}