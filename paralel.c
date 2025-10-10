#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "paralel.h"

//LOGICA ATÉ O MOMENTO -> CRIAÇÃO DO CODIGO PRODUTOR CONSUMIDOR
// E LOGICA DE POOL DE THREADS, FALTA AINDA ENTENDER COMO TRASNFORMAR ISSO
// EM UMA BIBLIOTECA QUE TEM UMA FUNÇÃO PARA ENVIAR AS TAREFAS 


//Tamanho da pool de threads
#define T_POOL 5
//lista de threads
pthread_t threads[T_POOL];
int threads_id[T_POOL];

// Lista = buffer do padrao produtor consumidor 
int fila[T_POOL];
int contador_fila = 0;


// inicialização semafaro para padrão produtor consumidor (Como só tem um produtor precisa fazer um semafaro para produzir ?)
sem_t espaco;
sem_t ocupado;
//incializacao do mutex para add no buffer
pthread_mutex_t mutex;

//Thread que irá fazer a chamada das atividades para pool -> produtora
void *thread_mestre(void *args){

    //talvez tenha um While aqui()
    int task = 0;//temporario irá receber dos args
    
    // espera por um sinal de espaço 
    sem_wait(&espaco);

    // bloqueia o mutex para buffer (seção critica)
    pthread_mutex_lock(&mutex);

    //coloca task na fil de tarefas (buffer)
    fila[contador_fila] = task;
    printf("tarefa adicionada: %d\n", task)
    contador_fila = contador_fila + 1;

    // libera mutex
    pthread_mutex_unlock(&mutex);

    // manda sinal de nova task
    sem_post(&ocupado);

};

//Threads que irá exucutar a lista de atividas -> consumidora
void *thread_workers(void *args){
    
    //espera sinal de nova taks
    sem_wait(&ocupado);

    //bloqueia o mutex 
    pthread_mutux_lock(&mutex);

    //tira task na fil de tarefas (buffer)
    int task = fila[contador_fila];
    //executa a task (fazer codigo)
    printf("tarefa executada: %d\n", task)
    contador_fila = contador_fila - 1;

    // libera mutex
    pthread_mutex_unlock(&mutex);

    // sinaliza novo espaco no buffer
    sem_post(&espaco);
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