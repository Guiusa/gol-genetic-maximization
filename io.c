#include "io.h"
#include <stdio.h>

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
