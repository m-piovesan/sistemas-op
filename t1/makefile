# Nome do compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -O2 -std=c11 -D_GNU_SOURCE

# Nome do arquivo fonte e do executável
SRC = main_formatada.c
EXEC = main_formatada

# Regras de compilação

# A regra padrão, que compila o código
all: $(EXEC)

# Como compilar o programa
$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC) -lpthread

# Limpeza dos arquivos gerados
clean:
	rm -f $(EXEC)

# Regra para limpar e compilar tudo novamente
rebuild: clean all
