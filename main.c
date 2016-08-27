#include <stdio.h>

#include "mask.h"
#include "board.h"
#include "move.h"

int main(){
    chess_mask cm;
    initMasks( &cm );

    /**
    printf("From: %i", i);
    printf(" To: %i\n", j);
    printBoard(fromToMask[i][j]);
    printf("--------\n");
    */

    for(int i = 0; i < 64; i++){
      printf("Index: %i\n", i);
      printBoard(cm.adiaMask[i]);
      printf("-------\n");
    }

    return 0;
}
