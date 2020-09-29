#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

static int size, threads;
static int *a, *b;

static void* rankSort(void* arg)
{
  int i, j, cnt, val;
  long my_rank = (long)arg;
  int start = my_rank * size / threads;
  int end = (my_rank + 1) * size / threads;

  /* sort the values */
  for (i = start; i < end; i++) {
    
    cnt = 0;
	  val = a[i];
	  
    for (j = 0; j < size; j++) {
  	  if (val > a[j]) cnt++;
	  }
	  
    b[cnt] = val;
  }

  return NULL;
}

int main(int argc, char *argv[])
{
  int i;
  long counter;
  struct timeval start, end;

  printf("RankSort v1.1 [pthreads outer]\n");

  /* check command line */
  if (argc != 3) {
    fprintf(stderr, "usage: %s number_of_elements number_of_threads\n", argv[0]);
    exit(-1);
  }

  size = atoi(argv[1]);

  if (size < 1) {
    fprintf(stderr, "number of elements must be at least 1\n");
    exit(-1);
  }

  threads = atoi(argv[2]);

  if (threads < 1) {
    fprintf(stderr, "number of threads must be at least 1\n");
     exit(-1);
  }

  /* allocate arrays */
  a = (int *)malloc(size * sizeof(int));
  b = (int *)malloc(size * sizeof(int));
  
  if ((a == NULL) || (b == NULL)) {
    fprintf(stderr, "could not allocate arrays\n");
    exit(-1);
  }

  /* create data structure for holding thread information */
  pthread_t* threadHandles = (pthread_t*)malloc(threads * sizeof(pthread_t));
  
  if (threadHandles == NULL) {
    fprintf(stderr, "could not allocate array\n");
    exit(-1);
  }

  /* generate input */
  for (i = 0; i < size; i++) {
    a[i] = -((i & 2) - 1) * i;
  }

  printf("sorting %d values with %d threads\n", size, threads);

  /* start time */
  gettimeofday(&start, NULL);

  /* launch the worker threads */
  for (counter = 1; counter < threads; counter++) {
    pthread_create(&threadHandles[counter], NULL, rankSort, (void*)counter);
  }

  /* have the master thread do some of the work */
  rankSort(0);

  /* wait for the threads to finish */
  for (counter = 1; counter < threads; counter++) {
	  pthread_join(threadHandles[counter], NULL);
  }

  /* end time */
  gettimeofday(&end, NULL);
  printf("runtime: %.4f s\n", end.tv_sec + end.tv_usec / 1000000.0 - start.tv_sec - start.tv_usec / 1000000.0);

  /* verify result */
  i = 1;
  
  while ((i < size) && (b[i - 1] < b[i])) {
    i++;
  }

  if (i < size) {
    printf("NOT sorted\n\n"); 
  } else {
    printf("sorted\n\n");
  } 

  free(a);
  free(b);
  free(threadHandles);
  
  return 0;
}

