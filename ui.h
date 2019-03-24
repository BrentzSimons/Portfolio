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
void UI_printClosingScreen(int, int);
void UI_printErrorColumnFull();
void UI_printErrorMoveOutOfBounds(int);
