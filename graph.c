#include "graph.h"

struct Graph {
  int rows, cols;
  wchar_t **board;
  int *colCounter;
};

// Creates the Graph struct and crates the board with r rows and c columns.
// Fills the board with spaces and sets the column counter to r-1.
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

// Frees up all memory that was allocated when creating the Graph struct.
void Graph_delete(Graph* board) {
  for (int i = 0; i < board->rows; i++) {
    free(board->board[i]);
  }
  free(board->board);
  free(board->colCounter);
  free(board);
}

// Resets the board to original clean state.
Graph* Graph_reset(Graph* board) {
  for (int r = 0; r < board->rows; r++) {
    for (int c = 0; c < board->cols; c++) {
      board->board[r][c] = ' ';
      board->colCounter[c] = board->rows - 1;
    }
  }

  return board;
}

// Prints out the board.
void Graph_print(Graph* board) {
  system("clear");
  printf("\n");
  for (int r = 0; r < board->rows + 1; r++) {
    if (r != board->rows) {
      wprintf(L"|");
    } else {
      wprintf(L" ");
    }
    for (int c = 0; c < board->cols; c++) {
      if (r == board->rows) {
        if (c < 9)
          wprintf(L"%d  ", c + 1);
        else
          wprintf(L"%d ", c + 1);
      } else {
        wprintf(L"%lc", board->board[r][c]);
        wprintf(L" |");
      }
    }
    wprintf(L"\n");
  }
  wprintf(L"\n");

  return;
}

// Inserts "player" disc into the board at column "move"
Graph* Graph_insertDisc(Graph* board, int move, wchar_t player) {
  board->board[board->colCounter[move]][move] = player;
  board->colCounter[move]--;
  return board;
}

// Checks if "move" creates a win. Only checks win conditions that include "move".
int Graph_checkForWin(Graph* board, int move) {
  if (board->colCounter[move] == 5) return 0;

  // Checks for a tie game
  bool tieGame = true;
  for (int i = 0; i < board->cols; i++) {
    if (board->colCounter[i] >= 0) {
      tieGame = false;
      break;
    }
  }
  if (tieGame) return 2;

  // Calculates the lower/upper column/row from the move
  int lowerCol = move-3, upperCol = move+3;
  int upperRow = board->colCounter[move]-2, lowerRow = board->colCounter[move]+4;
  if (lowerCol < 0) lowerCol = 0;
  if (upperCol > board->cols-1) upperCol = board->cols-1;
  if (upperRow < 0) upperRow = 0;
  if (lowerRow > board->rows-1) lowerRow = board->rows-1;

  // Checking horizontal
  for (int c = lowerCol; c < (upperCol-2); c++) {
    int r = (board->colCounter[move]+1);
    if (board->board[r][c] != ' ') {
      if ((board->board[r][c] == board->board[r][c+1]) && (board->board[r][c+1] == board->board[r][c+2]) && (board->board[r][c+2] == board->board[r][c+3]))
        return 1;
    }
  }

  // Checking vertical
  for (int r = upperRow; r < (lowerRow-2); r++) {
    int c = move;
    if (board->board[r][c] != ' ') {
      if ((board->board[r][c] == board->board[r+1][c]) && (board->board[r+1][c] == board->board[r+2][c]) && (board->board[r+2][c] == board->board[r+3][c]))
        return 1;
    }
  }

  // Checks diagonal up (bottom left to top right)
  // These nested if statements determine where the start and end of the window should be
  int start, end;
  if ((move - lowerCol) <= (lowerRow - (board->colCounter[move]+1))) {
    start = lowerCol;
    if ((upperCol - move) <= ((board->colCounter[move]+1) - upperRow)) {
      end = upperCol;
    } else {
      int difference = (board->colCounter[move]+1) - upperRow;
      end = move + difference;
    }
  } else {
    int difference = lowerRow - (board->colCounter[move]+1);
    start = move - difference;
    if ((upperCol - move) <= ((board->colCounter[move]+1) - upperRow)) {
      end = upperCol;
    } else {
      int difference = (board->colCounter[move]+1) - upperRow;
      end = move + difference;
    }
  }
  int size = end-start+1;   // Calculates how many elements are in the window
  if (size > 3) {
    wchar_t *window = malloc(size * sizeof(wchar_t));

    for (int i = 0; i < size; i++) { // Fills the window with the correct elements from the board
      window[i] = board->board[(board->colCounter[move]+1)-start+move-i][start+i];
    }

    for (int i = 0; i < size-3; i++) { // Checks through the window in sets of 4 to find common discs.
      if (window[i] != ' ') {
        if (window[i] == window[i+1] && window[i+1] == window[i+2] && window[i+2] == window[i+3]) {
          free(window);
          return 1;
        }
      }
    }
    free(window);
  }

  // Checks diagonal down (top left to bottom right)
  // These nested if statements determine where the start and end of the window should be
  if ((move - lowerCol) <= ((board->colCounter[move]+1) - upperRow)) {
    start = lowerCol;
    if ((upperCol - move) <= (lowerRow - (board->colCounter[move]+1))) {
      end = upperCol;
    } else {
      int difference = lowerRow - (board->colCounter[move]+1);
      end = move + difference;
    }
  } else {
    int difference = (board->colCounter[move]+1) - upperRow;
    start = move - difference;
    if ((upperCol - move) <= (lowerRow - (board->colCounter[move]+1))) {
      end = upperCol;
    } else {
      int difference = lowerRow - (board->colCounter[move]+1);
      end = move + difference;
    }
  }
  size = end-start+1;   // Calculates how many elements are in the window
  if (size > 3) {
    wchar_t *window = malloc(size * sizeof(wchar_t));

    for (int i = 0; i < size; i++) { // Fills the window with the correct elements from the board
      window[i] = board->board[(board->colCounter[move]+1)+start-move+i][start+i];
    }

    for (int i = 0; i < size-3; i++) { // Checks through the window in sets of 4 to find common discs.
      if (window[i] != ' ') {
        if (window[i] == window[i+1] && window[i+1] == window[i+2] && window[i+2] == window[i+3]) {
          free(window);
          return 1;
        }
      }
    }
    free(window);
  }

  return 0;
}

int Graph_getCols(Graph* board) {
  return board->cols;
}

int Graph_getColCounter(Graph* board, int move) {
  return board->colCounter[move];
}
