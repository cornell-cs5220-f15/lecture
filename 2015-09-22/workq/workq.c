/* workq.c --
 *
 *   Driver syntax: ./workq [nthreads]
 *   Defaults to nthreads = 1
 *
 * Example of a multi-threaded work queue.  This implementation does
 * not have proper synchronization -- your job is to add it in!  Search
 * for the TODO comments below.
 *
 * We do provide some convenience functions to wrap the pthread library
 * calls, if you'd like to use them.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


/************************* I/O ******************************/

/*
 * Lock on standard output
 */
pthread_mutex_t io_lock;


/*
 * Protected printf
 */
void lprintf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    pthread_mutex_lock(&io_lock);
    vprintf(format, args);
    pthread_mutex_unlock(&io_lock);
    va_end(args);
}


/************************* Work queue ******************************/

/* 
 * Each task consists of a pointer to user-managed data and a next
 * task.  If this was a C++ program, I would use a template here to
 * allow different data types.
 */
typedef struct task_t {
    void* data;
    struct task_t* next;
} task_t;


/*
 * The work queue consists not only of the head of a linked lisk of tasks,
 * but also a lock and condition variable for synchronizing the interaction
 * of producers (who put items on the queue) and consumers (who pull items).
 * 
 * For the moment, we assume that there is some logic in place to decide
 * when no more work will be produced, and that some entity can run this
 * logic and set the "done" flag.  In very dynamic situations, deciding
 * when no more tasks will be produced is itself not trivial.
 */
typedef struct workq_t {
    pthread_mutex_t lock;  /* Mutex for queue invariants */
    pthread_cond_t cv;     /* Signal for work ready / finalize */
    int done;              /* Flag that work queue is closed up */
    task_t* tasks;         /* Linked list of tasks */
} workq_t;


/*
 * Set up the work queue
 */
void workq_init(workq_t* workq)
{
    pthread_mutex_init(&(workq->lock), NULL);
    pthread_cond_init(&(workq->cv), NULL);
    workq->done = 0;
    workq->tasks = NULL;
}


/*
 * Destroy the work queue
 * NB: This should be done in a serial section when all threads are joined!
 */
void workq_destroy(workq_t* workq)
{
    pthread_cond_destroy(&(workq->cv));
    pthread_mutex_destroy(&(workq->lock));
}


/*
 * Lock the work queue
 */
void workq_lock(workq_t* workq)
{
    pthread_mutex_lock(&(workq->lock));
}


/*
 * Unlock the work queue
 */
void workq_unlock(workq_t* workq)
{
    pthread_mutex_unlock(&(workq->lock));
}


/*
 * Signal work available or all work done.
 * Wakes at most one waiting thread.
 */
void workq_signal(workq_t* workq)
{
    pthread_cond_signal(&(workq->cv));
}


/*
 * Signal work available or all work done.
 * Wakes all waiting threads
 */
void workq_broadcast(workq_t* workq)
{
    pthread_cond_broadcast(&(workq->cv));
}


/*
 * Wait for work available or all work done
 * Precondition: The queue should be locked before calling workq_wait.
 */
void workq_wait(workq_t* workq)
{
    while (workq->tasks == NULL && workq->done == 0)
        pthread_cond_wait(&(workq->cv), &(workq->lock));
}


/*
 * Add work to the queue
 * TODO: This needs synchronization!
 */
void workq_put(workq_t* workq, void* data)
{
    task_t* task = (task_t*) malloc(sizeof(task_t));
    task->data = data;
    task->next = workq->tasks;
    workq->tasks = task;
}


/*
 * Get a data item from the queue.  We assume NULL data can be used
 * to signal that the queue is empty.
 * TODO: This needs synchronization!
 */
void* workq_get(workq_t* workq)
{
    void* result = NULL;
    if (workq->tasks) {
        task_t* task = workq->tasks;
        result = task->data;
        workq->tasks = task->next;
        free(task);
    }
    return result;
}


/*
 * Signal that no more work will be added to the queue.
 * NB: This function can be called when there are still tasks to process!
 *     We're just saying that we're done adding new tasks.
 * TODO: This needs synchronization!
 */
void workq_finish(workq_t* workq)
{
    workq->done = 1;
}


/********************** Demo producer/consumer ***************************/


/*
 * Producer main routine.  We assume that there is only one producer
 * in this example -- otherwise we couldn't just call workq_finish.
 */
void producer_main(workq_t* workq, int nitems)
{
    int i;
    for (i = 0; i < nitems; ++i) {
        char buf[256];
        sprintf(buf, "Work item %d", i);
        workq_put(workq, strdup(buf));
    }
    workq_finish(workq);
}


/*
 * Consumer input data type
 */
typedef struct consumer_t {
    int id;            /* Unique identifier for consumer */
    workq_t* workq;    /* Work queue object              */
} consumer_t;


/*
 * Consumer thread main.
 */
void* consumer_main(void* arg)
{
    consumer_t* consumer = (consumer_t*) arg;
    workq_t* workq = consumer->workq;
    for (char* t = (char*) workq_get(workq);
         t != NULL;
         t = (char*) workq_get(workq)) {
        lprintf("Process %d: %s\n", consumer->id, t);
        free(t);
    }
    return NULL;
}


int main(int argc, char** argv)
{
    int i, nworkers = 1;
    pthread_t threads[32];
    consumer_t consumers[32];
    workq_t workq;

    /* Get number of workers from command line */
    if (argc > 1)
        nworkers = atoi(argv[1]);
    if (nworkers > 32 || nworkers < 1) {
        fprintf(stderr, "Error: Must have between 1 and 32 workers\n");
        return -1;
    }

    /* Initialize I/O mutex and work queue */
    pthread_mutex_init(&io_lock, NULL);
    workq_init(&workq);
    
    /* Launch worker threads */
    for (i = 0; i < nworkers; ++i) {
        consumers[i].id = i;
        consumers[i].workq = &workq;
        pthread_create(&threads[i], NULL, consumer_main, &consumers[i]);
        lprintf("Create worker %d\n", i);
    }
    
    /* Run producer */
    producer_main(&workq, 100);
    
    /* Join on worker threads */
    for (i = 0; i < nworkers; ++i) {
        lprintf("Join worker %d\n", i);
        pthread_join(threads[i], NULL);
    }
    
    /* Free I/O mutex and work queue */
    workq_destroy(&workq);
    pthread_mutex_destroy(&io_lock);

    return 0;
}
