typedef struct
{
  int alive ;
  int idx ;
} results_t ;

short **mallocGrid(short ROWS, short COLUMNS) ;
void freeGrid(short ROWS, short **grid) ;

void copyGrid(short ROWS, short COLUMNS, short **grid, short **src) ;
