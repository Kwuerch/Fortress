#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "config.h"
#include "board.h"

static const uint8_t debruijn[64] = {
    0, 1, 48, 2, 57, 49, 28, 3,
    61, 58, 50, 42, 38, 29, 17, 4,
    62, 55, 59, 36, 53, 51, 43, 22,
    45, 39, 33, 30, 24, 18, 12, 5,
    63, 47, 56, 27, 60, 41, 37, 16,
    54, 35, 52, 21, 44, 32, 23, 11,
    46, 26, 40, 15, 34, 20, 31, 10,
    25, 14, 19, 9, 13, 8, 7, 6
};

void initBoard( board* b){
    b -> wp = 0x000000000000ff00;
    b -> wr = 0x0000000000000081;
    b -> wn = 0x0000000000000042;
    b -> wh = 0x0000000000000024;
    b -> wq = 0x0000000000000010;
    b -> wk = 0x0000000000000008;
    b -> bp = 0x00ff000000000000;
    b -> br = 0x8100000000000000;
    b -> bn = 0x4200000000000000;
    b -> bh = 0x2400000000000000;
    b -> bk = 0x1000000000000000;
    b -> bq = 0x0800000000000000;
}

uint64_t black( board* b ) { return b->bp | b->br | b->bn | b->bh | b->bk | b->bq; }
uint64_t white( board* b ) { return b->wp | b->wr | b->wn | b->wh | b->wk | b->wq; }

uint64_t occupied( board* b ){ return black(b) | white(b); }

// bb cannot be zero
uint8_t bitScanForward( uint64_t bb){
    return debruijn[((bb & (~bb + 1)) * DEBRUIJN_CONST) >> 58];
    // TODO faster return debruijn[((bb ^ (bb - 1)) * DEBRUIJN_CONST) >> 58];
}

const char *byteToBinary( char bt ){
   static char b[9];
   b[0] = '\0';

   int i;
   for( i = 0; i < 8; i++){
      strcat(b , ( bt >> i ) & 1 ? "1" : "0" );
   }

   return b;
}

void printBoard( uint64_t bb ){
   int i;
   for(i = 56; i >= 0; i-=8){
      printf( "%s\n", byteToBinary( (char)( ( bb >> i ) & 0xff) ) );
   }
}
