#ifndef PARALEL_H
    #define PARALEL_H


    //Tipo de dado Tarefa(task) que tem como atributos
    //Um ponteiro para uma função
    //Um argumento para a função -> o que será executado
    typedef struct
    {
        void (*function)(void *arg);
        void *argument;
    } Tarefa;


    // Definindo o tipo de dado ThreadPool (vazio)
    typedef struct ThreadPool ThreadPool;

    // Funções públicas da biblioteca
    ThreadPool* pool_criar(int numero_threads);
    void pool_submeter_tarefa(ThreadPool *pool, Tarefa tarefa);
    void pool_destruir(ThreadPool *pool);
    
    



#endif