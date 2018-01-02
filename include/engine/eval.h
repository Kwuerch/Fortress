#ifndef EVAL_H_INCLUDED
#define EVAL_H_INCLUDED

#include <stdint.h>

#include "config.h"
#include "board.h"
#include "move.h"


Move     alphaBetaMaxRoot( board* b, int depth );
int      alphaBetaMax( board* b, int alpha, int beta, int depth );
int      alphaBetaMin( board* b, int alpha, int beta, int depth );
int      scoreBoard( board *b );
uint8_t  popcnt( uint64_t bb );

#endif
