#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>
#include <locale.h>

typedef struct graph {
  int rows, cols;
  wchar_t **board;
  int *colCounter;
} Graph;

Graph* Graph_create(int, int);
Graph* Graph_reset(Graph*);
void Graph_print(Graph*);
Graph* Graph_insertDisc(Graph*, int, wchar_t);
int Graph_checkForWin(Graph*);
int Graph_getCols(Graph*);
int Graph_getColCounter(Graph*, int);
