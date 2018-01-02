#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <limits.h>

#include "config.h"
#include "eval.h"
#include "board.h"
#include "mask.h"
#include "move.h"
#include "moveList.h"

Move alphaBetaMaxRoot( board* b, int depth ){
    if( depth == 0 ){ return noMove; }

    Move maxMove;
    int alpha = INT_MIN;

    moveStack* ms = moveStackNew();
    moveList *ml = genMoves(WHITE, b);

    moveListNode* cur = ml -> head;
    while( cur != NULL ){
        makeMove(WHITE, b, cur -> mv, ms);
        int score = alphaBetaMin(b, alpha, INT_MAX, depth - 1);
        //printf("score: %i\n", score);
        unmakeMove(b, ms);

        if(score > alpha){
            //printf("Found a better score %i \n", score );
            alpha = score;
            maxMove = cur -> mv;
        }

        cur = cur -> next;
    }

    printf("Score: %i \n", alpha );


    freeMoveStack(ms);
    freeMoveList(ml);

    return maxMove;
}

int alphaBetaMax( board* b, int alpha, int beta, int depth ){
    if( depth == 0 ){ return scoreBoard(b);}

    moveStack* ms = moveStackNew();
    moveList *ml = genMoves(WHITE, b);

    moveListNode* cur = ml -> head;

    if(cur == NULL){
        return maxMateVal;
    }

    while( cur != NULL ){
        makeMove(WHITE, b, cur -> mv, ms);
        int score = alphaBetaMin(b, alpha, beta, depth - 1);
        unmakeMove(b, ms);

        if(score >= beta){ 
            freeMoveStack(ms);
            freeMoveList(ml);
            return beta;
        }

        if(score > alpha){
            //printf("Found a bettter score: %i\n", score);
            alpha = score;
        }


        cur = cur -> next;
    }

    freeMoveStack(ms);
    freeMoveList(ml);

    return alpha;
}

int alphaBetaMin( board* b, int alpha, int beta, int depth ){
    if( depth == 0 ){ 
        //printf("%i\n",-scoreBoard(b));
        return -scoreBoard(b);
    }

    moveStack* ms = moveStackNew();
    moveList* ml = genMoves(BLACK, b);

    moveListNode* cur = ml -> head;
    
    if(cur == NULL){
        return minMateVal;
    }

    while( cur != NULL ){
        makeMove(BLACK, b, cur -> mv, ms);
        int score = alphaBetaMax(b, alpha, beta, depth - 1);
        unmakeMove(b, ms);

        if(score <= alpha){
            freeMoveStack(ms);
            freeMoveList(ml);
            return alpha;
        }

        if(score < beta){
            //printf("Found a worse score %i \n", score );
            beta = score;
        }

        cur = cur -> next;

    }

    freeMoveStack(ms);
    freeMoveList(ml);

    return beta;
}

int scoreBoard( board *b){
    int score = 0;

    score += popcnt(b -> wq) * 9;
    score += popcnt(b -> wr) * 5;
    score += popcnt(b -> wh) * 3;
    score += popcnt(b -> wn) * 3;
    score += popcnt(b -> wp) * 1;

    score += popcnt(b -> wq & midFour) * 3;
    score += popcnt((b -> wr | b -> wh) & midFour) * 2;
    score += popcnt(b -> wn & midFour) * 1;


    score -= popcnt(b -> bq) * 9;
    score -= popcnt(b -> br) * 5;
    score -= popcnt(b -> bh) * 3;
    score -= popcnt(b -> bn) * 3;
    score -= popcnt(b -> bp) * 1;

    score -= popcnt(b -> bq & midFour) * 3;
    score -= popcnt((b -> br | b -> bh) & midFour) * 2;
    score -= popcnt(b -> bn & midFour) * 1;

    return score;
}

uint8_t popcnt(uint64_t bb){
    uint8_t count;
    for(count = 0; bb != 0; count++, bb &= (bb - 1));

    return count;
}
