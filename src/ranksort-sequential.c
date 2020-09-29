#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define N 100000

int main(int argc, char *argv[])
{
  double start, stop;
  int readArray[N], resultArray[N];
  int currentItem, currentPosition;
  
  start = omp_get_wtime();
  
  for (int i = 0; i < N; i++) {
    readArray[i] = N - i;
  }

  for (int j = 0; j < N; j++) {
    currentItem = readArray[j];
    currentPosition = 0;
    for (int i = 0; i < N; i++) {
      if (currentItem > readArray[i]) {
        currentPosition++;
      }

      // Handles equal numbers
      if ((currentItem == readArray[i]) && (j < i)) {
        currentPosition++;
      }
    }
    resultArray[currentPosition] = currentItem;
  }

  for (int i = 0; i < N; i++) {
    printf("%d\n", resultArray[i]);
  }

  stop = omp_get_wtime();

  printf("time %f\n", stop-start);

  return 0;
}

