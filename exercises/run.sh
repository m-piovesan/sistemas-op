#!/bin/bash

# Função clean: remove apenas os executáveis (sem extensão .c)
if [ "$1" == "clean" ]; then
  echo "Removendo executáveis ex* (sem .c)..."
  find . -maxdepth 1 -type f -executable -name 'ex[0-9]*' ! -name '*.c' -exec rm -f {} +
  exit 0
fi

# Verifica se um número foi passado
if [ -z "$1" ]; then
  echo "Uso: ./run.sh <número do exercício> ou ./run.sh clean"
  exit 1
fi

FILE="ex$1.c"
OUTPUT="ex$1"

# Verifica se o arquivo existe
if [ ! -f "$FILE" ]; then
  echo "Arquivo $FILE não encontrado."
  exit 1
fi

# Compila e executa
gcc "$FILE" -o "$OUTPUT" -lpthread -lrt && ./"$OUTPUT"
