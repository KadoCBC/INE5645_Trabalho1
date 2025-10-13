#!/usr/bin/env python3
import math

def read_times(filename):
    """Lê os tempos de um arquivo"""
    with open(filename, 'r') as f:
        times = []
        for line in f:
            line = line.strip()
            if line:
                try:
                    times.append(float(line))
                except ValueError:
                    continue
    return times

def calculate_stats(times):
    """Calcula estatísticas dos tempos"""
    if not times:
        return None
    
    mean = sum(times) / len(times)
    
    # Calcula desvio padrão
    variance = sum((x - mean) ** 2 for x in times) / len(times)
    std = math.sqrt(variance)
    
    return {
        'mean': mean,
        'std': std,
        'min': min(times),
        'max': max(times),
        'median': sorted(times)[len(times)//2],
        'count': len(times)
    }

def main():
    # Lê os dados
    sequencial_times = read_times('tempos_sequencial.txt')
    paralelo_times = read_times('tempos_paralelo.txt')
    biblioteca_times = read_times('tempos_biblioteca.txt')
    
    print("=== ANÁLISE DE PERFORMANCE ===\n")
    
    # Calcula estatísticas
    seq_stats = calculate_stats(sequencial_times)
    par_stats = calculate_stats(paralelo_times)
    bib_stats = calculate_stats(biblioteca_times)
    
    print("PROGRAMA SEQUENCIAL:")
    if seq_stats:
        print(f"  Média: {seq_stats['mean']:.6f} segundos")
        print(f"  Desvio Padrão: {seq_stats['std']:.6f} segundos")
        print(f"  Mínimo: {seq_stats['min']:.6f} segundos")
        print(f"  Máximo: {seq_stats['max']:.6f} segundos")
        print(f"  Mediana: {seq_stats['median']:.6f} segundos")
        print(f"  Execuções: {seq_stats['count']}")
    print()
    
    print("PROGRAMA PARALELO:")
    if par_stats:
        print(f"  Média: {par_stats['mean']:.6f} segundos")
        print(f"  Desvio Padrão: {par_stats['std']:.6f} segundos")
        print(f"  Mínimo: {par_stats['min']:.6f} segundos")
        print(f"  Máximo: {par_stats['max']:.6f} segundos")
        print(f"  Mediana: {par_stats['median']:.6f} segundos")
        print(f"  Execuções: {par_stats['count']}")
    print()
    
    print("PROGRAMA BIBLIOTECA:")
    if bib_stats:
        print(f"  Média: {bib_stats['mean']:.6f} segundos")
        print(f"  Desvio Padrão: {bib_stats['std']:.6f} segundos")
        print(f"  Mínimo: {bib_stats['min']:.6f} segundos")
        print(f"  Máximo: {bib_stats['max']:.6f} segundos")
        print(f"  Mediana: {bib_stats['median']:.6f} segundos")
        print(f"  Execuções: {bib_stats['count']}")
    print()
    
    # Calcula speedup
    if seq_stats and par_stats:
        speedup_paralelo = seq_stats['mean'] / par_stats['mean']
        print(f"SPEEDUP PARALELO vs SEQUENCIAL: {speedup_paralelo:.2f}x")
    
    if seq_stats and bib_stats and bib_stats['mean'] > 0.001:  # Só se não for um erro
        speedup_biblioteca = seq_stats['mean'] / bib_stats['mean']
        print(f"SPEEDUP BIBLIOTECA vs SEQUENCIAL: {speedup_biblioteca:.2f}x")
    
    if par_stats and bib_stats and bib_stats['mean'] > 0.001:
        speedup_bib_vs_par = par_stats['mean'] / bib_stats['mean']
        print(f"SPEEDUP BIBLIOTECA vs PARALELO: {speedup_bib_vs_par:.2f}x")
    
    print("\n=== CONCLUSÕES ===")
    
    if seq_stats and par_stats:
        if par_stats['mean'] < seq_stats['mean']:
            improvement = ((seq_stats['mean'] - par_stats['mean']) / seq_stats['mean']) * 100
            print(f"✓ O programa paralelo é {improvement:.1f}% mais rápido que o sequencial")
        else:
            print("✗ O programa paralelo não melhorou a performance")
    
    if bib_stats and bib_stats['mean'] < 0.001:
        print("✗ O programa da biblioteca apresenta tempos incorretos (muito baixos)")
        print("  Isso indica um problema na implementação da biblioteca")
    elif seq_stats and bib_stats and bib_stats['mean'] > 0.001:
        if bib_stats['mean'] < seq_stats['mean']:
            improvement = ((seq_stats['mean'] - bib_stats['mean']) / seq_stats['mean']) * 100
            print(f"✓ O programa da biblioteca é {improvement:.1f}% mais rápido que o sequencial")
        else:
            print("✗ O programa da biblioteca não melhorou a performance")
    
    print(f"\nConfiguração do teste:")
    print(f"- Número de vetores: 10")
    print(f"- Tamanho de cada vetor: 500.000 elementos")
    print(f"- Número de threads: 2")
    print(f"- Algoritmo: QuickSort")
    
    print(f"\nDados brutos:")
    print(f"Sequencial: {sequencial_times}")
    print(f"Paralelo: {paralelo_times}")
    print(f"Biblioteca: {biblioteca_times}")

if __name__ == "__main__":
    main()
