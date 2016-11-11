#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "move.h"
#include "board.h"
#include "mask.h"
#include "moveList.h"

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

moveList* genMoves(Color colr, board* b){
    moveList* ml = malloc(sizeof(moveList));

    uint64_t occ = occupied(b);
    uint64_t wht = white(b);
    uint64_t blk = black(b);
    if(colr == WHITE){
        genPawnMoves(colr, occ, blk, b -> wp, ml);
        genKnightMoves(occ, blk, b -> wn, ml);
        genKingMoves(occ, blk, b -> wk, ml);
        genQueenMoves(occ, blk, b -> wq, ml);
        genRookMoves(occ, blk, b -> wr, ml);
        genBishopMoves(occ, blk, b -> wh, ml);
    }else{
        genPawnMoves(colr, occ, wht, b -> bp, ml);
        genKnightMoves(occ, wht, b -> bn, ml);
        genKingMoves(occ, wht, b -> bk, ml);
        genQueenMoves(occ, wht, b -> bq, ml);
        genRookMoves(occ, wht, b -> br, ml);
        genBishopMoves(occ, wht, b -> bh, ml);
    }

    return ml;
}

void genPawnMoves(Color c, uint64_t occ, uint64_t opp, uint64_t pawns, moveList* ml){
    if(pawns == 0){ return; }

    // Go through each pawn and append each individual's moves 
    for(uint8_t from; pawns != 0; pawns &= (pawns -1)){
        from = bitScanForward(pawns);

        for(uint64_t attacks = pawnMask[c][CAP][from] & opp; attacks != 0; attacks &= (attacks - 1)){
            addMove(ml,  createMove(from, bitScanForward(attacks), CAPTURE));
        }

        // Single Push
        for(uint64_t sPushes = pawnMask[c][PUSH][from] & ~occ; sPushes != 0; sPushes &= (sPushes - 1)){
            addMove(ml, createMove(from, bitScanForward(sPushes), QUIET));
        }

        // Double Push
        for(uint64_t dPushes = pawnMask[c][DOUBLE_PUSH][from] & ~occ; dPushes != 0; dPushes &= (dPushes - 1)){
            addMove(ml, createMove(from, bitScanForward(dPushes), DOUBLE_PAWN));
        }

        // Promos 
        for(uint64_t promos = pawnMask[c][PROMO][from] & ~occ; promos != 0; promos &= (promos - 1)){
            int proSq = bitScanForward(promos);
            addMove(ml, createMove(from, proSq, KNIGHT_PROMO));
            addMove(ml, createMove(from, proSq, BISHOP_PROMO));
            addMove(ml, createMove(from, proSq, ROOK_PROMO));
            addMove(ml, createMove(from, proSq, QUEEN_PROMO));
        }

        // Promo Captures 
        for(uint64_t promoCaps = pawnMask[c][PROMO_CAP][from] & opp; promoCaps != 0; promoCaps &= (promoCaps - 1)){
            int proSq = bitScanForward(promoCaps);
            addMove(ml, createMove(from, proSq, KNIGHT_PROMO_CAP));
            addMove(ml, createMove(from, proSq, BISHOP_PROMO_CAP));
            addMove(ml, createMove(from, proSq, ROOK_PROMO_CAP));
            addMove(ml, createMove(from, proSq, QUEEN_PROMO_CAP));
        }

    }

}

void genKnightMoves(uint64_t occ, uint64_t opp, uint64_t knights, moveList* ml){
    if(knights == 0){ return; }

    // Go through each knight and append each individual's moves 
    for(uint8_t from; knights != 0; knights &= (knights -1)){
        from = bitScanForward(knights);
        for(uint64_t attacks = piecMask[KNIGHT][from] & opp; attacks != 0; attacks &= (attacks - 1)){
            addMove(ml,  createMove(from, bitScanForward(attacks), CAPTURE));
        }

        for(uint64_t quiets = piecMask[KNIGHT][from] & ~occ; quiets != 0; quiets &= (quiets - 1)){
            addMove(ml, createMove(from, bitScanForward(quiets), QUIET));
        }
    }
}

void genKingMoves(uint64_t occ, uint64_t opp, uint64_t king, moveList* ml){
    if(king == 0){ return; }

    // Go through each knight and append each individual's moves 
    for(uint8_t from; king != 0; king &= (king -1)){
        from = bitScanForward(king);
        for(uint64_t attacks = piecMask[KING][from] & opp; attacks != 0; attacks &= (attacks - 1)){
            addMove(ml, createMove(from, bitScanForward(attacks), CAPTURE));
        }

        for(uint64_t quiets = piecMask[KING][from] & ~occ; quiets != 0; quiets &= (quiets - 1)){
            addMove(ml, createMove(from, bitScanForward(quiets), QUIET));
        }
    }
}

void genQueenMoves(uint64_t occ, uint64_t opp, uint64_t queens, moveList* ml){
    if(queens == 0){ return; }

    // Go through each queen and go through each queens moves 
    for(uint8_t from; queens != 0; queens &= (queens -1)){
        from = bitScanForward(queens);
        uint64_t moveSquares = rayAttacks(occ, QUEEN, from);
        for(uint64_t attacks = moveSquares & opp; attacks != 0; attacks &= (attacks - 1)){
            addMove(ml, createMove(from, bitScanForward(attacks), CAPTURE));
        }

        for(uint64_t quiets = moveSquares & ~occ; quiets != 0; quiets &= (quiets - 1)){
            addMove(ml, createMove(from, bitScanForward(quiets), QUIET));
        }
    }
}

void genRookMoves(uint64_t occ, uint64_t opp, uint64_t rooks, moveList* ml){
    if(rooks == 0){ return; }

    // Go through each rook and go through each move for each rook
    for(uint8_t from; rooks != 0; rooks &= (rooks -1)){
        from = bitScanForward(rooks);
        uint64_t moveSquares = rayAttacks(occ, ROOK, from);
        for(uint64_t attacks = moveSquares & opp; attacks != 0; attacks &= (attacks - 1)){
            addMove(ml, createMove(from, bitScanForward(attacks), CAPTURE));
        }

        for(uint64_t quiets = moveSquares & ~occ; quiets != 0; quiets &= (quiets - 1)){
            addMove(ml, createMove(from, bitScanForward(quiets), QUIET));
        }
    }
}

void genBishopMoves(uint64_t occ, uint64_t opp, uint64_t bishops, moveList* ml){
    if(bishops == 0){ return; }

    // Go through each bishop and go through each bishop's moves 
    for(uint8_t from; bishops != 0; bishops &= (bishops -1)){
        from = bitScanForward(bishops);
        uint64_t moveSquares = rayAttacks(occ, BISHOP, from);
        for(uint64_t attacks = moveSquares & opp; attacks != 0; attacks &= (attacks - 1)){
            addMove(ml, createMove(from, bitScanForward(attacks), CAPTURE));
        }

        for(uint64_t quiets = moveSquares & ~occ; quiets != 0; quiets &= (quiets - 1)){
            addMove(ml, createMove(from, bitScanForward(quiets), QUIET));
        }
    }
}

void makeMove(Color c, board * b, Move m){
    uint64_t from = indxMask[getFrom(m)];
    uint64_t to = indxMask[getTo(m)];

    uint64_t *fromBoard;
    uint64_t *toBoard;

    // Determine which piece is moving, and to where
    if(c == WHITE){
        if(b -> wp & from){
            fromBoard = &(b -> wp);
        }else if(b -> wr & from){
            fromBoard = &(b -> wr);
        }else if(b -> wn & from){
            fromBoard = &(b -> wn);
        }else if(b -> wh & from){
            fromBoard = &(b -> wh);
        }else if(b -> wq & from){
            fromBoard = &(b -> wq);
        }else if(b -> wk & from){
            fromBoard = &(b -> wk);
        }

        if(b -> bp & from){
            toBoard = &(b -> bp);
        }else if(b -> br & from){
            toBoard = &(b -> br);
        }else if(b -> bn & from){
            toBoard = &(b -> bn);
        }else if(b -> bh & from){
            toBoard = &(b -> bh);
        }else if(b -> bq & from){
            toBoard = &(b -> bq);
        }else if(b -> bk & from){
            toBoard = &(b -> bk);
        }

    }else{ // c == BLACK
        if(b -> bp & from){
            fromBoard = &(b -> bp);
        }else if(b -> br & from){
            fromBoard = &(b -> br);
        }else if(b -> bn & from){
            fromBoard = &(b -> bn);
        }else if(b -> bh & from){
            fromBoard = &(b -> bh);
        }else if(b -> bq & from){
            fromBoard = &(b -> bq);
        }else if(b -> bk & from){
            fromBoard = &(b -> bk);
        }

        if(b -> wp & from){
            toBoard = &(b -> wp);
        }else if(b -> wr & from){
            toBoard = &(b -> wr);
        }else if(b -> wn & from){
            toBoard = &(b -> wn);
        }else if(b -> wh & from){
            toBoard = &(b -> wh);
        }else if(b -> wq & from){
            toBoard = &(b -> wq);
        }else if(b -> wk & from){
            toBoard = &(b -> wk);
        }
    }

    uint8_t flags = getMoveFlags(m);
    switch(flags){
        case QUIET:
            *fromBoard &= ~from;
            *fromBoard |= to;
            break;
        case DOUBLE_PAWN:
            *fromBoard &= ~from;
            *fromBoard |= to;
            break;
        case KING_CASTLE:
            break;
        case QUEEN_CASTLE:
            break;
        case CAPTURE:
            *fromBoard &= ~from;
            *fromBoard |= to;
            *toBoard   &= ~to;
            break;
        case EP_CAPTURE:
            break;
        case KNIGHT_PROMO:
            break;
        case BISHOP_PROMO:
            break;
        case ROOK_PROMO:
            break;
        case QUEEN_PROMO:
            break;
        case KNIGHT_PROMO_CAP:
            break;
        case BISHOP_PROMO_CAP:
            break;
        case ROOK_PROMO_CAP:
            break;
        case QUEEN_PROMO_CAP:
            break;
    }

}

void undoMove(moveList m){

}
