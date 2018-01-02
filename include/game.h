#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "move.h"
#include "board.h"
#include "config.h"

void playGame();
Move randomMove(board* b, Color c);

#endif
