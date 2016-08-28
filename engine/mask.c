#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "mask.h"
#include "board.h"
#include "move.h"
#include "config.h"

uint64_t soutMask[64];
uint64_t nortMask[64];
uint64_t eastMask[64];
uint64_t westMask[64];
uint64_t diagMask[64];
uint64_t adiaMask[64];

uint64_t piecMask[6][64];
uint64_t blBeMask[6][64];

uint64_t indxMask[64];
uint64_t fromToMask[64][64];


void initIndexBoard(){
   uint64_t bb = 1;
   for( int i = 0; i < 64; i++){
     indxMask[i] = bb; 
     bb <<= 1;
   }
}

void initSoutMask(){
   uint64_t mask = 0x0080808080808080;
   int i;
   for(i = 63; i > -1; i--){
      soutMask[i] = mask;
      mask >>= 1;
   }
}

void initNortMask(){
   uint64_t mask = 0x0101010101010100;
   int i;
   for(i = 0; i < 64; i++){
      nortMask[i] = mask;
      mask <<= 1;
   }
}

void initEastMask(){
   uint64_t eastRay = 0x00000000000000fe;
   uint64_t loopRay;
   int i; 
   int j;
   for( int i = 0; i < 8; i++){
      loopRay = eastRay;
      for( int j = i; j < 64; j+=8){
         eastMask[j] = loopRay; 
         loopRay = nortOne(loopRay);
      }
      eastRay = eastOne(eastRay);
   }
}

void initWestMask(){
   uint64_t westRay = 0x000000000000007f;
   uint64_t loopRay;
   int i; 
   int j;
   for( int i = 7; i > -1; i--){
      loopRay = westRay;
      for( int j = i; j < 64; j+=8){
         westMask[j] = loopRay; 
         loopRay = nortOne(loopRay);
      }
      westRay = westOne(westRay);
   }
}

void initDiagMask(){
   uint64_t bb;

   for(int i = 0; i < 64; i++){
      for(bb = neasOne(indxMask[i]); bb != 0; bb = neasOne(bb)){
         diagMask[i] |= bb;
      }

      for(bb = swesOne(indxMask[i]); bb != 0; bb = swesOne(bb)){
         diagMask[i] |= bb;
      }
   }
}

void initAdiaMask(){
   uint64_t bb;

   for(int i = 0; i < 64; i++){
      for(bb = nwesOne(indxMask[i]); bb != 0; bb = nwesOne(bb)){
         adiaMask[i] |= bb;
      }

      for(bb = seasOne(indxMask[i]); bb != 0; bb = seasOne(bb)){
         adiaMask[i] |= bb;
      }
   }
}


void initKngtMask(){
   
}

void initFromTo(){
   for(int i = 0; i < 64; i++){
      uint64_t bb = indxMask[i];

      for(int j = i + 8; j < 64; j += 8){
         bb |= nortOne(bb);
         fromToMask[i][j] = bb ^ indxMask[i];
      }

      bb = indxMask[i];

      for(int j = i - 8; j > -1; j -= 8){
         bb |= soutOne(bb);
         fromToMask[i][j] = bb ^ indxMask[i];
      }

      bb = indxMask[i];
      
      int eastBound = (i/8) * 8 + 8;
      for(int j = i + 1; j < eastBound; j += 1){
         bb |= eastOne(bb);
         fromToMask[i][j] = bb ^ indxMask[i];
      }

      bb = indxMask[i];

      int westBound = (i/8) * 8 - 1;
      for(int j = i - 1; j > westBound; j -= 1){
         bb |= westOne(bb);
         fromToMask[i][j] = bb ^ indxMask[i];
      }

      bb = indxMask[i];

      int j = i + 9;
      while(j < 64 && neasOne(indxMask[j-9])){
         bb |= neasOne(bb);
         fromToMask[i][j] = bb ^ indxMask[i];
         j += 9;
      }

      bb = indxMask[i];

      j = i - 9;
      while(j > 0 && swesOne(indxMask[j + 9])){
         bb |= swesOne(bb);
         fromToMask[i][j] = bb ^ indxMask[i];
         j -= 9;
      }

      bb = indxMask[i];

      j = i + 7;
      while(j > 0 && nwesOne(indxMask[j - 7])){
         bb |= nwesOne(bb);
         fromToMask[i][j] = bb ^ indxMask[i];
         j += 7;
      }

      bb = indxMask[i];

      j = i - 7;
      while(j > 0 && seasOne(indxMask[j + 7])){
         bb |= seasOne(bb);
         fromToMask[i][j] = bb ^ indxMask[i];
         j -= 7;
      }
   }
}

void initPieceMask() { 
    for(int i = 0; i < 64; i++ ){
        piecMask[PAWN][i] = 0;
        piecMask[ROOK][i] = westMask[i] | eastMask[i] | nortMask[i] | soutMask[i];
        piecMask[BISHOP][i] = diagMask[i] | adiaMask[i];
        piecMask[KNIGHT][i] = 0;
        piecMask[QUEEN][i] = piecMask[ROOK][i] | piecMask[BISHOP][i];
        piecMask[KING][i] = 0;
    }
}

void initBlockersAndBeyond(){
    // TODO could fix bab to be only [3][64]
    for(int i = 0; i < 64; i++ ){
        blBeMask[BISHOP][i] = piecMask[BISHOP][i] & babMask;
        blBeMask[ROOK][i] = (nortMask[i] & notRank8) | (eastMask[i] & notHFile) | (westMask[i] & notAFile) | (soutMask[i] & notRank1);
        blBeMask[QUEEN][i] = blBeMask[ROOK][i] | blBeMask[BISHOP][i];
    }
}

void initMasks(){
   initIndexBoard();
   initSoutMask();
   initNortMask();
   initEastMask();
   initWestMask();
   initDiagMask();
   initAdiaMask();
   initPieceMask();
   initBlockersAndBeyond();
   initFromTo();
}
