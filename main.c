#include <stdio.h>

#include "config.h"
#include "mask.h"
#include "board.h"
#include "move.h"

int main(){
    initMasks();

    board b;
    initBoard(&b);

    /**
    printf("From: %i", i);
    printf(" To: %i\n", j);
    printBoard(fromToMask[i][j]);
    printf("--------\n");
    */

    printBoard(rayAttacks(black(b) | b.wh, BISHOP, 0));
    printf("Index: %i\n", bitScanForward(0x0040000000000000));

    return 0;
}
