# Trabalho de Sistemas Operacionais — Sincronização com Semáforos

Este projeto foi desenvolvido como parte de um trabalho da disciplina de Sistemas Operacionais. O objetivo é aplicar conceitos de **sincronização de processos** utilizando **semáforos POSIX** e memória compartilhada com `mmap`.

## Descrição

O programa simula um ambiente com múltiplos processos (5 no total) que precisam:

- Aguardar uns pelos outros em uma **barreira de sincronização** antes de começar.
- Acessar um **recurso crítico compartilhado** de forma exclusiva, respeitando uma **fila FIFO**.
- Repetir o uso do recurso múltiplas vezes, com seções de prólogo e epílogo simulando atividades externas.
- Finalizar aguardando novamente todos os processos na barreira antes de encerrar.

Todos os mecanismos de controle de acesso são implementados com **semáforos POSIX** (`sem_t`) e memória compartilhada entre processos filhos criados com `fork()`.

## Estruturas principais

- `barreira_t`: controla a sincronização por barreira.
- `fila_fifo_t`: gerencia o acesso ordenado ao recurso compartilhado.

## Como compilar

Utilize o `Makefile` incluído:

```bash
make
```
