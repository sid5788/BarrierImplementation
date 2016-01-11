/***************************************************
 * * Single Author Info
 * * ssingh24 Siddharth Singh
 * ***************************************************/

#include <stdio.h>
#include "pthread.h"
#include "./mybarrier.h"

#define N 4

//Global Value to be manipulated
int count = 0;
int count1 = 0;
int first_test=0;
int second_test=0;

mythread_barrier_t barrier;

void *barrierTest() {

    count++;
    mythread_barrier_wait(&barrier);
    if ( count == N ) {
          first_test++;
    }
    
    count1 ++;
    mythread_barrier_wait(&barrier);
    if ( count1 == N ) {
          second_test++;
    }

}

int main()
{
  pthread_t thread[N];
  //Sid: Test1: Test with first barrier
  mythread_barrier_init(&barrier,NULL,N);
  int i = 0;
  for ( i = 0; i <N; i++) {
     pthread_create(&thread[i], NULL, &barrierTest, NULL);

  }
  for ( i = 0; i<N; i++) {
     pthread_join(thread[i], NULL);
  }
  if ( first_test == N && second_test == N) {
          printf("PASS \n");
       }else{
          printf("FAIL \n");
       }

  mythread_barrier_destroy(&barrier);
  return 1;
}
