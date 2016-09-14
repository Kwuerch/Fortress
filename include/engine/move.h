#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include "config.h"

uint64_t nortOne( uint64_t bb );
uint64_t soutOne( uint64_t bb );
uint64_t eastOne( uint64_t bb );
uint64_t westOne( uint64_t bb );
uint64_t neasOne( uint64_t bb );
uint64_t nwesOne( uint64_t bb );
uint64_t seasOne( uint64_t bb );
uint64_t swesOne( uint64_t bb );

typedef uint16_t Move;

uint64_t rayAttacks( uint64_t occ, Piece p, uint8_t sq );

void createMove( uint8_t from, uint8_t to, uint8_t moveFlags, Move* mv );

uint8_t getFrom(Move* mv);

uint8_t getTo(Move* mv);

uint8_t getMoveFlags(Move* mv);

#endif
