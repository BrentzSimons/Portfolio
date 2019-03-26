#include "ui.h"

int UI_getMove(bool turn) {
  if (turn) {
    wprintf(L"White, please enter which column you would like to play: ");
    int move;
    while(scanf("%d", &move) != 1) {
        wprintf(L"Please only enter an integer: ");
        while(getchar() != '\n');
    }
    return move;
  }
  wprintf(L"Black, please enter which column you would like to play: ");
  int move;
  while(scanf("%d", &move) != 1) {
      wprintf(L"Please only enter an integer: ");
      while(getchar() != '\n');
  }
  return move;
}

int UI_getRows() {
  wprintf(L"Please enter how many rows you want: ");
  int rows;
  while(scanf("%d", &rows) != 1) {
      wprintf(L"Please only enter an integer: ");
      while(getchar() != '\n');
  }
  return rows;
}

int UI_getCols() {
  wprintf(L"Please enter how many columns you want: ");
  int cols;
  while(scanf("%d", &cols) != 1) {
      wprintf(L"Please only enter an integer: ");
      while(getchar() != '\n');
  }
  return cols;
}

int UI_getGamemode() {
  wprintf(L"Would you like to play against the computer or a person? (Enter 0 for computer or 1 for person) ");
  int gamemode;
  while(scanf("%d", &gamemode) != 1 || (gamemode != 0 && gamemode != 1)) {
      wprintf(L"Please only enter 0 or 1: ");
      while(getchar() != '\n');
  }
  return gamemode;
}

int UI_getDifficulty() {
  wprintf(L"Select a difficulty for the computer (1 is easy, 2 is medium, 3 is hard) ");
  int difficulty;
  while(scanf("%d", &difficulty) != 1 || (difficulty != 1 && difficulty != 2 && difficulty != 3)) {
      wprintf(L"Please only enter 1, 2, or 3: ");
      while(getchar() != '\n');
  }
  return difficulty;
}

void UI_printWin(wchar_t winner) {
  if (winner == 't') {
    wprintf(L"It's a tie game!!\n");
    return;
  }
  if (winner == 0x25CB) {
    wprintf(L"Black wins!!\n");
    return;
  }
  wprintf(L"White wins!!\n");
  return;
}

int UI_playAgain() {
  wprintf(L"\nWould you like to play again? (1 for yes, 0 for no) ");
  int input;
  while (scanf("%d", &input) != 1 || (input != 0 && input != 1)) {
    wprintf(L"Please only enter 1 or 0: ");
    while(getchar() != '\n');
  }

  return input;
}

void UI_printClosingScreen(int numRedWins, int numBlackWins, int numTieGames) {
  system("clear");
  wprintf(L"\nThank you for playing!\n\n");
  if (numRedWins == 1)
    wprintf(L"White won 1 time\n");
  else
    wprintf(L"White won %d times\n", numRedWins);

  if (numBlackWins == 1)
    wprintf(L"Black won 1 time\n");
  else
    wprintf(L"Black won %d times\n", numBlackWins);

  if (numTieGames == 1)
    wprintf(L"There was 1 tie game\n");
  else
    wprintf(L"There were %d tie games\n", numTieGames);

  wprintf(L"\n");

  return;
}

void UI_printErrorColumnFull() {
  wprintf(L"I'm sorry that column is already full, please try again.\n");
  return;
}

void UI_printErrorMoveOutOfBounds(int cols) {
  wprintf(L"Please only enter 1-%d. ", cols);
  return;
}
