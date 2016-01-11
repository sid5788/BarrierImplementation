/***************************************************
* Single Author Info
* ssingh24 Siddharth Singh
***************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./mybarrier.h"

#define SUCCESS 1
#define FAILURE 0

//Note:Code Reused from Homework2

void err_sys(const char* x) 
{ 
    perror(x); 
    exit(1); 
}

static void handler(int signum)
{
    /* Take appropriate actions for signal delivery */
    busy_lock=1;
}

int mythread_barrier_init(mythread_barrier_t *barrier, const mythread_barrierattr_t *attr, unsigned count) {
    int count1 = (unsigned int)count;
    /*Sid:Initialize the barrier count with the number of threads to wait on
     */
    if (barrier != NULL && count1 > 0) {
       pthread_mutex_init(&barrier->lock, NULL);
       pthread_mutex_init(&barrier->lock_sched, NULL);
       busy_lock=0;
       //Sid: Initialize the signals to wait for in sigaction
       memset(&barrier->sa, 0, sizeof(barrier->sa));
       barrier->sa.sa_handler = handler;
       sigemptyset(&barrier->sa.sa_mask);
       if (sigaction(SIGUSR1, &barrier->sa, NULL) < 0) {
          err_sys("signal handler not installed");
       }

       barrier->total_count = count1;
       barrier->current_count = 0;
       barrier->threads =(pthread_t*) malloc (count1*(sizeof(pthread_t)));
       int i = 0;
       for (i=0;i<count1;i++) {
           barrier->threads[i]=0;
       }
       return SUCCESS;
    }

    return FAILURE;
}

int mythread_barrier_wait(mythread_barrier_t *barrier) {
    if (barrier != NULL) {
       /*Sid: Increase barrier value by 1 indicating this thread has reached the barrier location.
        *Put these transactions in mutex lock so that the wrong value of count is not used
        */
       int timer=0;
       pthread_mutex_lock(&barrier->lock);
       if (barrier->current_count == 0){
          pthread_mutex_lock(&barrier->lock_sched);
       }
          busy_lock = 0;
       //Sid:Get TID of this thread and put it in array
       if (barrier->current_count < barrier->total_count) {
           barrier->threads[barrier->current_count] = pthread_self();
       }
       barrier->current_count+=1;

       //Sid: Wait_Loop into this thread
       if (barrier->current_count < barrier->total_count) {
           //Sid:Unlock the critical section, and wait loop
           pthread_mutex_unlock(&barrier->lock);
           while(busy_lock == 0);
        /*   while((pthread_mutex_trylock(&barrier->lock_sched))!=0) {
              if (busy_lock == 1) {
                 pthread_mutex_unlock(&barrier->lock_sched);
              }
           }*/
           
           //Sid:Reacquire the lock after leaving the wait loop
           pthread_mutex_lock(&barrier->lock);
       }
       if (barrier->current_count == barrier->total_count){
           //Sid: Unblock all threads
           int i = 0;
           for (i=0;i<barrier->total_count-1;i++) {
               pthread_kill(barrier->threads[i],SIGUSR1);
           }
       }

       barrier->current_count--;
       pthread_mutex_unlock(&barrier->lock);
       /*Note:Below is required to allow signals to be processed for each thread before entering the next barrier. 
        * Different machines need different timer.. for example remote.eos required much larger time compared to awXX machines
        */
       
       while(timer<9999999) {
          timer++;
       }
       timer=0;
       if (barrier->current_count == 0){
           pthread_mutex_unlock(&barrier->lock_sched);
       }
       //sleep(1);     
       return SUCCESS;
    }
    return FAILURE;
}

int mythread_barrier_destroy(mythread_barrier_t *barrier) {
    if (barrier != NULL) {
       busy_lock = 0;
       pthread_mutex_destroy(&barrier->lock);
       pthread_mutex_destroy(&barrier->lock_sched);
       memset(&barrier->sa, 0, sizeof(barrier->sa));
       int i = 0;
       for (i=0;i<barrier->total_count;i++) {
           barrier->threads[i]=0;
       }

       barrier->total_count = 0;
       barrier->current_count = 0;
       return SUCCESS;
    }

    return FAILURE;
}

