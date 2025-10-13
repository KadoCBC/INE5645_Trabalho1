#!/bin/bash

# Script para testar performance dos três programas
# Executa cada programa múltiplas vezes e coleta os tempos

NUM_EXECUCOES=10
DIRETORIO="/home/mateus/Documentos/projeto_c/INE5645_Trabalho1"

echo "=== TESTE DE PERFORMANCE ==="
echo "Executando cada programa $NUM_EXECUCOES vezes"
echo "Data: $(date)"
echo ""

# Arquivos para armazenar os resultados
SEQUENCIAL_FILE="$DIRETORIO/tempos_sequencial.txt"
PARALELO_FILE="$DIRETORIO/tempos_paralelo.txt"
BIBLIOTECA_FILE="$DIRETORIO/tempos_biblioteca.txt"

# Limpa arquivos anteriores
rm -f "$SEQUENCIAL_FILE" "$PARALELO_FILE" "$BIBLIOTECA_FILE"

echo "Testando programa SEQUENCIAL..."
for i in $(seq 1 $NUM_EXECUCOES); do
    echo "Execução $i/$NUM_EXECUCOES"
    cd "$DIRETORIO"
    ./sequencial 2>&1 | grep "tempo total" | awk '{print $4}' >> "$SEQUENCIAL_FILE"
done

echo ""
echo "Testando programa PARALELO..."
for i in $(seq 1 $NUM_EXECUCOES); do
    echo "Execução $i/$NUM_EXECUCOES"
    cd "$DIRETORIO"
    ./paralelo 2>&1 | grep "tempo total" | awk '{print $4}' >> "$PARALELO_FILE"
done

echo ""
echo "Testando programa BIBLIOTECA..."
for i in $(seq 1 $NUM_EXECUCOES); do
    echo "Execução $i/$NUM_EXECUCOES"
    cd "$DIRETORIO"
    ./biblioteca 2>&1 | grep "Tempo total paralelo" | awk '{print $5}' >> "$BIBLIOTECA_FILE"
done

echo ""
echo "=== RESULTADOS COLETADOS ==="
echo "Arquivos gerados:"
echo "- $SEQUENCIAL_FILE"
echo "- $PARALELO_FILE" 
echo "- $BIBLIOTECA_FILE"
echo ""
echo "Teste concluído!"
