#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "mask.h"
#include "board.h"
#include "move.h"
#include "config.h"



void initIndexBoard( chess_mask * cm ){
   uint64_t bb = 1;
   for( int i = 0; i < 64; i++){
     cm -> indxMask[i] = bb; 
     bb <<= 1;
   }
}

void initSoutMask( chess_mask * cm ){
   uint64_t mask = 0x0080808080808080;
   int i;
   for(i = 63; i > -1; i--){
      cm -> soutMask[i] = mask;
      mask >>= 1;
   }
}

void initNortMask( chess_mask * cm ){
   uint64_t mask = 0x0101010101010100;
   int i;
   for(i = 0; i < 64; i++){
      cm -> nortMask[i] = mask;
      mask <<= 1;
   }
}

void initEastMask( chess_mask * cm ){
   uint64_t eastRay = 0x00000000000000fe;
   uint64_t loopRay;
   int i; 
   int j;
   for( int i = 0; i < 8; i++){
      loopRay = eastRay;
      for( int j = i; j < 64; j+=8){
         cm -> eastMask[j] = loopRay; 
         loopRay = nortOne(loopRay);
      }
      eastRay = eastOne(eastRay);
   }
}

void initWestMask( chess_mask * cm ){
   uint64_t westRay = 0x000000000000007f;
   uint64_t loopRay;
   int i; 
   int j;
   for( int i = 7; i > -1; i--){
      loopRay = westRay;
      for( int j = i; j < 64; j+=8){
         cm -> westMask[j] = loopRay; 
         loopRay = nortOne(loopRay);
      }
      westRay = westOne(westRay);
   }
}

void initDiagMask( chess_mask * cm ){
   uint64_t bb;

   for(int i = 0; i < 64; i++){
      for(bb = neasOne(cm -> indxMask[i]); bb != 0; bb = neasOne(bb)){
         cm -> diagMask[i] |= bb;
      }

      for(bb = swesOne(cm -> indxMask[i]); bb != 0; bb = swesOne(bb)){
         cm -> diagMask[i] |= bb;
      }
   }
}

void initAdiaMask( chess_mask * cm ){
   uint64_t bb;

   for(int i = 0; i < 64; i++){
      for(bb = nwesOne(cm -> indxMask[i]); bb != 0; bb = nwesOne(bb)){
         cm -> adiaMask[i] |= bb;
      }

      for(bb = seasOne(cm -> indxMask[i]); bb != 0; bb = seasOne(bb)){
         cm -> adiaMask[i] |= bb;
      }
   }
}


void initKngtMask( chess_mask * cm ){
   
}

void initFromTo( chess_mask * cm ){
   for(int i = 0; i < 64; i++){
      uint64_t bb = cm -> indxMask[i];

      for(int j = i + 8; j < 64; j += 8){
         bb |= nortOne(bb);
         cm -> fromToMask[i][j] = bb ^ cm -> indxMask[i];
      }

      bb = cm -> indxMask[i];

      for(int j = i - 8; j > -1; j -= 8){
         bb |= soutOne(bb);
         cm -> fromToMask[i][j] = bb ^ cm -> indxMask[i];
      }

      bb = cm -> indxMask[i];
      
      int eastBound = (i/8) * 8 + 8;
      for(int j = i + 1; j < eastBound; j += 1){
         bb |= eastOne(bb);
         cm -> fromToMask[i][j] = bb ^ cm -> indxMask[i];
      }

      bb = cm -> indxMask[i];

      int westBound = (i/8) * 8 - 1;
      for(int j = i - 1; j > westBound; j -= 1){
         bb |= westOne(bb);
         cm -> fromToMask[i][j] = bb ^ cm -> indxMask[i];
      }

      bb = cm -> indxMask[i];

      int j = i + 9;
      while(j < 64 && neasOne(cm -> indxMask[j-9])){
         bb |= neasOne(bb);
         cm -> fromToMask[i][j] = bb ^ cm -> indxMask[i];
         j += 9;
      }

      bb = cm -> indxMask[i];

      j = i - 9;
      while(j > 0 && swesOne(cm -> indxMask[j + 9])){
         bb |= swesOne(bb);
         cm -> fromToMask[i][j] = bb ^ cm -> indxMask[i];
         j -= 9;
      }

      bb = cm -> indxMask[i];

      j = i + 7;
      while(j > 0 && nwesOne(cm -> indxMask[j - 7])){
         bb |= nwesOne(bb);
         cm -> fromToMask[i][j] = bb ^ cm -> indxMask[i];
         j += 7;
      }

      bb = cm -> indxMask[i];

      j = i - 7;
      while(j > 0 && seasOne(cm -> indxMask[j + 7])){
         bb |= seasOne(bb);
         cm -> fromToMask[i][j] = bb ^ cm -> indxMask[i];
         j -= 7;
      }
   }
}

void initPieceMask(  chess_mask * cm  ) { 
    for(int i = 0; i < 64; i++ ){
        cm -> rookMask[i] = cm -> westMask[i] | cm -> eastMask[i] | cm -> nortMask[i] | cm -> soutMask[i];
        cm -> bishMask[i] = cm -> diagMask[i] | cm -> adiaMask[i];
        cm -> quenMask[i] = cm -> bishMask[i] | cm -> rookMask[i];
    }
}

void initMasks( chess_mask * cm ){
   initIndexBoard( cm );
   initSoutMask( cm );
   initNortMask( cm );
   initEastMask( cm );
   initWestMask( cm );
   initDiagMask( cm );
   initAdiaMask( cm );
   initPieceMask( cm );
   initFromTo( cm );
}
