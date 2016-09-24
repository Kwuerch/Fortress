#include <stdio.h>

#include "config.h"
#include "mask.h"
#include "board.h"
#include "move.h"
#include "test.h"

int main(){
    initMasks();

    /**
    printf("From: %i", i);
    printf(" To: %i\n", j);
    printBoard(fromToMask[i][j]);
    printf("--------\n");
    printBoard(shadMask[0][32]);
    printf("--------\n");
    printBoard(shadMask[17][21]);
    printf("--------\n");
    printBoard(shadMask[17][33]);
    printf("--------\n");
    printBoard(shadMask[33][17]);
    printf("--------\n");
    printBoard(shadMask[27][45]);
    printf("--------\n");
    printBoard(shadMask[25][11]);
    printf("--------\n");
    printBoard(shadMask[13][27]);
    printf("--------\n");
    printBoard(shadMask[38][20]);
    printf("--------\n");
    */


    //printBoard((black(&b) | b.wr) & blBeMask[ROOK][0]);
    //printBoard(rayAttacks(0x0101010000000001, ROOK, 0));
    //printBoard(piecMask[KNIGHT][27]);
    //printBoard(piecMask[KNIGHT][0]);
    //printBoard(piecMask[KING][0]);
    //printBoard(piecMask[KING][27]);
    //Move m = createMove(63, 12, 3);
    //printMove(m);
    
    testMoveGen();

    return 0;
}
