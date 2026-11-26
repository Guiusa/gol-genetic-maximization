#include "life.h"
#include <stdlib.h>

/*
  Generates a randomized grid (half 0s, half 1s)
*/
int randGen(short ROWS, short COLUMNS, short **grid) {
  int alive = 0 ;
  for(int i = 1; i<ROWS-1; i++){
    for(int j = 1; j<COLUMNS-1; j++){
      if (rand()%2) {
        grid[i][j] = 1 ;
        alive ++ ;
      } else grid[i][j] = 0 ;
    } 
  }

  for(int i = 0; i<ROWS; i++)
    grid[i][0] = grid[i][COLUMNS-1] = 0 ;
  for(int j = 0; j<COLUMNS; j++)
    grid[0][j] = grid[ROWS-1][j] = 0 ;

  return alive ;
}

/*
  Executes the rules of game of life and generates the new
  generation
*/
int newGen(short ROWS, short COLUMNS, short **grid) {
  int alive = 0 ;
  for(int i = 1; i<ROWS-1; i++){
    for(int j = 1; j<COLUMNS-1; j++){
      int neighborSumm = 0 ;
      for(int di = -1; di<= 1; di++){
        for(int dj = -1; dj<= 1; dj++){
          if(di == 0 && dj == 0) continue ;
          if(grid[i+di][j+dj] & 1) neighborSumm++ ;
        }
      }
      if(grid[i][j] & 1) { // ta vivo  
        if(neighborSumm == 2 || neighborSumm == 3) {
          grid[i][j] = 3 ;
        } else grid[i][j] = 1 ;
      } else { // atualmente morto
        if(neighborSumm == 3) {
          grid[i][j] = 2 ;
        } else grid[i][j] = 0 ;
      } 
    }
  }

  
  for(int i = 1; i<ROWS-1; i++)
    for(int j = 1; j<COLUMNS-1; j++) {
      grid[i][j] >>= 1;
      alive += grid[i][j];
    }
  return alive ;
}


/*
  Reproduction between two grids
*/
int reproduceGrids(short ROWS, short COLUMNS, short **g1, short **g2, short **gr) {
  int resultAlive = 0 ;
  for(int i = 0; i<ROWS; i++){
    for(int j = 0; j<COLUMNS; j++) {
      if(g1[i][j] && g2[i][j]) {
        resultAlive++ ;
        gr[i][j] = 1 ;
      } else if (!(g1[i][j] || g2[i][j])) {
        gr[i][j] = 0 ;
      } else {
        if(rand() & 2) {
          gr[i][j] = 1 ;
          resultAlive++ ;
        } else gr[i][j] = 0 ;
      }
    }
  }
  return resultAlive ;
}

/*
  Mutates the grid following REPFACTOR
*/
int mutateGrid(short ROWS, short COLUMNS, double REPFACTOR, short **grid) {
  double fac = REPFACTOR ;
  int mutations = fac * ROWS * COLUMNS ;
  for(int m = 0; m<mutations; m++){
    int mi = rand() % ROWS ;
    int mj = rand() % COLUMNS ;
    grid[mi][mj] = rand() % 2 ;
  }
  return mutations ;
}
