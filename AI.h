#include <stdlib.h>
#include <stdbool.h>

#include "graph.h"

typedef struct score_column {
  int score;
  int col;
} Score_Column;

int AI_getRandomMove(Graph*);
int AI_getMove(Graph*);
int AI_getBestMove(Graph*);
Score_Column AI_minimax(Graph*, int, bool, int, int, int);
int AI_calcMoveScore(Graph*, int, bool);
int AI_getScoreHorizontal(Graph*, int, int, int, int, int, wchar_t, wchar_t);
int AI_getScoreVertical(Graph*, int, int, int, int, int, wchar_t, wchar_t);
int AI_getScoreDiagonalUp(Graph*, int, int, int, int, int, wchar_t, wchar_t);
int AI_getScoreDiagonalDown(Graph*, int, int, int, int, int, wchar_t, wchar_t);
