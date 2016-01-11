/***************************************************
 * Single Author Info
 * ssingh24 Siddharth Singh
***************************************************/

//Note: Code reused from assignment 1

#include <pthread.h>
#include <signal.h>

int busy_lock;

typedef struct mythread_barrierattr{
        char value;
} mythread_barrierattr_t;

typedef struct mythread_barrier {
    pthread_mutex_t lock;
    pthread_mutex_t lock_sched;
    struct sigaction sa;
    int total_count;
    int current_count;
    pthread_t *threads;
    mythread_barrierattr_t* attr;
} mythread_barrier_t;

/*Sid: init will initialize the condition value to number of threads to wait for as provided by count. Set the barrier_q to NULL
 */
int mythread_barrier_init(mythread_barrier_t *barrier, const mythread_barrierattr_t *attr, unsigned count);

/*Sid: Will suspend the thread to wait for all the threads to finish. It will check if the value has been set to number of threads and then resume function.
 */
int mythread_barrier_wait(mythread_barrier_t *barrier);

/*Sid: Releases the memory allocated to barrier
 */
int mythread_barrier_destroy(mythread_barrier_t *barrier);

