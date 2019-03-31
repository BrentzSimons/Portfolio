#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>

int UI_getMove(bool);
int UI_getRows();
int UI_getCols();
int UI_getGamemode();
int UI_getDifficulty();
void UI_printWin(wchar_t);
int UI_playAgain();
int UI_promptToKeepSettings();
int UI_promptToChangeBoard();
int UI_promptToChangeGamemode(bool);
int UI_promptToChangeDifficulty();
int UI_promptToResetScore();
void UI_printClosingScreen(int, int, int);
void UI_printErrorColumnFull();
void UI_printErrorMoveOutOfBounds(int);
