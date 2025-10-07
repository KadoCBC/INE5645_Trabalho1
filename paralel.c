#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "paralel.h"


int pollt = 4;
pthread_t threads[pollt];
int threads_id[pollt];


sem_t employer;
sem_t master;
pthread_mutex_t mutex;

//protoripo de uma fila de tasks com tamanho igual numero de threads
task fila[pollt];
int contador_fila;

//função para tirar da fila (deve ser um struct depois provavelmente)
void dequeue(int n_task){
    pthread_mutex_lock(&mutex);
    fila[n] = NULL;  //duvida de como usar o mutex nesse caso
    --contador_fila;
    pthread_mutex_unlock(&mutex);
    
};
//função para colocar na fila (deve ser um struct depois provavelmente)
void queue(task *n){
    if (contador_fila >= 4){
        wait(&master); 
    };
    pthread_mutex_lock(&mutex);
    fila[contador_fila + 1 ] = n;
    ++contador_fila
    pthread_mutex_unlock(&mutex);
};

//Thread que irá fazer a chamada das atividades para pool -> produtora
void *thread_master(void *args){


};

//Threads que irá exucutar a lista de atividas -> consumidora
void *thread_employers(void *args){

};


//Logica para criar uma pool de threads baseando em produtor/consumidor. 
int createPool(pollt){

    int i = 0;

    pthread_create(&threds[i], NULL, thread_master, &threads_id[i]);
    
    
    for (i = 1, i < pollt, i++){
        threads_id[i] = i;
        pthread_create(&threds[i], NULL, thread_employers, &threads_id[i]);
    }; 
};


void execute(){

    return
};