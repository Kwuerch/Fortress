#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include "config.h"
#include "board.h"
#include "moveList.h"
#include "moveStack.h"
#include <stdint.h>

/**--------------------------------------------------------------*\
 *           FROM         |          TO          |   MOVE FLAGS   | 
 *            6           |           6          |       4        |
 *---------------------------------------------------------------*/

typedef uint16_t Move;

uint64_t nortOne( uint64_t bb );
uint64_t soutOne( uint64_t bb );
uint64_t eastOne( uint64_t bb );
uint64_t westOne( uint64_t bb );
uint64_t neasOne( uint64_t bb );
uint64_t nwesOne( uint64_t bb );
uint64_t seasOne( uint64_t bb );
uint64_t swesOne( uint64_t bb );


uint64_t rayAttacks( uint64_t occ, Piece p, uint8_t sq );

Move createMove( uint8_t from, uint8_t to, uint8_t moveFlags );

uint8_t getFrom(Move mv);

uint8_t getTo(Move mv);

uint8_t getMoveFlags(Move mv);

moveList* genMoves(Color colr, board* b);
void genPawnMoves(Color colr, uint64_t occ, uint64_t opp, uint64_t pawns, moveList* mvs);
void genKnightMoves(uint64_t occ, uint64_t opp, uint64_t knights, moveList* mvs);
void genKingMoves(board *b, Color c, moveList* mvs);
void genQueenMoves(uint64_t occ, uint64_t opp, uint64_t queens, moveList* mvs);
void genRookMoves(uint64_t occ, uint64_t opp, uint64_t rooks, moveList* mvs);
void genBishopMoves(uint64_t occ, uint64_t opp, uint64_t bishops, moveList* mvs);

void printMove(Move m);

void makeMove(Color c, board * b, Move m, moveStack *ms);
void unmakeMove(board *b, moveStack *ms);

#endif
