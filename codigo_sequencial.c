#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <pthread.h>
#include <string.h>

// Definições globais
#define NUM_VETORES 10
#define TAMANHO_VETOR 1000 
//#define NUM_THREADS 4


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


// Função de troca
void trocar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Algoritmo de partição
int particao(int arr[], int left, int right) {
    int pivo = arr[right]; // Pivô é sempre o ultimo elemento do vetor

    //dois ponteiros i e j, i é o ponteiro que marca a troca e j o que faz a comparação dos valores no vetor
    int i = left - 1; // começa apontando para o nada
    for (int j = left; j <= right - 1; j++) {
        //caso numero seja menor ou igual ao pivo, coloca ele para a esquerda
        if (arr[j] <= pivo) {
            i = i + 1;
            trocar(&arr[i], &arr[j]);
        }
    }
    // Troca o pivô para sua posição final
    trocar(&arr[right], &arr[i + 1]);
    return (i + 1);
}

//função quicksort
void quicksort(int arr[], int left, int right) {
    if (left < right) {
        int pi = particao(arr, left, right);
        quicksort(arr, left, pi - 1);
        quicksort(arr, pi + 1, right);
    }
}

int main(){

    // Inicializa a semente do gerador de números aleatórios
    srand(time(NULL));

    // Declaração dos vetores que serão criados
    int **vetores_sequencial;

    gerar_vetores(&vetores_sequencial, NUM_VETORES, TAMANHO_VETOR);


    //Organização da matrix sequencial
    for( int i = 0; i < NUM_VETORES; i++) {
        quicksort(vetores_sequencial[i], 0, TAMANHO_VETOR-1);
    }
}




