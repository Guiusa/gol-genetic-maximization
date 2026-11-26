#define LOGDIR "logbests/"
#define GRAPHLOG "graph_log.txt"

void printGrid(short ROWS, short COLUMNS, short **grid) ;
void printNumbGrid(short ROWS, short COLUMNS, short **grid) ;
void writeLogHeader(short ROWS, short COLUMNS, short goliter) ;
void writeGenDataLogHeader(int rounds) ;
void logBest(short ROWS, short COLUMNS, short **grid)  ;
void logGenData(int best, int worst, int mean) ;
