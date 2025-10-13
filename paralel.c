#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "paralel.h"

// Thread mestre será a main do programa -> produtora
// A função executar(task) será a ação de produzir
// A fila de tarefas é o buffer do padrão
// a pool de threads será quem consumira as tarefas do buffer -> consumidora
// Será controlado o acesso por mutex e semafaros

//Tamanho da pool de threads
#define T_POOL 5

//Tipo de dado Tarefa(task) que tem como atributos
//Um ponteiro para uma função
//Um argumento para a função -> o que será executado
typedef struct
{
    void (*function)(void *arg);
    void *argument;
} Tarefa;

//Criando o tipo para a fila de tarefas (buffer)
typedef struct FilaTarefas
{
    Tarefa *tarefas; // ponteiro para lista de tarefas
    size_t inicio, fim, tamanho_max;

    //mutex para proteger seção critica
    pthread_mutex_t mutex;
    //Semaforos para "contar" tarefas
    sem_t ocupado;
    sem_t livre;
} FilaTarefas;

//Declarando o conteudo do tipo ThreadPool(.h)
struct ThreadPool
{
    pthread_t *threads; //ponteiro para array de threads
    int desligar;
    size_t numero_threads;
    FilaTarefas *fila; //ponteiro para lista de tarefas
};


//Threads que irá exucutar a lista de atividas -> consumidora
static void *thread_worker(void *arg){

    //Conversão do argumento para o tipo ThreadPool
    ThreadPool *pool = (ThreadPool *)arg; // ponteiro pool do tipo ThreadPool que recebe do argumento da funcao uma ThreadPool
    //criar um ponteiro para fila, argumento dentro da pool(facilitar o acesso a fila)
    FilaTarefas *fila = pool->fila; 
    
    while(1){
        //Espera até que tenha algo na fila, usando semafaro
        sem_wait(&fila->ocupado); //wait no semafaro ocupado, que é atributo da fila
        
        //Após passar o semafaro, trava o mutex da fila (secão critica)
        pthread_mutex_lock(&fila->mutex);


        //caso a pool seja sinalizada para desligar
        if (pool -> desligar && fila->inicio == fila->fim){
            pthread_mutex_unlock(&fila->mutex);
            sem_post(&fila->ocupado);
            break;
        };

        //dentro da fila, pegando a tarefa do inicio da lista e guardando o retorno
        Tarefa tarefa = fila->tarefas[fila->inicio];
        fila -> inicio = (fila->inicio + 1) % fila->tamanho_max;

        pthread_mutex_unlock(&fila->mutex);

        //sinaliza que um espaço na fila foi liberado
        sem_post(&fila->livre);

        // Executa a tarefa (fora da seção crítica)
        (tarefa.function)(tarefa.argument);
    };
    return NULL;
};


//Logica para criar uma pool de threads baseando em produtor/consumidor. 
ThreadPool *pool_init(size_t num_threads, size_t queue_size){

    if (num_threads == 0 || queue_size == 0) return NULL;

    //alocando pool na memoria e guardando em um ponteiro
    ThreadPool *pool = (ThreadPool*)malloc(sizeof(ThreadPool));

    //inicializando e alocando na memoria atributos da pool e fila
    pool->desligar = 0;
    pool->numero_threads = num_threads; //define n de threads
    pool->threads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
    pool->fila = (FilaTarefas*)malloc(sizeof(FilaTarefas));
    pool->fila->tarefas = (Tarefa*)malloc(sizeof(Tarefa) * queue_size);
    pool->fila->tamanho_max = queue_size; //!!queue_size definido no arg (quero estabelecer uma padrao depois)
    pool->fila->inicio = 0;
    pool->fila->fim = 0;
    pthread_mutex_init(&pool->fila->mutex, NULL);
    sem_init(&pool->fila->ocupado, 0, 0);
    sem_init(&pool->fila->livre, 0, queue_size);


    for (size_t i = 0; i < num_threads; i++) {
        pthread_create(&pool->threads[i], NULL, thread_worker, pool);
    }

    return pool;
};


void execute(ThreadPool* pool, void (*funcao)(void *), void *argumento) {
    if (pool == NULL || pool->desligar) return;

    Tarefa tarefa;
    tarefa.function = funcao;
    tarefa.argument = argumento;

    FilaTarefas* fila = pool->fila;
    //espera espaço na fila
    sem_wait(&fila->livre);

    pthread_mutex_lock(&fila->mutex);
    fila->tarefas[fila->fim] = tarefa;
    fila->fim = (fila->fim + 1) % fila->tamanho_max;
    pthread_mutex_unlock(&fila->mutex);

    //sinaliza que adicionou uma tarefa na lista
    sem_post(&fila->ocupado);
};

void pool_destroy(ThreadPool* pool) {

    //sleep(5);
    pool->desligar = 1;

    //acorda as threads para serem destruidas
    for (size_t i = 0; i < pool->numero_threads; i++) {
        sem_post(&pool->fila->ocupado);
    }
    for (size_t i = 0; i < pool->numero_threads; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    //liberando memoria
    free(pool->fila->tarefas);
    sem_destroy(&pool->fila->ocupado);
    sem_destroy(&pool->fila->livre);
    pthread_mutex_destroy(&pool->fila->mutex);
    free(pool->fila);
    free(pool->threads);
    free(pool);
};