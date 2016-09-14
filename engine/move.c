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
    printf("From:%i\t", getFrom(mv));
    printf("To:%i\t", getTo(mv));
    printf("Flags:%i\n", getMoveFlags(mv));
}

// TODO - update to Linked List instead of array

void genMoves(Color colr, board* b, Move* mvs){
    uint64_t occ = occupied(b);
    uint64_t wht = white(b);
    uint64_t blk = black(b);
    if(colr == WHITE){
        genPawnMoves(occ, wht, b -> wp, mvs);
        genKnightMoves(occ, wht, b -> wn, mvs);
        genKingMoves(occ, wht, b -> wk, mvs);
        genQueenMoves(occ, wht, b -> wq, mvs);
        genRookMoves(occ, wht, b -> wr, mvs);
        genBishopMoves(occ, wht, b -> wh, mvs);
    }else{
        genPawnMoves(occ, blk, b -> bp, mvs);
        genKnightMoves(occ, blk, b -> bn, mvs);
        genKingMoves(occ, blk, b -> bk, mvs);
        genQueenMoves(occ, blk, b -> bq, mvs);
        genRookMoves(occ, blk, b -> br, mvs);
        genBishopMoves(occ, blk, b -> bh, mvs);
    }
}

void genPawnMoves(uint64_t occ, uint64_t opp, uint64_t pawns, Move* mvs){

}

void genKnightMoves(uint64_t occ, uint64_t opp, uint64_t knights, Move* mvs){
    if(knights == 0){ return; }

    // Go through each knight and append each individual's moves 
    for(uint8_t from; knights != 0; knights &= (knights -1)){
        from = bitScanForward(knights);
        for(uint64_t attacks = piecMask[KNIGHT][from] & opp; attacks != 0; attacks &= (attacks - 1)){
            Move m = createMove(from, bitScanForward(attacks), CAPTURE);
            printMove(m);
        }

        for(uint64_t quiets = piecMask[KNIGHT][from] & ~occ; quiets != 0; quiets &= (quiets - 1)){
            Move m = createMove(from, bitScanForward(quiets), QUIET);
            printMove(m);
        }
    }
}

void genKingMoves(uint64_t occ, uint64_t opp, uint64_t king, Move* mvs){
    if(king == 0){ return; }

    // Go through each knight and append each individual's moves 
    for(uint8_t from; king != 0; king &= (king -1)){
        from = bitScanForward(king);
        for(uint64_t attacks = piecMask[KING][from] & opp; attacks != 0; attacks &= (attacks - 1)){
            Move m = createMove(from, bitScanForward(attacks), CAPTURE);
            printMove(m);
        }

        for(uint64_t quiets = piecMask[KING][from] & ~occ; quiets != 0; quiets &= (quiets - 1)){
            Move m = createMove(from, bitScanForward(quiets), QUIET);
            printMove(m);
        }
    }
}

void genQueenMoves(uint64_t occ, uint64_t opp, uint64_t queens, Move* mvs){
    if(queens == 0){ return; }

    // Go through each queen and go through each queens moves 
    for(uint8_t from; queens != 0; queens &= (queens -1)){
        from = bitScanForward(queens);
        uint64_t moveSquares = rayAttacks(occ, QUEEN, from);
        for(uint64_t attacks = moveSquares & opp; attacks != 0; attacks &= (attacks - 1)){
            Move m = createMove(from, bitScanForward(attacks), CAPTURE);
            printMove(m);
        }

        for(uint64_t quiets = moveSquares & ~occ; quiets != 0; quiets &= (quiets - 1)){
            Move m = createMove(from, bitScanForward(quiets), QUIET);
            printMove(m);
        }
    }
}

void genRookMoves(uint64_t occ, uint64_t opp, uint64_t rooks, Move* mvs){
    if(rooks == 0){ return; }

    // Go through each rook and go through each move for each rook
    for(uint8_t from; rooks != 0; rooks &= (rooks -1)){
        from = bitScanForward(rooks);
        uint64_t moveSquares = rayAttacks(occ, ROOK, from);
        for(uint64_t attacks = moveSquares & opp; attacks != 0; attacks &= (attacks - 1)){
            Move m = createMove(from, bitScanForward(attacks), CAPTURE);
            printMove(m);
        }

        for(uint64_t quiets = moveSquares & ~occ; quiets != 0; quiets &= (quiets - 1)){
            Move m = createMove(from, bitScanForward(quiets), QUIET);
            printMove(m);
        }
        
    }
}

void genBishopMoves(uint64_t occ, uint64_t opp, uint64_t bishops, Move* mvs){
    if(bishops == 0){ return; }

    // Go through each bishop and go through each bishop's moves 
    for(uint8_t from; bishops != 0; bishops &= (bishops -1)){
        from = bitScanForward(bishops);
        uint64_t moveSquares = rayAttacks(occ, BISHOP, from);
        for(uint64_t attacks = moveSquares & opp; attacks != 0; attacks &= (attacks - 1)){
            Move m = createMove(from, bitScanForward(attacks), CAPTURE);
            printMove(m);
        }

        for(uint64_t quiets = moveSquares & ~occ; quiets != 0; quiets &= (quiets - 1)){
            Move m = createMove(from, bitScanForward(quiets), QUIET);
            printMove(m);
        }
    }
}
