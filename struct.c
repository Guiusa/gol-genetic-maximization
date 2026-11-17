#include "struct.h"
#include <stdlib.h>

/*
  Allocates memory for a grid
  a grid is a matrix of ROWS X COLUMNS short values
*/
short **mallocGrid(short ROWS, short COLUMNS) {
  short **grid = (short **) malloc(ROWS * sizeof(short *)) ;
    for(int i = 0; i<ROWS; i++) 
      grid[i] = (short *) malloc(COLUMNS * sizeof(short)) ;
  return grid ;
}


/*
  Frees memory of a grid
*/
void freeGrid(short ROWS, short **grid) {
  for(int i = 0; i<ROWS; i++) 
    free(grid[i]) ;
  free(grid) ;
}


/*
  Copies the values of src grid into the new grid
*/
void copyGrid(short ROWS, short COLUMNS, short **grid, short **src) {
  for(int i = 0; i<ROWS; i++)
    for(int j = 0; j<COLUMNS; j++)
      grid[i][j] = src[i][j] ;
}
