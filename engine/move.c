#include <stdint.h>
#include <stdio.h>

#include "config.h"
#include "move.h"
#include "board.h"
#include "mask.h"

uint64_t nortOne( uint64_t bb ) { return bb << 8; }
uint64_t soutOne( uint64_t bb ) { return bb >> 8; }
uint64_t eastOne( uint64_t bb ) { return ( bb << 1 ) & notAFile; }
uint64_t westOne( uint64_t bb ) { return ( bb >> 1 ) & notHFile; }
uint64_t neasOne( uint64_t bb ) { return ( bb << 9 ) & notAFile; }
uint64_t nwesOne( uint64_t bb ) { return ( bb << 7 ) & notHFile; }
uint64_t seasOne( uint64_t bb ) { return ( bb >> 7 ) & notAFile; }
uint64_t swesOne( uint64_t bb ) { return ( bb >> 9 ) & notHFile; }

uint64_t rayAttacks( uint64_t occupied, Piece p, uint8_t sq ){
    uint64_t moves = piecMask[p][sq];
    for( uint64_t blockers = blBeMask[p][sq] & occupied; blockers != 0; blockers &= (blockers - 1)){
        moves &= ~shadMask[sq][bitScanForward(blockers)]; 
    }
    return moves;
}

void createMove( uint8_t from, uint8_t to, uint8_t moveFlag,  Move* mv){
    *mv = 0;
    *mv |= (0x3f & from);
    *mv <<= 6;
    *mv |= (0x3f & to);
    *mv <<= 4;
    *mv |= (0xf & moveFlag);
}

uint8_t getFrom(Move* mv){ return (*mv) >> 10; }

uint8_t getTo(Move* mv){ return ( *mv >> 4) & 0x3f; }

uint8_t getMoveFlags(Move* mv){ return *mv & 0xf; }

// TODO - update to Linked List instead of array

void genMoves(Color colr, board* b, Move* mvs){

}

void genPawnMoves(Color colr, board* b, Move* mvs){

}

void genKnightMoves(Color colr, board* b, Move* mvs){

}

void genKingMoves(Color colr, board* b, Move* mvs){

}

void genQueenMoves(Color colr, board* b, Move* mvs){

}

void genRookMoves(Color colr, board* b, Move* mvs){
/**
    while(pieces != 0){
        uint64_t attacks = rayAttacks(
        while(){

        }
    }
    
**/
}

void genBishopMoves(){

}
