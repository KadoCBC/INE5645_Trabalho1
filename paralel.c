#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "paralel.h"

// Thread mestre será a main do programa -> produtora
// A função executar(task) será a ação de produzir
// A fila de tarefas é o buffer do padrão
// a pool de threads será quem consumira as tarefas do buffer -> consumidora
// Será controlado o acesso por mutex e variaveis de condição

//Tamanho da pool de threads
#define T_POOL 5

//Criando o tipo para a fila de tarefas (buffer)
typedef struct 
{
    Tarefa lista_t[T_POOL];
    int inicio;
    int fim;

    //mutex para proteger seção critica
    pthread_mutex_t mutex;
    //Semaforos para "contar" tarefas
    sem_t espaco;
    sem_t ocupado;
} FilaTarefas;

//Declarando o conteudo do tipo ThreadPool(.h)
struct ThreadPool
{
    pthread_t *threads;
    int id_threads;
    FilaTarefas *fila;
};


//Threads que irá exucutar a lista de atividas -> consumidora
void *thread_worker(void *args){

    //Conversão do argumento para o tipo ThreadPool
    ThreadPool *pool = (ThreadPool *)arg; // ponteiro pool do tipo ThreadPool que recebe do argumento da funcao uma ThreadPool
    //criar um ponteiro para fila, argumento dentro da pool(facilitar o acesso a fila)
    FilaTarefas *fila = pool->fila; 
    
    While(1){
        //Espera até que tenha algo na fila, usando semafaro
        sem_wait(&fila->ocupado); //wait no semafaro ocupado, que é atributo da fila
        
        //Após passar o semafaro, trava o mutex da fila (secão critica)
        pthread_mutex_lock(&fila->mutex);

        //dentro da fila, pegando a tarefa do inicio da lista e guardando o retorno
        Tarefa tarefa = fila->lista_t[fila->inicio];
        fila -> inicio = (fila->inicio + 1) % T_POOL;

        pthread_mutex_unlock(&fila->mutex);

        //sinaliza que um espaço na fila foi liberado
        sem_post(&fila->espaco);

        // Executa a tarefa (fora da seção crítica)
        (tarefa.funcao)(tarefa.argumento);
    };
    return NULL;
};


//Logica para criar uma pool de threads baseando em produtor/consumidor. 
int createPool(){

    int i = 0;

    pthread_create(&threds[i], NULL, thread_mestre, &threads_id[i]);
    
    
    for (i = 1, i < T_POOL, i++){
        threads_id[i] = i;
        pthread_create(&threds[i], NULL, thread_workers, &threads_id[i]);
    };

    for(i = 1; i < T_POOL; i++){
        pthread_join(threads[i],NULL);
    };

    pthread_cancel(threads[0]); // thread mestre
};

//codigo incompleto, falta entender como realizar o escopo
int execute() {

    sem_init(&ocupado, 0, 0); // inicia semafaro "fechado"
    sem_init(&espaco, 0, T_POOL); //inicia semafaro a partir do tamanho da pool
    pthread_mutex_init(&mutex, NULL);

    //cria a pool
    createPool();

    //destroi os semafaros
    sem_destroy(&ocupado);
    sem_destroy(&espaco);
    pthread_mutex_destroy(&mutex);

};