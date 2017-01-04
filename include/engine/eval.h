#ifndef EVAL_H_INCLUDED
#define EVAL_H_INCLUDED

#include <stdint.h>

#include "config.h"
#include "board.h"
#include "move.h"


Move     calcBestMove(board *b);
uint16_t scoreBranch(board *b, uint8_t depth);
uint16_t scoreBoard( board *b, Color c);
uint8_t  popcnt( uint64_t bb );


#endif
