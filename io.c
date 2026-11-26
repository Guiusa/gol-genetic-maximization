#include "io.h"
#include <stdio.h>
#include <string.h>

#define BOX "██"

/*
  Prints the grid with visual characters
*/
void printGrid(short ROWS, short COLUMNS, short **grid){
  for(int i = 0; i<ROWS; i++) { 
    for(int j = 0; j<COLUMNS; j++) {
      printf("%s",
        (grid[i][j] & 1) ? 
          BOX : 
          "  "
      ) ;
    }
    printf("\n") ;
  }
}

/*
  Prints the grid as 0s and 1s
*/
void printNumbGrid(short ROWS, short COLUMNS, short **grid){
  for(int i = 0; i<ROWS; i++) { 
    for(int j = 0; j<COLUMNS; j++) {
      printf("%d",
        (grid[i][j] & 1) ? 
          1 : 
          0
      ) ;
    }
    printf("\n") ;
  }
}

void writeLogHeader(short ROWS, short COLUMNS, short goliter) {
  char path[100] ;
  strcpy(path, LOGDIR) ;
  strcat(path, "banana") ;
  FILE *ptr = fopen(path, "w") ;
  fprintf(ptr, "%d %d %d\n", ROWS, COLUMNS, goliter) ;
  fclose(ptr) ;
}

void logBest(short ROWS, short COLUMNS, short **grid) {
  char path[100] ;
  strcpy(path, LOGDIR) ;
  strcat(path, "banana") ;
  FILE *ptr = fopen(path, "a") ;
  for (int i = 0; i<ROWS; i++)
    for(int j = 0; j<COLUMNS; j++)
      fprintf(ptr, " %d", grid[i][j]) ;
  fprintf(ptr, "\n") ;
  fclose(ptr) ;
}

void writeGenDataLogHeader(int rounds) {
  char path[100] ;
  strcpy(path, LOGDIR) ;
  strcat(path, GRAPHLOG) ;
  FILE *ptr = fopen(path, "w") ;

  fprintf(ptr, "%d\n", rounds) ;
  fclose(ptr) ;
}

void logGenData(int best, int worst, int mean){
  char path[100] ;
  strcpy(path, LOGDIR) ;
  strcat(path, GRAPHLOG) ;
  FILE *ptr = fopen(path, "a") ;

  fprintf(ptr, "%d %d %d\n", best, worst, mean) ;
  fclose(ptr) ;
}
