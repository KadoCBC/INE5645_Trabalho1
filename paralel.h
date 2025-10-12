#ifndef PARALEL_H
    #define PARALEL_H

    #include <stddef.h>
    //uso do size_t - tipo de dados

    //tipo de dados threadpool
    typedef struct ThreadPool ThreadPool;

    //funcao para iniciar uma pool -> retorna seu ponteiro
    ThreadPool* pool_init(size_t num_threads, size_t queue_size);

    // funcao para executar um tarefa
    //precisa do ponteiro da pool, e a função+arg
    void execute(ThreadPool* pool, void (*funcao)(void *), void *argumento);

    //para destruir a pool
    //precisa do ponteiro da pool
    void pool_destroy(ThreadPool* pool);
    
#endif