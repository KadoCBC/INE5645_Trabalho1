# Relatório de Performance - INE5645 Trabalho 1

## Resumo Executivo

Este relatório apresenta os resultados dos testes de performance realizados em três implementações diferentes para ordenação de vetores usando o algoritmo QuickSort:

1. **Implementação Sequencial**: Processamento sequencial das tarefas
2. **Implementação Paralela**: Processamento paralelo usando pthreads
3. **Implementação com Biblioteca**: Uso de uma biblioteca de thread pool

## Configuração dos Testes

- **Número de vetores**: 10
- **Tamanho de cada vetor**: 500.000 elementos
- **Número de threads**: 2
- **Algoritmo**: QuickSort
- **Número de execuções**: 10 para cada implementação
- **Sistema**: Linux 6.14.0-29-generic

## Resultados Detalhados

### 1. Programa Sequencial

**Estatísticas:**
- **Média**: 1.658136 segundos
- **Desvio Padrão**: 0.031236 segundos
- **Mínimo**: 1.620524 segundos
- **Máximo**: 1.742659 segundos
- **Mediana**: 1.651652 segundos

**Dados brutos**: [1.66684, 1.637751, 1.620524, 1.636722, 1.742659, 1.66549, 1.660569, 1.651652, 1.64966, 1.649497]

### 2. Programa Paralelo

**Estatísticas:**
- **Média**: 0.879553 segundos
- **Desvio Padrão**: 0.055227 segundos
- **Mínimo**: 0.852119 segundos
- **Máximo**: 1.043952 segundos
- **Mediana**: 0.861767 segundos

**Dados brutos**: [0.861767, 0.864938, 0.856221, 1.043952, 0.855726, 0.852119, 0.872274, 0.855242, 0.859539, 0.873752]

### 3. Programa Biblioteca

**Estatísticas:**
- **Média**: 0.087462 segundos
- **Desvio Padrão**: 0.262150 segundos
- **Mínimo**: 0.000057 segundos
- **Máximo**: 0.873913 segundos
- **Mediana**: 0.000086 segundos

**Dados brutos**: [0.873913, 9.7e-05, 8.7e-05, 7.4e-05, 6.7e-05, 8.1e-05, 9.4e-05, 5.7e-05, 6.8e-05, 8.6e-05]

**⚠️ Observação**: Os tempos da biblioteca apresentam grande variabilidade, com alguns valores extremamente baixos que indicam possível problema na medição de tempo.

## Análise de Performance

### Speedup Calculado

1. **Paralelo vs Sequencial**: 1.89x (89% de melhoria)
2. **Biblioteca vs Sequencial**: 18.96x (94.7% de melhoria) - *Valores questionáveis*
3. **Biblioteca vs Paralelo**: 10.06x (90.1% de melhoria) - *Valores questionáveis*

### Eficiência do Paralelismo

Para o programa paralelo com 2 threads:
- **Speedup teórico máximo**: 2.0x
- **Speedup observado**: 1.89x
- **Eficiência**: 94.5% (muito boa)

## Conclusões

### ✅ Implementação Paralela

A implementação paralela demonstrou **melhoria significativa de performance**:

- **47.0% mais rápida** que a implementação sequencial
- **Speedup de 1.89x** com 2 threads
- **Eficiência de 94.5%**, indicando excelente aproveitamento do paralelismo
- **Baixa variabilidade** nos tempos (desvio padrão baixo)

### ⚠️ Implementação da Biblioteca

A implementação da biblioteca apresenta **problemas de medição**:

- Tempos inconsistentes com grande variabilidade
- Alguns valores extremamente baixos (microssegundos) que não são realistas
- Possível problema na sincronização ou medição de tempo
- **Recomendação**: Investigar e corrigir a implementação da biblioteca

### 📊 Comparação Geral

1. **Melhor Performance**: Implementação Paralela (confiável e eficiente)
2. **Base de Comparação**: Implementação Sequencial (estável)
3. **Necessita Correção**: Implementação Biblioteca (tempos inconsistentes)

## Recomendações

1. **Para uso em produção**: Utilize a implementação paralela, que demonstrou performance consistente e confiável
2. **Para a biblioteca**: Investigar e corrigir os problemas de sincronização que estão causando tempos incorretos
3. **Para futuros testes**: Considerar aumentar o número de execuções e usar diferentes tamanhos de vetores para análise mais robusta

## Arquivos Gerados

- `tempos_sequencial.txt`: Dados brutos da implementação sequencial
- `tempos_paralelo.txt`: Dados brutos da implementação paralela
- `tempos_biblioteca.txt`: Dados brutos da implementação da biblioteca
- `analyze_results_simple.py`: Script de análise dos resultados
- `test_performance.sh`: Script de execução dos testes

---

**Data do Relatório**: 12 de outubro de 2025  
**Sistema de Teste**: Linux 6.14.0-29-generic  
**Compilador**: GCC com flags -lm -lpthread
