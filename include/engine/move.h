#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include "config.h"
#include "board.h"

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

Move createMove( uint8_t from, uint8_t to, uint8_t moveFlags );

uint8_t getFrom(Move mv);

uint8_t getTo(Move mv);

uint8_t getMoveFlags(Move mv);

void genMoves(Color colr, board* b, Move* mvs);
void genPawnMoves(uint64_t occupied, uint64_t pawns, Move* mvs);
void genKnightMoves(uint64_t occupied, uint64_t knights, Move* mvs);
void genKingMoves(uint64_t occupied, uint64_t king, Move* mvs);
void genQueenMoves(uint64_t occupied, uint64_t queens, Move* mvs);
void genRookMoves(uint64_t occupied, uint64_t rooks, Move* mvs);
void genBishopMoves(uint64_t occupied, uint64_t bishops, Move* mvs);

void printMove(Move m);


#endif
