#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "mask.h"
#include "move.h"
#include "board.h"
#include "moveList.h"
#include "moveStack.h"

static void testShadowMask();
static void testFromToMask();
static void testMasks();

void runTests(){
    testMasks();
}

static void testMasks(){
    assert(indxMask[7] == 0x0000000000000080);
    assert(westMask[56] == 0xfe00000000000000);
    assert(neasMask[1] == 0x0000000000000100);
    assert(swesMask[17] == 0x0000000000000408);
    testShadowMask();
    testFromToMask();
}

static void testShadowMask(){
    assert(shadMask[1][41]  == 0x0202000000000000);
    printf("Passed Shadow North\n");

    assert(shadMask[54][22] == 0x0000000000004040);
    printf("Passed Shadow South\n");

    assert(shadMask[30][26] == 0x0000000003000000);
    printf("Passed Shadow East\n");

    assert(shadMask[34][36] == 0x000000e000000000);
    printf("Passed Shadow West\n");

    assert(shadMask[7][42]  == 0x0102000000000000);
    printf("Passed Shadow NorthEast\n");

    assert(shadMask[21][30] == 0x0000008000000000);
    printf("Passed Shadow NorthWest\n");

    assert(shadMask[60][42] == 0x0000000201000000);
    printf("Passed Shadow SouthEast\n");

    assert(shadMask[17][44] == 0x4020000000000000);
    printf("Passed Shadow SouthWest\n");
}

static void testFromToMask(){
    assert(fromToMask[12][44]  == 0x0000001010100000);
    printf("Passed FromTo North\n");

    assert(fromToMask[44][12] == 0x0000001010100000);
    printf("Passed FromTo South\n");

    assert(fromToMask[37][34] == 0x0000001800000000);
    printf("Passed FromTo East\n");

    assert(fromToMask[34][37] == 0x0000001800000000);
    printf("Passed FromTo West\n");

    assert(fromToMask[21][49] == 0x0000040810000000);
    printf("Passed FromTo NorthEast\n");

    assert(fromToMask[49][21] == 0x0000040810000000);
    printf("Passed FromTo SouthWest\n");

    assert(fromToMask[42][60] == 0x0008000000000000);
    printf("Passed FromTo NorthWest\n");

    assert(fromToMask[60][42] == 0x0008000000000000);
    printf("Passed FromTo SouthEast\n");

}

void testMoveGen(){
    board b;
    b.wp = 0x000000000000ff00;
    b.wr = 0x0000000000000081;
    b.wn = 0x0000000000000002;
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
    b.wn = 0x0000000000000002;
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

    moveStack *ms = malloc(sizeof(moveStack));

    printf("%s\n", "White Moves:");
    moveList *ml = genMoves(WHITE, &b);

    moveListNode* cur = ml -> head;

    while(cur != NULL){
        printMove(cur -> mv);

        makeMove(WHITE, &b, cur -> mv, ms);
        printBoardFull(&b);
        unmakeMove(&b, ms);

        cur = cur -> next;
    }


    /**
    printf("%s\n", "Black Moves:");
    ml = genMoves(BLACK, &b);

    cur = ml -> head;

    while(cur != NULL){
        printMove(cur -> mv);

        makeMove(BLACK, &b, cur -> mv);
        printBoardFull(&b);

        cur = cur -> next;
    }
    **/

    freeMoveList(ml);
    freeMoveStack(ms);

}

void testValidMoveGen(){
    board b;
    b.wp = 0x0000000000000000;
    b.wr = 0x0000000000000000;
    b.wn = 0x0000000000000000;
    b.wh = 0x0000000000000000;
    b.wq = 0x0000000000000000;
    b.wk = 0x0000000000001000;
    b.bp = 0x0000000020000000;
    b.br = 0x0000000040000080;
    b.bn = 0x0000000000000200;
    b.bh = 0x0000000000000000;
    b.bk = 0x0010000000000000;
    b.bq = 0x0000008000000000;

    printBoardFull(&b);

    // Moves
    Move move1 = createMove(12, 21, QUIET);
    Move move2 = createMove(12, 13, QUIET);
    Move move3 = createMove(12, 11, QUIET);


    moveList *ml = genMoves(WHITE, &b);

    moveListNode* cur = ml -> head;

    assert(cur != NULL);

    while(cur != NULL){
        printMove(cur -> mv);

        if(cur -> mv != move1 && cur -> mv != move2 && cur -> mv != move3){
           assert(0); 
        }

        cur = cur -> next;
    }

    freeMoveList(ml);

}
