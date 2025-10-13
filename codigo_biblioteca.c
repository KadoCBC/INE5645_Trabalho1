#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "paralel.h"
#include <pthread.h>
#include <unistd.h>

// --- DEFINIÇÕES GLOBAIS ---
#define NUM_VETORES 10
#define TAMANHO_VETOR 500000
#define NUM_THREADS 2

void gerar_vetores(int ***matriz_vetores, int num_vetores, int tamanho) {
    // 1. Aloca memória para o array de ponteiros (a "matriz" que aponta para os vetores)
    *matriz_vetores = (int **)malloc(num_vetores * sizeof(int *));

    if (*matriz_vetores == NULL) {
        perror("Falha ao alocar memória para a matriz de vetores");
        exit(EXIT_FAILURE);
    }

    // 2. Aloca memória e preenche cada vetor individualmente
    for (int i = 0; i < num_vetores; i++) {
        (*matriz_vetores)[i] = (int *)malloc(tamanho * sizeof(int));
        
        if ((*matriz_vetores)[i] == NULL) {
            perror("Falha ao alocar memória para o vetor");
            // Libera o que já foi alocado em caso de erro
            for (int k = 0; k < i; k++) free((*matriz_vetores)[k]);
            free(*matriz_vetores);
            exit(EXIT_FAILURE);
        }

        // 3. Preenche o vetor com números aleatórios
        for (int j = 0; j < tamanho; j++) {
            (*matriz_vetores)[i][j] = rand(); 
        }
    }
}


void liberar_vetores(int** matriz, int num_vetores) {
    for (int i = 0; i < num_vetores; i++) {
        free(matriz[i]);
    }
    free(matriz);
};

void trocar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
};

int particao(int arr[], int left, int right) {
    int pivo = arr[right];
    int i = left - 1;
    for (int j = left; j <= right - 1; j++) {
        if (arr[j] <= pivo) {
            i++;
            trocar(&arr[i], &arr[j]);
        }
    }
    trocar(&arr[i + 1], &arr[right]);
    return (i + 1);
};

void quicksort(int arr[], int left, int right) {
    if (left < right) {
        int pi = particao(arr, left, right);
        quicksort(arr, left, pi - 1);
        quicksort(arr, pi + 1, right);
    }
};

//Adaptação para usar a biblioteca
void tarefa_ordenar(void* arg) {

    // 1. Converte o argumento genérico `void*` de volta para o tipo que sabemos que ele é: `int*`
    int *vetor_a_ordenar = (int*)arg;

    // 2. Chama a função de trabalho real com os parâmetros corretos
    quicksort(vetor_a_ordenar, 0, TAMANHO_VETOR - 1);
};



int main() {
    // Inicializa a semente do gerador de números aleatórios
    srand(time(NULL));

    // Declaração dos vetores que serão criados
    int **vetores;

    gerar_vetores(&vetores, NUM_VETORES, TAMANHO_VETOR);
    
    struct timeval t1, t2;
    double tempo_paralelo;

    // Cria a pool de threads
    ThreadPool* pool = pool_init(NUM_THREADS, NUM_VETORES);
    if (pool == NULL) {
        fprintf(stderr, "Falha ao criar a pool de threads.\n");
        return 1;
    }

    gettimeofday(&t1, NULL);
    
    // Submete cada tarefa de ordenação para a pool
    for (int i = 0; i < NUM_VETORES; i++) {
        // O argumento é o ponteiro para o i vetor
        execute(pool, tarefa_ordenar, vetores[i]);
    }
    pool_destroy(pool);
    
    gettimeofday(&t2, NULL);
    
    tempo_paralelo = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);
    printf("Tempo total paralelo   = %f segundos\n", tempo_paralelo);

    
    liberar_vetores(vetores, NUM_VETORES);

    return 0;
}