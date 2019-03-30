#include "AI.h"

int NEGATIVE_INFINITY = -2147483648;
int POSITIVE_INFINITY = 2147483647;

// Returns a random number from 0 to the amount of columns minus 1.
// This is the easy mode for the AI.
//
// Parameter: needs the Graph pointer that holds the information of the board.
int AI_getRandomMove(Graph* board) {
  int move = rand() % board->cols;
  while (board->colCounter[move] < 0) // Checks if the column is already full
    move = rand() % board->cols;

  return move;
}

// Goes through all of the columns and calculates a score for each, then
// returns the column with the heighest score.
// This is the medium mode for the AI.
//
// Parameter: needs the Graph pointer that holds the information of the board.
int AI_getMove(Graph* board) {
  int bestScore = 0;
  int bestMove = AI_getRandomMove(board);   // Sets the initial best move as random incase all columns have the same score
  for (int c = 0; c < board->cols; c++) {
    int r = board->colCounter[c];
    if (r >= 0) {  // Makes sure there is still a possible move in the current column
      board->board[r][c] = 0x25CB;  // Places black "ghost" piece into column
      int score = AI_calcMoveScore(board, c, false); // Calculates score for current column
      if (score > bestScore) {  // Keeps track of the max score and its respective column
        bestScore = score;
        bestMove = c;
      }
      board->board[r][c] = ' ';   // Removes the "ghost" piece.
    }
  }
  printf("\n");
  return bestMove;
}

// Calls the minimax algorithm to get the best move. Changes the depth for minimax based on
// the amount of columns in the board to lower runtime on bigger boards, but get better
// results on smaller boards.
// This is the hard mode for the AI.
//
// Parameter: needs the Graph pointer that holds the information of the board.
int AI_getBestMove(Graph* board) {
  Score_Column bestMove;
  if (board->cols < 20)
    bestMove = AI_minimax(board, 7, true, -1, NEGATIVE_INFINITY, POSITIVE_INFINITY);
  else if (board->cols < 40)
    bestMove = AI_minimax(board, 6, true, -1, NEGATIVE_INFINITY, POSITIVE_INFINITY);
  else if (board->cols < 55)
    bestMove = AI_minimax(board, 5, true, -1, NEGATIVE_INFINITY, POSITIVE_INFINITY);
  else if (board->cols < 100)
    bestMove = AI_minimax(board, 4, true, -1, NEGATIVE_INFINITY, POSITIVE_INFINITY);
  // else if (board->cols < 1)
  //   bestMove = AI_minimax(board, 3, true, 0, NEGATIVE_INFINITY, POSITIVE_INFINITY);
  else
    bestMove = AI_minimax(board, 2, true, -1, NEGATIVE_INFINITY, POSITIVE_INFINITY);

  return bestMove.col;
}

// Uses the minimax algorithm with alpha-beta pruning to recursively go through all of the possible moves
// and calculates a score.The stop condition happens when the depth reaches 0 or if there is a win.
//
// Parameters: needs the Graph pointer that holds the information of the board.
// A positive integer for the depth. A boolean that keeps track if you are on the player
// that was the max score. A integer
Score_Column AI_minimax(Graph* board, int depth, bool maximizingPlayer, int move, int alpha, int beta) {
  Score_Column bestMove;
  // If move is -1, minimax was just called from another function and will cause problems if checkForWin is called.
  // The recursive calls end whenever depth is equal to zero or if there is a win.
  if (move != -1 && (depth == 0 || Graph_checkForWin(board, move) != 0)) {
    board->colCounter[move]++; // Adds to the column counter for the current move because of how calcMoveScore is coded.
    if (maximizingPlayer) // If it is the users turn, get the negative version of the score because AI wants a bad score for the player.
      bestMove.score = AI_calcMoveScore(board, move, maximizingPlayer) * -1;
    else
      bestMove.score = AI_calcMoveScore(board, move, maximizingPlayer);
    board->colCounter[move]--; // Resets the column counter to where it was.
    return bestMove;
  }

  if (maximizingPlayer) {
    bestMove.score = NEGATIVE_INFINITY;
    for (int c = 0; c < board->cols; c++) {
      if (board->colCounter[c] >= 0) {
        Score_Column tmp; // Keeps track of the current Score and its respective column
        board->board[board->colCounter[c]][c] = 0x25CB;
        board->colCounter[c]--;
        tmp = AI_minimax(board, depth-1, false, c, alpha, beta); // Recursively calls minimax
        board->colCounter[c]++;
        board->board[board->colCounter[c]][c] = ' ';

        if (tmp.score > bestMove.score) { // Keeps track of the max score and its respective column
          bestMove.score = tmp.score;
          bestMove.col = c;
        }

        // Alpha-beta pruning
        if (bestMove.score > alpha)
          alpha = bestMove.score;
        if (alpha >= beta)
          break;
      }
    }
    return bestMove;
  } else {
    bestMove.score = POSITIVE_INFINITY;
    for (int c = 0; c < board->cols; c++) {
      if (board->colCounter[c] >= 0) {
        Score_Column tmp; // Keeps track of the current Score and its respective column
        bestMove.col = c;
        board->board[board->colCounter[c]][c] = 0x25CF;
        board->colCounter[c]--;
        tmp = AI_minimax(board, depth-1, true, c, alpha, beta); // Recursively calls minimax
        board->colCounter[c]++;
        board->board[board->colCounter[c]][c] = ' ';

        if (tmp.score < bestMove.score) { // Keeps track of the max score and its respective column
          bestMove.score = tmp.score;
          bestMove.col = c;
        }

        // Alpha-beta pruning
        if (bestMove.score < beta)
          beta = bestMove.score;
        if (alpha >= beta)
          break;
      }
    }
    return bestMove;
  }
}

int AI_calcMoveScore(Graph* board, int move, bool turn) {
  // Calculates the lower/upper column/row from the move
  int lowerCol = move-3, upperCol = move+3;
  int upperRow = board->colCounter[move]-3, lowerRow = board->colCounter[move]+3;
  if (lowerCol < 0) lowerCol = 0;
  if (upperCol > board->cols-1) upperCol = board->cols-1;
  if (upperRow < 0) upperRow = 0;
  if (lowerRow > board->rows-1) lowerRow = board->rows-1;

  int score = 0;

  wchar_t currPlayer, oppPlayer;
  if (turn) {
    currPlayer = 0x25CF;
    oppPlayer = 0x25CB;
  } else {
    currPlayer = 0x25CB;
    oppPlayer = 0x25CF;
  }

  score += AI_getScoreVertical(board, move, lowerCol, upperCol, lowerRow, upperRow, currPlayer, oppPlayer);
  score += AI_getScoreHorizontal(board, move, lowerCol, upperCol, lowerRow, upperRow, currPlayer, oppPlayer);
  score += AI_getScoreDiagonalUp(board, move, lowerCol, upperCol, lowerRow, upperRow, currPlayer, oppPlayer);
  score += AI_getScoreDiagonalDown(board, move, lowerCol, upperCol, lowerRow, upperRow, currPlayer, oppPlayer);

  // Checks to see if the opposite player is one move away from winning
  board->board[board->colCounter[move]][move] = oppPlayer;
  board->colCounter[move]--;
  if (Graph_checkForWin(board, move) == 1)
    score += 1000;
  board->colCounter[move]++;

  // If the move is the middle of the board, gives it a small score increase.
  if (move == board->cols/2) {
    score += 2;
  }

  return score;
}

int AI_getScoreHorizontal(Graph* board, int move, int lowerCol, int upperCol, int lowerRow, int upperRow, wchar_t currPlayer, wchar_t oppPlayer) {
  int score = 0;

  // Counts the number black discs in the same row as the potential move.
  // Only counts -3 to +3 columns from the potential move without going
  // under or over the amount of columns.
  for (int c = lowerCol; c < (upperCol-2); c++) {
    int count = 0;
    int r = board->colCounter[move];

    switch (0) {
      case 0:
        if (board->board[r][c] == currPlayer) {
          count++;
        } else if (board->board[r][c] == oppPlayer) {
          count = 0;
          break;
        }
      case 1:
        if (board->board[r][c+1] == currPlayer) {
          count++;
        } else if (board->board[r][c+1] == oppPlayer) {
          count = 0;
          break;
        }
      case 2:
        if (board->board[r][c+2] == currPlayer) {
          count++;
        } else if (board->board[r][c+2] == oppPlayer) {
          count = 0;
          break;
        }
      case 3:
        if (board->board[r][c+3] == currPlayer) {
          count++;
        } else if (board->board[r][c+3] == oppPlayer) {
          count = 0;
          break;
        }
    }
    //determines how many points to give based on how many discs were found
    switch (count) {
      case 2: // Two discs within 4 places
        score += 5;
        break;
      case 3: // Three discs within 4 places
        score += 20;
        break;
      case 4: // Four discs within 4 places (this is a win)
        score += 1000000000;
        break;
    }
  }

  return score;
}

int AI_getScoreVertical(Graph* board, int move, int lowerCol, int upperCol, int lowerRow, int upperRow, wchar_t currPlayer, wchar_t oppPlayer) {
  int score = 0;

  // Counts the number black discs in the same column as the potential move.
  // Only counts -3 to +3 rows from the potential move without going
  // under or over the amount of rows.
  for (int r = upperRow; r < (lowerRow-2); r++) {
    int count = 0;
    int c = move;

    switch (0) {
      case 0:
        if (board->board[r][c] == currPlayer) {
          count++;
        } else if (board->board[r][c] == oppPlayer) {
          count = 0;
          break;
        }
      case 1:
        if (board->board[r+1][c] == currPlayer) {
          count++;
        } else if (board->board[r+1][c] == oppPlayer) {
          count = 0;
          break;
        }
      case 2:
        if (board->board[r+2][c] == currPlayer) {
          count++;
        } else if (board->board[r+2][c] == oppPlayer) {
          count = 0;
          break;
        }
      case 3:
        if (board->board[r+3][c] == currPlayer) {
          count++;
        } else if (board->board[r+3][c] == oppPlayer) {
          count = 0;
          break;
        }
    }

    //determines how many points to give based on how many discs were found
    switch (count) {
      case 2: // Two discs within 4 places
        score += 5;
        break;
      case 3: // Three discs within 4 places
        score += 20;
        break;
      case 4: // Four discs within 4 places (this is a win)
        score += 1000000000;
        break;
    }
  }

  return score;
}

int AI_getScoreDiagonalUp(Graph* board, int move, int lowerCol, int upperCol, int lowerRow, int upperRow, wchar_t currPlayer, wchar_t oppPlayer) {
  int score = 0;

  // These nested if statements determine where the start and end of the window should be
  int start, end;
  if ((move - lowerCol) <= (lowerRow - board->colCounter[move])) {
    start = lowerCol;
    if ((upperCol - move) <= (board->colCounter[move] - upperRow)) {
      end = upperCol;
    } else {
      int difference = board->colCounter[move] - upperRow;
      end = move + difference;
    }
  } else {
    int difference = lowerRow - board->colCounter[move];
    start = move - difference;
    if ((upperCol - move) <= (board->colCounter[move] - upperRow)) {
      end = upperCol;
    } else {
      int difference = board->colCounter[move] - upperRow;
      end = move + difference;
    }
  }

  int size = end-start+1;   // Calculates how many elements are in the window
  if (size > 3) {
    wchar_t *window = malloc(size * sizeof(wchar_t));

    for (int i = 0; i < size; i++) { // Fills the window with the correct elements from the board
      window[i] = board->board[board->colCounter[move]-start+move-i][start+i];
    }

    int count = 0;
    for (int i = 0; i < size-3; i++) { // Checks through the window in sets of 4 to find common discs.
      count = 0;
      switch (0) {
        case 0:
          if (window[i] == currPlayer) {
            count++;
          } else if (window[i] == oppPlayer) {
            count = 0;
            break;
          }
        case 1:
          if (window[i+1] == currPlayer) {
            count++;
          } else if (window[i+1] == oppPlayer) {
            count = 0;
            break;
          }
        case 2:
          if (window[i+2] == currPlayer) {
            count++;
          } else if (window[i+2] == oppPlayer) {
            count = 0;
            break;
          }
        case 3:
          if (window[i+3] == currPlayer) {
            count++;
          } else if (window[i+3] == oppPlayer) {
            count = 0;
            break;
          }
      }
      //determines how many points to give based on how many discs were found
      switch (count) {
        case 2: // Two discs within 4 places
          score += 5;
          break;
        case 3: // Three discs within 4 places
          score += 20;
          break;
        case 4: // Four discs within 4 places (this is a win)
          score += 1000000000;
          break;
      }
    }
    free(window);
  }

  return score;
}

int AI_getScoreDiagonalDown(Graph* board, int move, int lowerCol, int upperCol, int lowerRow, int upperRow, wchar_t currPlayer, wchar_t oppPlayer) {
  int score = 0;

  // These nested if statements determine where the start and end of the window should be
  int start, end;
  if ((move - lowerCol) <= (board->colCounter[move] - upperRow)) {
    start = lowerCol;
    if ((upperCol - move) <= (lowerRow - board->colCounter[move])) {
      end = upperCol;
    } else {
      int difference = lowerRow - board->colCounter[move];
      end = move + difference;
    }
  } else {
    int difference = board->colCounter[move] - upperRow;
    start = move - difference;
    if ((upperCol - move) <= (lowerRow - board->colCounter[move])) {
      end = upperCol;
    } else {
      int difference = lowerRow - board->colCounter[move];
      end = move + difference;
    }
  }


  int size = end-start+1;   // Calculates how many elements are in the window
  if (size > 3) {
    wchar_t *window = malloc(size * sizeof(wchar_t));

    for (int i = 0; i < size; i++) { // Fills the window with the correct elements from the board
      window[i] = board->board[board->colCounter[move]+start-move+i][start+i];
    }

    int count = 0;
    for (int i = 0; i < size-3; i++) { // Checks through the window in sets of 4 to find common discs.
      count = 0;
      switch (0) {
        case 0:
          if (window[i] == currPlayer) {
            count++;
          } else if (window[i] == oppPlayer) {
            count = 0;
            break;
          }
        case 1:
          if (window[i+1] == currPlayer) {
            count++;
          } else if (window[i+1] == oppPlayer) {
            count = 0;
            break;
          }
        case 2:
          if (window[i+2] == currPlayer) {
            count++;
          } else if (window[i+2] == oppPlayer) {
            count = 0;
            break;
          }
        case 3:
          if (window[i+3] == currPlayer) {
            count++;
          } else if (window[i+3] == oppPlayer) {
            count = 0;
            break;
          }
      }
      //determines how many points to give based on how many discs were found
      switch (count) {
        case 2: // Two discs within 4 places
          score += 5;
          break;
        case 3: // Three discs within 4 places
          score += 20;
          break;
        case 4: // Four discs within 4 places (this is a win)
          score += 1000000000;
          break;
      }
    }
    free(window);
  }

  return score;
}
