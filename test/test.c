#include <stdlib.h>
#include <stdio.h>
#include "move.h"
#include "board.h"
#include "moveList.h"

void testMoveGen(){
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

    printBoardFull(&b);

    printf("%s\n", "White Moves:");
    moveList* ml = genMoves(WHITE, &b);
    printMoves(ml);

    printf("%s\n", "Black Moves:");
    ml = genMoves(BLACK, &b);
    printMoves(ml);

    freeMoveList(ml);

}

void testMoveMake(){
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

    printBoardFull(&b);

    printf("%s\n", "White Moves:");
    moveList* ml = genMoves(WHITE, &b);

    moveListNode* cur = ml -> head;

    while(cur != NULL){
        printMove(cur -> mv);

        makeMove(WHITE, &b, cur -> mv);
        printBoardFull(&b);

        cur = cur -> next;
    }


    printf("%s\n", "Black Moves:");
    ml = genMoves(BLACK, &b);

    cur = ml -> head;

    while(cur != NULL){
        printMove(cur -> mv);

        makeMove(BLACK, &b, cur -> mv);
        printBoardFull(&b);

        cur = cur -> next;
    }

    freeMoveList(ml);

}
