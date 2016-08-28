#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "config.h"
#include "board.h"

static const uint8_t debruijn[64] = {
    0, 1, 3, 7, 15, 31, 63, 62,
    61, 59, 55, 47, 30, 60, 57, 51,
    39, 14, 29, 58, 53, 43, 23, 46,
    28, 56, 49, 35, 6, 13, 27, 54,
    45, 26, 52, 41, 19, 38, 12, 25,
    50, 37, 11, 22, 44, 24, 48, 33,
    2, 5, 10, 21, 42, 20, 40, 17,
    34, 4, 9, 18, 36, 8, 16, 32
}

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

uint64_t black( board* b ) { return b -> bp | b -> br | b -> bn | b -> bh | b -> bk | b -> bq; }
uint64_t white( board* b ) { return b -> wp | b -> wr | b -> wn | b -> wh | b -> wk | b -> wq; }

uint8_t bitScanForward( uint64_t * bb){
    return debruijn[(bb * DEBRUIJN_CONST) >> 58];
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
