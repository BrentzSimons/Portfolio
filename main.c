#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>
#include <locale.h>

//#include "graph.h" (graph.h is included in the AI.h file)
#include "ui.h"
#include "AI.h"

int main(void) {
  setlocale(LC_CTYPE, "");
  bool turn = true;    // Follows whose turn it is. False is black, true is white.
  bool game = true;     // Tells if the game is continuing or is over.
  bool gamemode = false;  // Tells which gamemode to play. False = player vs computer. True = player vs player
  int difficulty;       // Controls difficulty for AI
  const wchar_t PLAYER_WHITE = 0x25CF, PLAYER_BLACK = 0x25CB;
  Graph *board = Graph_create(UI_getRows(), UI_getCols());
  int move=-1, win;
  int numWhiteWins = 0, numBlackWins = 0, numTieGames = 0;

  if (UI_getGamemode()) {
    gamemode = true;    // User is playing against another player
  } else {
    gamemode = false;   // User is playing against computer
    difficulty = UI_getDifficulty();
  }

  Graph_print(board);

  while (game) {
    if (turn || gamemode) {   // Checks if it is the users turn or if the game is player vs player
      UI_printPreviousMove(turn, move);
      move = UI_getMove(turn) - 1;
      while (move < 0 || move >= Graph_getCols(board)) {
        UI_printErrorMoveOutOfBounds(Graph_getCols(board));
        move = UI_getMove(turn) - 1;
      }
      while (Graph_getColCounter(board, move) < 0) {
        UI_printErrorColumnFull();
        move = UI_getMove(turn) - 1;
      }
    } else {
      switch (difficulty) {
        case 1: // Easy difficulty: Picks a random move
          move = AI_getRandomMove(board);
          break;
        case 2: // Medium difficulty: Picks a move that will give the best score
          move = AI_getMove(board);
          break;
        case 3: // Hard difficulty: Uses minimax to find the best move possible
          move = AI_getBestMove(board);
          break;
      }
    }

    if (turn)
      board = Graph_insertDisc(board, move, PLAYER_WHITE);
    else
      board = Graph_insertDisc(board, move, PLAYER_BLACK);

    Graph_print(board);
    win = Graph_checkForWin(board, move);
    if (win) {
      if (win == 2) { // Tie game
        UI_printWin('t');
        numTieGames++;
      } else {
        if (turn) {   // White wins
          UI_printWin(PLAYER_WHITE);
          numWhiteWins++;
        } else {      // Black wins
          UI_printWin(PLAYER_BLACK);
          numBlackWins++;
        }
      }

      UI_printGameWins(numWhiteWins, numBlackWins, numTieGames);

      if (UI_playAgain()) {
        if (!UI_promptToKeepSettings()) {
          if (UI_promptToChangeBoard()) {
            Graph_delete(board);
            board = Graph_create(UI_getRows(), UI_getCols());
          }
          if (gamemode) {
            if (UI_promptToChangeGamemode(gamemode)) {
              gamemode = false;
              difficulty = UI_getDifficulty();
            }
          } else {
            if (UI_promptToChangeGamemode(gamemode)) {
              gamemode = true;
            } else {
              if (UI_promptToChangeDifficulty()) {
                difficulty = UI_getDifficulty();
              }
            }
          }
          if (UI_promptToResetScore()) {
            numWhiteWins = 0;
            numBlackWins = 0;
            numTieGames = 0;
          }
        }
        win = 0;
        move = -1;
        board = Graph_reset(board);
        Graph_print(board);
      } else {
        game = false;
      }
    }
    turn = !turn;
  }

  Graph_delete(board);
  UI_printClosingScreen();
  UI_printGameWins(numWhiteWins, numBlackWins, numTieGames);

  return 0;
}
