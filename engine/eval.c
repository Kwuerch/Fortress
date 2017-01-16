#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "config.h"
#include "eval.h"
#include "board.h"
#include "mask.h"
#include "move.h"
#include "moveList.h"

#define MAX_TREE_DEPTH 3

Move calcBestMove(board *b){
    moveList *ml = genMoves(WHITE, b); 
    moveStack *ms = moveStackNew();

    Move bestMove = 0; // Checmate
    uint16_t maxScore = 0;

    moveListNode *cur = ml -> head;

    while(cur != NULL){
        makeMove(WHITE, b, cur -> mv, ms);

        uint16_t branchScore = scoreBranch(b, MAX_TREE_DEPTH);
        if( branchScore > maxScore ){
            maxScore = branchScore;
            bestMove = cur -> mv;
        }

        unmakeMove(b, ms);

        cur = cur -> next;

    }

    freeMoveStack(ms);
    freeMoveList(ml);

    // Returns  0 if checkmate an actual move otherwise
    return bestMove;
}

uint16_t scoreBranch(board *b, uint8_t depth){
    uint16_t score = 0;
    uint16_t count = 0;

    if( depth == 0 ){
        return scoreBoard(b, WHITE);
    }

    moveList *blMl = genMoves(BLACK, b);

    // Opponent Checkmate
    if(blMl -> head == NULL){
        freeMoveList(blMl);
        return 1000;
    }

    moveStack *ms = moveStackNew();

    moveListNode *curBlk = blMl -> head;

    // Iterate through all black moves and generate white moves to evaluate
    while(curBlk != NULL){
        makeMove(BLACK, b, curBlk -> mv, ms);

        // Generate new moves
        moveList *whMl = genMoves(WHITE, b);

        moveListNode *curWht = whMl -> head;

        // White Checkmate
        if(curWht == NULL){
            count ++; // increment score by zero
        }

        while(curWht != NULL){
            makeMove(WHITE, b, curWht -> mv, ms);

            score += scoreBranch(b, depth - 1);
            count ++;
            
            unmakeMove(b, ms);
            
            curWht = curWht -> next; 
        }

        unmakeMove(b, ms);
        freeMoveList(whMl);

        curBlk = curBlk -> next;
    }

    freeMoveList(blMl);
    freeMoveStack(ms);

    return score/count;
}

uint16_t scoreBoard( board *b, Color c){
    uint16_t score = 100; // Set to 100 to differentiate from checkmate


    if( c == WHITE ){
        score += popcnt(b -> wq) * 9;
        score += popcnt(b -> wr) * 5;
        score += popcnt(b -> wh) * 3;
        score += popcnt(b -> wn) * 3;
        score += popcnt(b -> wp) * 1;

        score -= popcnt(b -> wq) * 9;
        score -= popcnt(b -> wr) * 5;
        score -= popcnt(b -> wh) * 3;
        score -= popcnt(b -> wn) * 3;
        score -= popcnt(b -> wp) * 1;
        
    }else{
        score += popcnt(b -> wq) * 9;
        score += popcnt(b -> wr) * 5;
        score += popcnt(b -> wh) * 3;
        score += popcnt(b -> wn) * 3;
        score += popcnt(b -> wp) * 1;

        score -= popcnt(b -> wq) * 9;
        score -= popcnt(b -> wr) * 5;
        score -= popcnt(b -> wh) * 3;
        score -= popcnt(b -> wn) * 3;
        score -= popcnt(b -> wp) * 1;
    }

    return score;
}

uint8_t popcnt(uint64_t bb){
    uint8_t count;
    for(count = 0; bb != 0; count++, bb &= (bb - 1));

    return count;
}

int alphaBetaMax(board *b, int alpha, int beta, int depth){
    if(depth == 0){
        return scoreBoard(b);
    }

    moveStack *ms = moveStackNew();
    moveList *ml = genMoves(BLACK, ml);

    moveListNode cur = ml -> head;
    while(cur != NULL){
        makeMove(b, cur -> mv, ms);
        int score = alphaBetaMin(b, alpha, beta, depth - 1);

        if(score >= aplha){
            
        }
    }


}

int alphaBetaMin(board *b, int alpha, int beta, int depth){
    if(depth == 0){
        return scoreBoard(b);
    }

}
