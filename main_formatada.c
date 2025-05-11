#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define MAX_PROCESSOS 5

// ------------------- ESTRUTURAS -------------------

typedef struct {
    int contador;
    int total;
    sem_t mutex;
    sem_t turno;
} barreira_t;

typedef struct {
    int fila[MAX_PROCESSOS];
    int inicio;
    int fim;
    int usando;
    sem_t mutex;
    sem_t espera[MAX_PROCESSOS];
} fila_fifo_t;

// ------------------- FUNÇÕES DE BARRREIRA -------------------

void init_barreira(barreira_t *b, int total) {
    b->contador = 0;
    b->total = total;
    sem_init(&b->mutex, 1, 1);
    sem_init(&b->turno, 1, 0);
}

void espera_barreira(barreira_t *b) {
    sem_wait(&b->mutex);
    b->contador++;
    if (b->contador == b->total) {
        for (int i = 0; i < b->total - 1; i++)
            sem_post(&b->turno);
        b->contador = 0;
        sem_post(&b->mutex);
    } else {
        sem_post(&b->mutex);
        sem_wait(&b->turno);
    }
}

// ------------------- FUNÇÕES DE FILA FIFO -------------------

void init_fila(fila_fifo_t *f) {
    f->inicio = 0;
    f->fim = 0;
    f->usando = 0;
    sem_init(&f->mutex, 1, 1);
    for (int i = 0; i < MAX_PROCESSOS; i++) {
        sem_init(&f->espera[i], 1, 0);
    }
}

void inicia_uso(int recurso, fila_fifo_t *f, int Pi) {
    sem_wait(&f->mutex);
    if (f->usando == 0 && f->inicio == f->fim) {
        f->usando = 1;
        sem_post(&f->mutex);
    } else {
        f->fila[f->fim] = Pi;
        f->fim = (f->fim + 1) % MAX_PROCESSOS;
        sem_post(&f->mutex);
        sem_wait(&f->espera[Pi]);
    }
}

void termina_uso(int recurso, fila_fifo_t *f) {
    sem_wait(&f->mutex);
    if (f->inicio != f->fim) {
        int proximo = f->fila[f->inicio];
        f->inicio = (f->inicio + 1) % MAX_PROCESSOS;
        sem_post(&f->espera[proximo]);
    } else {
        f->usando = 0;
    }
    sem_post(&f->mutex);
}

// ------------------- FUNÇÃO DO FILHO -------------------

void processo_filho(int Pi, barreira_t *b, fila_fifo_t *f, int recurso) {
    srand(getpid());
    printf("--Processo: %d chegando na barreira\n", Pi);
    espera_barreira(b);
    printf("**Processo: %d saindo da barreira\n", Pi);

    for (int uso = 0; uso < 3; uso++) {
        int s;

        // Prólogo
        s = 1 + rand() % 3;
        printf("Processo: %d Prologo: %d de %d segundos\n", Pi, uso, s);
        sleep(s);

        // Tenta iniciar uso
        inicia_uso(recurso, f, Pi);

        // Uso
        s = 1 + rand() % 3;
        printf("Processo: %d USO: %d por %d segundos\n", Pi, uso, s);
        sleep(s);

        // Termina uso
        termina_uso(recurso, f);

        // Epílogo
        s = 1 + rand() % 3;
        printf("Processo: %d Epilogo: %d de %d segundos\n", Pi, uso, s);
        sleep(s);
    }

    printf("--Processo: %d chegando novamente na barreira\n", Pi);
    espera_barreira(b);
    printf("++Processo: %d saindo da barreira novamente\n", Pi);
    exit(Pi);
}

// ------------------- FUNÇÃO PRINCIPAL -------------------

int main() {
    int i;
    pid_t filhos[MAX_PROCESSOS];

    // Inicializa gerador de números aleatórios
    srand(time(NULL));

    // Aloca memória compartilhada
    barreira_t *b = mmap(NULL, sizeof(barreira_t), PROT_READ | PROT_WRITE,
                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    fila_fifo_t *f = mmap(NULL, sizeof(fila_fifo_t), PROT_READ | PROT_WRITE,
                          MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // Inicializa estruturas
    init_barreira(b, MAX_PROCESSOS);
    init_fila(f);

    // Gera número aleatório do recurso (herdado pelos filhos)
    int recurso = rand() % 100;

    // Cria filhos
    for (i = 0; i < MAX_PROCESSOS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            processo_filho(i, b, f, recurso);
        } else {
            filhos[i] = pid;
        }
    }

    // Pai espera filhos terminarem
    for (i = 0; i < MAX_PROCESSOS; i++) {
        int status;
        pid_t pid = wait(&status);
        int Pi = WEXITSTATUS(status);
        printf("+++ Filho de número lógico %d e pid %d terminou!\n", Pi, pid);
    }

    // Destrói semáforos
    sem_destroy(&b->mutex);
    sem_destroy(&b->turno);
    munmap(b, sizeof(barreira_t));

    sem_destroy(&f->mutex);
    for (i = 0; i < MAX_PROCESSOS; i++) {
        sem_destroy(&f->espera[i]);
    }
    munmap(f, sizeof(fila_fifo_t));

    return 0;
}
