#include "graph.h"

struct Graph {
  int rows, cols;
  wchar_t **board;
  int *colCounter;
};

Graph* Graph_create(int r, int c) {
  Graph *tmp = malloc(sizeof(Graph));
  tmp->rows = r;
  tmp->cols = c;
  tmp->colCounter = malloc(c * sizeof(int));
  tmp->board = malloc(r * sizeof(wchar_t*));
  for (int i = 0; i < r; i++) {
    tmp->board[i] = malloc(c * sizeof(wchar_t*));
    for (int j = 0; j < c; j++) {
      tmp->board[i][j] = ' ';
      tmp->colCounter[j] = r-1;
    }
  }

  return tmp;
}

Graph* Graph_reset(Graph* self) {
  for (int r = 0; r < self->rows; r++) {
    for (int c = 0; c < self->cols; c++) {
      self->board[r][c] = ' ';
      self->colCounter[c] = self->rows - 1;
    }
  }

  return self;
}

void Graph_print(Graph* self) {
  system("clear");
  printf("\n");
  for (int r = 0; r < self->rows + 1; r++) {
    if (r != self->rows) {
      wprintf(L"|");
    } else {
      wprintf(L" ");
    }
    for (int c = 0; c < self->cols; c++) {
      if (r == self->rows) {
        if (c < 9)
          wprintf(L"%d  ", c + 1);
        else
          wprintf(L"%d ", c + 1);
      } else {
        wprintf(L"%lc", self->board[r][c]);
        wprintf(L" |");
      }
    }
    wprintf(L"\n");
  }
  wprintf(L"\n");

  return;
}

Graph* Graph_insertDisc(Graph* self, int move, wchar_t player) {
  self->board[self->colCounter[move]][move] = player;
  self->colCounter[move]--;
  return self;
}

int Graph_checkForWin(Graph* self) {
  int count = 0;
  for (int i = 0; i < self->cols; i++) {
    if (self->colCounter[i] < 0) count++;
  }
  if (count == self->cols) return 2;
  for (int r = 0; r < self->rows-3; r++) {
    for (int c = 0; c < self->cols-3; c++) {
      if (self->board[r][c] != ' ') {
        if ((self->board[r][c] == self->board[r+1][c+1]) && (self->board[r+1][c+1] == self->board[r+2][c+2]) && (self->board[r+2][c+2] == self->board[r+3][c+3]))
          return 1;
      }
    }
  }

  //checking up diagonal
  for (int r = 3; r < self->rows; r++) {
    for (int c = 0; c < self->cols-3; c++) {
      if (self->board[r][c] != ' ') {
        if ((self->board[r][c] == self->board[r-1][c+1]) && (self->board[r-1][c+1] == self->board[r-2][c+2]) && (self->board[r-2][c+2] == self->board[r-3][c+3]))
          return 1;
      }
    }
  }

  //checking horizontal
  for (int r = 0; r < self->rows; r++) {
    for (int c = 0; c < self->cols-3; c++) {
      if (self->board[r][c] != ' ') {
        if ((self->board[r][c] == self->board[r][c+1]) && (self->board[r][c+1] == self->board[r][c+2]) && (self->board[r][c+2] == self->board[r][c+3]))
          return 1;
      }
    }
  }

  //checking vertical
  for (int r = 0; r < self->rows-3; r++) {
    for (int c = 0; c < self->cols; c++) {
      if (self->board[r][c] != ' ') {
        if ((self->board[r][c] == self->board[r+1][c]) && (self->board[r+1][c] == self->board[r+2][c]) && (self->board[r+2][c] == self->board[r+3][c]))
          return 1;
      }
    }
  }

  return 0;
}

int Graph_getCols(Graph* self) {
  return self->cols;
}

int Graph_getColCounter(Graph* self, int move) {
  return self->colCounter[move];
}
