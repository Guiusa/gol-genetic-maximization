#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/param.h>
#include "io.h"
#include "struct.h"
#include "life.h"

#define true 1
#define CPYFACTOR 0.3

int cmpResults(const void *a, const void *b) {
  results_t *ra = (results_t *) a ;
  results_t *rb = (results_t *) b ;

  return ra->alive < rb->alive ;
}

int main() {
  srand(time(NULL)) ;

  short ROWS = 50 ;
  short COLUMNS = 50 ;
  int indNum = 10 ;
  int genNum = 10 ;
  int rounds = 10 ;
  scanf("%hd %hd %d %d %d",
    &ROWS,
    &COLUMNS,
    &indNum,
    &genNum,
    &rounds) ;
  writeLogHeader(ROWS, COLUMNS, genNum) ;
  // Allocates the structures
  short ***indStart = (short ***) malloc(indNum * sizeof(short **)) ;
  short ***indExec = (short ***) malloc(indNum * sizeof(short **)) ;
  short ***indSwap = (short ***) malloc(indNum * sizeof(short **)) ;
  for(int i = 0; i<indNum; i++){
    indStart[i] = mallocGrid(ROWS, COLUMNS) ;
    indExec[i] = mallocGrid(ROWS, COLUMNS) ;
    indSwap[i] = mallocGrid(ROWS, COLUMNS) ;
  }
  results_t *finalAlives = (results_t *) calloc(indNum, sizeof(results_t)) ;
  
  int mostAlive = 0 ;
  // Main training loop
  for(int r = 0; r<rounds; r++) {
    //If first generation, randomize individuals
    if(r==0){
      for(int i = 0; i<indNum; i++) {
        randGen(ROWS, COLUMNS, indStart[i]) ;
        copyGrid(ROWS, COLUMNS, indExec[i], indStart[i]) ;
        copyGrid(ROWS, COLUMNS, indSwap[i], indStart[i]) ;
      }
    } //Else, keeps best individuals and reproduce them 
    else {
      int bestIdx = MAX(CPYFACTOR * indNum, 2) ;
      for(int bi = 0; bi<bestIdx; bi++)
        copyGrid(ROWS, COLUMNS, indSwap[bi], indStart[finalAlives[bi].idx]) ;
      while(true) {
        if(bestIdx == indNum) break ;
        for(int i1 = 0; i1 < indNum; i1++){
          for(int i2 = i1+1; i2<indNum; i2++){
            reproduceGrids(ROWS, COLUMNS, 
              indStart[finalAlives[i1].idx],
              indStart[finalAlives[i2].idx],
              indSwap[bestIdx++]
            ) ;
            mutateGrid(ROWS, COLUMNS, 0.001, indSwap[bestIdx-1]) ;
            if (bestIdx == indNum) i1 = i2 = indNum ;
          }
        }
      }
      short ***aux = indStart ;
      indStart = indSwap ;
      indSwap = aux ;
      for(int iaux = 0; iaux<indNum; iaux++)
        copyGrid(ROWS, COLUMNS, indExec[iaux], indStart[iaux]) ;
      }

    for(int i = 0; i<indNum; i++){
      for (int g = 0; g<genNum; g++){
        finalAlives[i].alive = newGen(ROWS, COLUMNS, indExec[i]) ;
        finalAlives[i].idx = i ;
      }
      qsort(finalAlives, indNum, sizeof(results_t), cmpResults) ;
      if(finalAlives[0].alive > mostAlive)
        mostAlive = finalAlives[0].alive ;
    }

    printf("Generation %d results:\n", r) ;
    int summAlv = 0 ;
    for(int i = 0; i<indNum; i++){
      printf("Individual %d has %d alive cells\n", 
        finalAlives[i].idx,
        finalAlives[i].alive
      ) ;
      summAlv += finalAlives[i].alive ;
    }
    logBest(ROWS, COLUMNS, indStart[finalAlives[0].idx]) ;
    printf("Mean of total alives: %d\n", (int) summAlv/indNum) ;
    printf("##########################################\n\n") ;
  }
  printf("Most cells alive was %d\n", mostAlive) ;

  for(int i = 0; i<indNum; i++) {
    freeGrid(ROWS, indStart[i]) ;
    freeGrid(ROWS, indExec[i]) ;
    freeGrid(ROWS, indSwap[i]) ;
  }
  free(indSwap) ;
  free(indStart) ;
  free(indExec) ;
  free(finalAlives) ;
  return 0 ;
}
