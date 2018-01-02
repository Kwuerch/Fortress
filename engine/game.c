#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "board.h"
#include "config.h"
#include "move.h"
#include "moveList.h"
#include "eval.h"

#define PLY_DEPTH 6

Move randomMove(board* b, Color c);

void playGame(){
    board b;
    b.wp = 0x000000000000ff00;
    b.wr = 0x0000000000000081;
    b.wn = 0x0000000000000042;
    b.wh = 0x0000000000000024;
    b.wq = 0x0000000000000010;
    b.wk = 0x0000000000000008;
    b.bp = 0x00ff000000000000;
    b.br = 0x8100000000000000;
    b.bn = 0x4200000000000000;
    b.bh = 0x2400000000000000;
    b.bk = 0x1000000000000000;
    b.bq = 0x0800000000000000;

    srand(time(NULL));   // Seed Rand

    moveStack *ms = malloc(sizeof(moveStack));
    Color c = WHITE;
    Move mv;

    printf("-----New Game----\n");
    printBoardFull(&b);

    while(1){

        if(c == WHITE){
            printf("---WHITE---\n");
            mv = alphaBetaMaxRoot(&b, PLY_DEPTH);
        }else{
            printf("---BLACK---\n");
            mv = randomMove(&b, c);
            printMove(mv);
        }

        if(mv == noMove){
            printf("Checkmate...or Stalemate\n");
            break;
        }
        makeMove(c, &b, mv, ms);
        c = (c == WHITE) ? BLACK : WHITE;

        printBoardFull(&b);
        printf("\n");
    }
}

Move randomMove(board* b, Color c){
    moveList *ml = genMoves(c, b);
    Move mv;

    int size = 0;
    moveListNode* cur = ml -> head;
    while(cur != NULL){
        size++;
        cur = cur -> next;
    }

    if(size == 0){
        return noMove;
    }

    int r = rand() % size;      // returns a pseudo-random integer between 0 and RAND_MAX
    size = 0;
    cur = ml -> head;
    while(size++ != r && cur != NULL){
        cur = cur -> next;
    }

    mv = cur -> mv;

    freeMoveList(ml);
    return mv; 
}


