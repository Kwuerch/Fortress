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
uint64_t nwesMask[64];
uint64_t neasMask[64];
uint64_t swesMask[64];
uint64_t seasMask[64];
uint64_t diagMask[64];
uint64_t adiaMask[64];

uint64_t piecMask[5][64];
uint64_t pawnMask[2][5][64];
uint64_t blBeMask[3][64];

uint64_t indxMask[64];
uint64_t fromToMask[64][64];
uint64_t shadMask[64][64];


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

void initWestMask(){
   uint64_t westRay = 0x00000000000000fe;
   uint64_t loopRay;
   int i; 
   int j;
   for( int i = 0; i < 8; i++){
      loopRay = westRay;
      for( int j = i; j < 64; j+=8){
         westMask[j] = loopRay; 
         loopRay = nortOne(loopRay);
      }
      westRay = westOne(westRay);
   }
}

void initEastMask(){
   uint64_t eastRay = 0x000000000000007f;
   uint64_t loopRay;
   int i; 
   int j;
   for( int i = 7; i > -1; i--){
      loopRay = eastRay;
      for( int j = i; j < 64; j+=8){
         eastMask[j] = loopRay; 
         loopRay = nortOne(loopRay);
      }
      eastRay = eastOne(eastRay);
   }
}

void initNwesMask(){
   for(int i = 0; i < 64; i++){
      for(uint64_t bb = nwesOne(indxMask[i]); bb != 0; bb = nwesOne(bb)){
         nwesMask[i] |= bb;
      }
   }

}

void initNeasMask(){
   for(int i = 0; i < 64; i++){
      for(uint64_t bb = neasOne(indxMask[i]); bb != 0; bb = neasOne(bb)){
         neasMask[i] |= bb;
      }
   }
}

void initSwesMask(){
   for(int i = 0; i < 64; i++){
      for(uint64_t bb = swesOne(indxMask[i]); bb != 0; bb = swesOne(bb)){
         swesMask[i] |= bb;
      }
   }
}

void initSeasMask(){
   for(int i = 0; i < 64; i++){
      for(uint64_t bb = seasOne(indxMask[i]); bb != 0; bb = seasOne(bb)){
         seasMask[i] |= bb;
      }
   }
}

void initDiagMask(){
    for(int i = 0; i < 64; i++){
        diagMask[i] |= neasMask[i] | swesMask[i];
    }
}

void initAdiaMask(){
    for(int i = 0; i < 64; i++){
        adiaMask[i] |= nwesMask[i] | seasMask[i];
    }
}

void initPawnMask(){
    for(int i = 0; i < 64; i++){
        uint64_t index = indxMask[i];

        pawnMask[WHITE][PUSH][i]   |= nortOne(index) & notRank8;
        pawnMask[BLACK][PUSH][i]   |= soutOne(index) & notRank1;

        pawnMask[WHITE][DOUBLE_PUSH][i]   |= nortOne(nortOne(index)) & rank4;
        pawnMask[BLACK][DOUBLE_PUSH][i]   |= soutOne(soutOne(index)) & rank5;

        pawnMask[WHITE][CAP][i] |= (neasOne(index) | nwesOne(index)) & notRank8;
        pawnMask[BLACK][CAP][i] |= (soutOne(index) | soutOne(index)) & notRank1;

        pawnMask[WHITE][PROMO_CAP][i] |= (neasOne(index) | nwesOne(index)) & rank8;
        pawnMask[BLACK][PROMO_CAP][i] |= (soutOne(index) | soutOne(index)) & rank1;

        pawnMask[WHITE][PROMO][i]  |= nortOne(index) & rank8;
        pawnMask[BLACK][PROMO][i]  |= soutOne(index) & rank1;
    }
}

void initKngtMask(){
    for(int i = 0; i < 64; i++){
        uint64_t index = indxMask[i];
        piecMask[KNIGHT][i] |= ((index <<  6) | (index >> 10)) & notGHFile;
        piecMask[KNIGHT][i] |= ((index << 15) | (index >> 17)) & notHFile;
        piecMask[KNIGHT][i] |= ((index << 17) | (index >> 15)) & notAFile;
        piecMask[KNIGHT][i] |= ((index << 10) | (index >>  6)) & notABFile;
    }
}

void initKingMask(){
    for(int i = 0; i < 64; i++){
        uint64_t index = indxMask[i];
        piecMask[KING][i] |= ((index << 7) | (index >> 1) | (index >> 9)) & notHFile;
        piecMask[KING][i] |= ((index >> 7) | (index << 1) | (index << 9)) & notAFile;
        piecMask[KING][i] |= (index << 8) | (index >> 8);
    }
}

void initFromTo(){
   for(int i = 0; i < 64; i++){

      uint64_t bb = indxMask[i];
      for(int j = i + 16; j < 64; j += 8){
         bb |= nortOne(bb);
         fromToMask[i][j] = bb ^ indxMask[i];
      }

      bb = indxMask[i];
      for(int j = i - 16; j > -1; j -= 8){
         bb |= soutOne(bb);
         fromToMask[i][j] = bb ^ indxMask[i];
      }

      bb = indxMask[i];
      int westBound = (i/8) * 8 + 8;
      for(int j = i + 2; j < westBound; j += 1){
         bb |= westOne(bb);
         fromToMask[i][j] = bb ^ indxMask[i];
      }

      bb = indxMask[i];
      int eastBound = (i/8) * 8 - 1;
      for(int j = i - 2; j > eastBound; j -= 1){
         bb |= eastOne(bb);
         fromToMask[i][j] = bb ^ indxMask[i];
      }

      bb = indxMask[i];
      int j = i + 18;
      while(j < 64 && nwesOne(indxMask[j-9])){
         bb |= nwesOne(bb);
         fromToMask[i][j] = bb ^ indxMask[i];
         j += 9;
      }

      bb = indxMask[i];
      j = i - 18;
      while(j > 0 && seasOne(indxMask[j + 9])){
         bb |= seasOne(bb);
         fromToMask[i][j] = bb ^ indxMask[i];
         j -= 9;
      }

      bb = indxMask[i];
      j = i + 14;
      while(j > 0 && neasOne(indxMask[j - 7])){
         bb |= neasOne(bb);
         fromToMask[i][j] = bb ^ indxMask[i];
         j += 7;
      }

      bb = indxMask[i];
      j = i - 14;
      while(j > 0 && swesOne(indxMask[j + 7])){
         bb |= swesOne(bb);
         fromToMask[i][j] = bb ^ indxMask[i];
         j -= 7;
      }
   }
}

void initShadowMask(){
   for(int i = 0; i < 64; i++){

      uint64_t bb = nortMask[i];
      for(int j = i + 8; j < 64; j += 8){
         bb = nortOne(bb);
         shadMask[i][j] = bb;
      }

      bb = soutMask[i];
      for(int j = i - 8; j > -1; j -= 8){
         bb = soutOne(bb);
         shadMask[i][j] = bb;
      }

      bb = westMask[i];
      int westBound = (i/8) * 8 + 8;
      for(int j = i + 1; j < westBound; j += 1){
         bb &= westOne(bb);
         shadMask[i][j] = bb;
      }

      bb = eastMask[i];
      int eastBound = (i/8) * 8 - 1;
      for(int j = i - 1; j > eastBound; j -= 1){
         bb &= eastOne(bb);
         shadMask[i][j] = bb;
      }

      bb = nwesMask[i];
      int j = i + 9;
      while(j < 64 && nwesOne(indxMask[j-9])){
         bb &= nwesOne(bb);
         shadMask[i][j] = bb;
         j += 9;
      }

      bb = seasMask[i];
      j = i - 9;
      while(j > 0 && seasOne(indxMask[j + 9])){
         bb &= seasOne(bb);
         shadMask[i][j] = bb;
         j -= 9;
      }

      bb = neasMask[i];
      j = i + 7;
      while(j > 0 && neasOne(indxMask[j - 7])){
         bb &= neasOne(bb);
         shadMask[i][j] = bb;
         j += 7;
      }

      bb = swesMask[i];
      j = i - 7;
      while(j > 0 && swesOne(indxMask[j + 7])){
         bb &= swesOne(bb);
         shadMask[i][j] = bb;
         j -= 7;
      }
   }
}


/** Mask for pseudo-legal possible moves ** ONLY ATTACK MOVES FOR PAWN **/
void initPieceMask() { 
    for(int i = 0; i < 64; i++ ){
        piecMask[ROOK][i] = westMask[i] | eastMask[i] | nortMask[i] | soutMask[i];
        piecMask[BISHOP][i] = diagMask[i] | adiaMask[i];
        piecMask[QUEEN][i] = piecMask[ROOK][i] | piecMask[BISHOP][i];
    }

    initPawnMask();
    initKngtMask();
    initKingMask();
}

void initBlockersAndBeyond(){
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
   initNeasMask();
   initSwesMask();
   initNwesMask();
   initSeasMask();
   initDiagMask();
   initAdiaMask();
   initPieceMask();
   initBlockersAndBeyond();
   initFromTo();
   initShadowMask();
}
