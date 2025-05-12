#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *PrintHello(void *threadid) {
  long id;
  id = (long)threadid;
  printf("Thread %ld: Olá mundo!\n", id);
  pthread_exit(NULL);
}

int main() {
  pthread_t t1, t2;
  int create;
  long num;

  num = 1;
  printf("Main: criando a thread %ld\n", num);
  create = pthread_create(&t1, NULL, PrintHello, (void *)num);

  num = 2;
  printf("Main: criando a thread %ld\n", num);
  create = pthread_create(&t2, NULL, PrintHello, (void *)num);

  // 🔒 Espera as duas threads terminarem
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  printf("Main: finalizando\n");
  return 0;
}
