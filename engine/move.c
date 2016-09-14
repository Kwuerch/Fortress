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

Move createMove( uint8_t from, uint8_t to, uint8_t moveFlag){
    Move mv = 0;
    mv |= (0x3f & from);
    mv <<= 6;
    mv |= (0x3f & to);
    mv <<= 4;
    mv |= (0xf & moveFlag);
    return mv;
}

uint8_t getFrom(Move mv){ return mv >> 10; }

uint8_t getTo(Move mv){ return ( mv >> 4) & 0x3f; }

uint8_t getMoveFlags(Move mv){ return mv & 0xf; }

void printMove(Move mv){
    printf("To:%i ", getTo(mv));
    printf("From:%i ", getFrom(mv));
    printf("Flags:%i\n ", getMoveFlags(mv));
}

// TODO - update to Linked List instead of array

void genMoves(Color colr, board* b, Move* mvs){
    uint64_t occ = occupied(b);
    if(colr == WHITE){
        genPawnMoves(occ, b -> wp, mvs);
        genKnightMoves(occ, b -> wn, mvs);
        genKingMoves(occ, b -> wk, mvs);
        genQueenMoves(occ, b -> wq, mvs);
        genRookMoves(occ, b -> wr, mvs);
        genBishopMoves(occ, b -> wh, mvs);
    }else{
        genPawnMoves(occ, b -> bp, mvs);
        genKnightMoves(occ, b -> bn, mvs);
        genKingMoves(occ, b -> bk, mvs);
        genQueenMoves(occ, b -> bq, mvs);
        genRookMoves(occ, b -> br, mvs);
        genBishopMoves(occ, b -> bh, mvs);
    }
}

void genPawnMoves(uint64_t occupied, uint64_t pawns, Move* mvs){

}

void genKnightMoves(uint64_t occupied, uint64_t knight, Move* mvs){

}

void genKingMoves(uint64_t occupied, uint64_t king, Move* mvs){

}

void genQueenMoves(uint64_t occupied, uint64_t queens, Move* mvs){

}

void genRookMoves(uint64_t occupied, uint64_t rooks, Move* mvs){
    if(rooks == 0){ return; }

    // Go through each rook and go through each move for each rook
    for(uint8_t from = bitScanForward(rooks); rooks != 0; rooks &= (rooks -1)){
        uint64_t moveSquares = rayAttacks(occupied, ROOK, from);
        for(uint64_t attacks = moveSquares & occupied; attacks != 0; attacks &= (attacks - 1)){
            Move m = createMove(from, bitScanForward(attacks), CAPTURE);
            printMove(m);
        }

        for(uint64_t quiets = moveSquares & ~occupied; quiets != 0; quiets &= (quiets - 1)){
            Move m = createMove(from, bitScanForward(quiets), QUIET);
            printMove(m);
        }
    }
}

void genBishopMoves(uint64_t occupied, uint64_t bishops, Move* mvs){

}
