#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

// Definições globais
#define NUM_VETORES 10
#define TAMANHO_VETOR 1000 
#define NUM_THREADS 2

int **matriz;


void gerar_vetores(int num_vetores, int tamanho) {
    
    // Atribui a memória alocada à matriz
    matriz = (int **)malloc(num_vetores * sizeof(int *));

    if (matriz == NULL) {
        perror("Falha ao alocar memória para a matriz de vetores");
        exit(EXIT_FAILURE);
    }

    // --- 2. ALOCAÇÃO E PREENCHIMENTO DE CADA VETOR INTERNO ---
    for (int i = 0; i < num_vetores; i++) {
        
        // Aloca o vetor de inteiros e armazena o ponteiro na posição 'i' da matriz global.
        matriz[i] = (int *)malloc(tamanho * sizeof(int));
        
        if (matriz[i] == NULL) {
            perror("Falha ao alocar memória para o vetor individual");
            
            // Libera a memória dos vetores que já foram alocados antes de falhar
            for (int k = 0; k < i; k++) free(matriz[k]);
            free(matriz);
            
            exit(EXIT_FAILURE);
        }

        // 3. PREENCHIMENTO DO VETOR com números aleatórios
        for (int j = 0; j < tamanho; j++) {
            matriz[i][j] = rand(); 
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

void *t_function(void *arg){

    int id_thread = *(int *)arg;

    for( int i = id_thread; i < NUM_VETORES; i = i + NUM_THREADS) {
    quicksort(matriz[i], 0, TAMANHO_VETOR-1);
    }
    pthread_exit(NULL);
}



int main(){
    
    struct timeval t1, t2;
    pthread_t t[NUM_THREADS];
    int tid[NUM_THREADS];

    // Inicializa a semente do gerador de números aleatórios
    srand(time(NULL));

    gerar_vetores(NUM_VETORES, TAMANHO_VETOR);

    gettimeofday(&t1, NULL);
    //Criação das Threds com chamada de função
    for (int i = 0 ; i < NUM_THREADS; i++){
        tid[i] = i;
        pthread_create(&t[i], NULL, t_function, &tid[i]); 
    }
    
    for( int i=0; i < NUM_THREADS; i++){
        pthread_join(t[i],NULL);
    }
    
    
    gettimeofday(&t2, NULL);

    double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec -
    t1.tv_usec)/1000000.0);
    printf("tempo total = %f\n", t_total);
}




