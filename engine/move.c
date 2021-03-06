#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "config.h"
#include "move.h"
#include "board.h"
#include "mask.h"
#include "moveList.h"
#include "moveStack.h"

static bool      isKingMoveValid(board *b, Color c, Move m);
static uint64_t *getFromBoard(board *b, Color c, Move m);
static uint64_t *getToBoard(board *b, Color c, Move m);

uint64_t nortOne( uint64_t bb ) { return bb << 8; }
uint64_t soutOne( uint64_t bb ) { return bb >> 8; }
uint64_t eastOne( uint64_t bb ) { return ( bb >> 1 ) & notAFile; }
uint64_t westOne( uint64_t bb ) { return ( bb << 1 ) & notHFile; }
uint64_t neasOne( uint64_t bb ) { return ( bb << 7 ) & notAFile; }
uint64_t nwesOne( uint64_t bb ) { return ( bb << 9 ) & notHFile; }
uint64_t seasOne( uint64_t bb ) { return ( bb >> 9 ) & notAFile; }
uint64_t swesOne( uint64_t bb ) { return ( bb >> 7 ) & notHFile; }

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
    moveList* ml = moveListNew();
    ml -> head = NULL;
    ml -> tail = NULL;

    uint64_t occ = occupied(b);
    uint64_t wht = white(b);
    uint64_t blk = black(b);
    if(colr == WHITE){
        genPawnMoves(colr, occ, blk, b -> wp, ml);
        genKnightMoves(occ, blk, b -> wn, ml);
        genKingMoves(b, WHITE, ml);
        genQueenMoves(occ, blk, b -> wq, ml);
        genRookMoves(occ, blk, b -> wr, ml);
        genBishopMoves(occ, blk, b -> wh, ml);
    }else{
        genPawnMoves(colr, occ, wht, b -> bp, ml);
        genKnightMoves(occ, wht, b -> bn, ml);
        genKingMoves(b, BLACK, ml);
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

void genKingMoves(board *b, Color c, moveList* ml){
    uint64_t occ = occupied(b);
    uint64_t king;
    uint64_t opp;

    if(c == WHITE){
        king = b -> wk;
        opp = black(b);
    }else{
        king = b -> bk;
        opp = white(b);
    }

    if(king == 0){ return; }

    // There is only one king
    uint8_t from = bitScanForward(king); 

    for(uint64_t attacks = piecMask[KING][from] & opp; attacks != 0; attacks &= (attacks - 1)){
        Move mv = createMove(from, bitScanForward(attacks), CAPTURE);
        if(isKingMoveValid(b, c, mv)){
            addMove(ml, mv);
        }
    }

    for(uint64_t quiets = piecMask[KING][from] & ~occ; quiets != 0; quiets &= (quiets - 1)){
        Move mv = createMove(from, bitScanForward(quiets), QUIET);
        if(isKingMoveValid(b, c, mv)){
            addMove(ml, mv);
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

static bool isKingMoveValid(board *b, Color c, Move m){

    uint8_t to = getTo(m);

    // Ensure current king piece does not affect calculations
    uint64_t occ = occupied(b) & ~indxMask[to];


    // Check if desired move location is under control of the opponent
    if(c == WHITE){
        // Check Full Diagonal Rays for desired move location
        for( uint64_t attackers = piecMask[BISHOP][to] & (b -> bq | b -> bh); attackers != 0; attackers &= (attackers - 1)){
            

            if((fromToMask[to][bitScanForward(attackers)] & occ) == 0 ){
                return false;
            }
        }
    

        // Check Full Horizontal and Vertical Rays for desired move location 
        for( uint64_t attackers = piecMask[ROOK][to] & (b -> bq | b -> br); 
                attackers != 0; attackers &= (attackers - 1)){
            

            if((fromToMask[to][bitScanForward(attackers)] & occ) == 0 ){
                return false;
            }
        }

        // Check Single Rays except attacking pawn locations for desired move
        if(((nortOne(indxMask[to]) | soutOne(indxMask[to]) | eastOne(indxMask[to]) 
                | westOne(indxMask[to]) | swesOne(indxMask[to]) | 
                    seasOne(indxMask[to])) & b -> bk) != 0){ 

            return false; 
        }

        // Check Single Rays of attacking pawn locations for desired move
        if(((neasOne(indxMask[to]) | nwesOne(indxMask[to])) & (b -> bk | b -> bp)) != 0){
            return false;
        }

        // Check Knight Attacks
        if((piecMask[KNIGHT][to] & b -> bn) != 0){
            return false;
        }

    }else{ // c == BLACK
        // Check Full Diagonal Rays for desired move location
        for( uint64_t attackers = piecMask[BISHOP][to] & (b -> wq | b -> wh);
                attackers != 0; attackers &= (attackers - 1)){
            

            if((fromToMask[to][bitScanForward(attackers)] & occ) == 0 ){
                return false;
            }
        }
    

        // Check Full Horizontal and Vertical Rays for desired move location 
        for( uint64_t attackers = piecMask[ROOK][to] & (b -> wq | b -> wr);
                attackers != 0; attackers &= (attackers - 1)){
            

            if((fromToMask[to][bitScanForward(attackers)] & occ) == 0 ){
                return false;
            }
        }

        // Check Single Rays except attacking pawn locations for desired move
        if(((nortOne(indxMask[to]) | soutOne(indxMask[to]) | eastOne(indxMask[to]) |
                westOne(indxMask[to]) | nwesOne(indxMask[to]) | neasOne(indxMask[to])) 
                    & b -> bk) != 0){ 

            return false; 
        }

        // Check Single Rays of attacking pawn locations for desired move
        if(((seasOne(indxMask[to]) | swesOne(indxMask[to])) & (b -> bk | b -> bp)) != 0){
            return false;
        }

        // Check Knight Attacks
        if((piecMask[KNIGHT][to] & b -> wn) != 0){
            return false;
        }

    }

    return true;
}

void makeMove(Color c, board *b, Move m, moveStack *stk){
    uint64_t from = indxMask[getFrom(m)];
    uint64_t to = indxMask[getTo(m)];

    uint64_t *fromBoard = getFromBoard(b, c, m);

    // Could send stackMove pointer in to get modified and have the captured piece added
    uint64_t *toBoard = getToBoard(b, c, m);

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

    stackMove sm = {.mv = m, .clr = c, .to = toBoard, .from = fromBoard};

    push(stk, &sm);
}

// Unmakes the most recent move on the stack
void unmakeMove(board *b, moveStack *stk){
    stackMove last = pop(stk);

    uint64_t from = indxMask[getFrom(last.mv)];
    uint64_t to = indxMask[getTo(last.mv)];

    uint64_t *fromBoard = last.from;
    uint64_t *toBoard = last.to;

    uint8_t flags = getMoveFlags(last.mv);

    switch(flags){
        case QUIET:
            *fromBoard &= ~to;
            *fromBoard |= from;
            break;
        case DOUBLE_PAWN:
            *fromBoard &= ~to;
            *fromBoard |= from;
            break;
        case KING_CASTLE:
            break;
        case QUEEN_CASTLE:
            break;
        case CAPTURE:
            *fromBoard &= ~to;
            *fromBoard |= from;
            *toBoard |= to;
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

static uint64_t* getFromBoard(board *b, Color c, Move mv){
    uint64_t from = indxMask[getFrom(mv)];
    uint64_t *fromBoard;

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
    }

    return fromBoard;
}

static uint64_t* getToBoard(board *b, Color c, Move mv){
    uint64_t to = indxMask[getTo(mv)];
    uint64_t* toBoard;

    if(c == WHITE){
        if(b -> bp & to){
            toBoard = &(b -> bp);
        }else if(b -> br & to){
            toBoard = &(b -> br);
        }else if(b -> bn & to){
            toBoard = &(b -> bn);
        }else if(b -> bh & to){
            toBoard = &(b -> bh);
        }else if(b -> bq & to){
            toBoard = &(b -> bq);
        }else if(b -> bk & to){
            toBoard = &(b -> bk);
        }
    }else{ // c == BLACK
        if(b -> wp & to){
            toBoard = &(b -> wp);
        }else if(b -> wr & to){
            toBoard = &(b -> wr);
        }else if(b -> wn & to){
            toBoard = &(b -> wn);
        }else if(b -> wh & to){
            toBoard = &(b -> wh);
        }else if(b -> wq & to){
            toBoard = &(b -> wq);
        }else if(b -> wk & to){
            toBoard = &(b -> wk);
        }
    }

    return toBoard;
}
