#include <iostream>
#include <stdint.h>
#include <cstdio>
#include <string.h>

#define notAFile  0xfefefefefefefefe;
#define notHFile  0x7f7f7f7f7f7f7f7f;
#define notAHFile 0x7e7e7e7e7e7e7e7e;

#define babMask 0xff818181818181ff;

#define aFile     0x0101010101010101;

using namespace std;

enum Piece { Pawn=0, Rook, Knight, Bishop, Queen, King };

uint64_t testFunct (){ return 3; };

uint64_t soutMask[64];
uint64_t nortMask[64];
uint64_t eastMask[64];
uint64_t westMask[64];
uint64_t diagMask[64];
uint64_t adiaMask[64];
uint64_t fromToMask[64][64];
uint64_t indexMask[64];

uint64_t wp = 0x000000000000ff00;
uint64_t wr = 0x0000000000000081;
uint64_t wn = 0x0000000000000042;
uint64_t wh = 0x0000000000000024;
uint64_t wq = 0x0000000000000010;
uint64_t wk = 0x0000000000000008;
uint64_t bp = 0x00ff000000000000;
uint64_t br = 0x8100000000000000;
uint64_t bn = 0x4200000000000000;
uint64_t bh = 0x2400000000000000;
uint64_t bk = 0x1000000000000000;
uint64_t bq = 0x0800000000000000;

enum rank { rank1, rank2, rank3, rank4, rank5, rank6, rank7, rank8 };
enum file { fileA, fileB, fileC, fileD, fileE, fileF, fileG, fileH };

uint64_t nortOne( uint64_t bb ) { return bb << 8; }
uint64_t soutOne( uint64_t bb ) { return bb >> 8; }
uint64_t eastOne( uint64_t bb ) { return ( bb << 1 ) & notAFile; }
uint64_t westOne( uint64_t bb ) { return ( bb >> 1 ) & notHFile; }
uint64_t neasOne( uint64_t bb ) { return ( bb << 9 ) & notAFile; }
uint64_t nwesOne( uint64_t bb ) { return ( bb << 7 ) & notHFile; }
uint64_t seasOne( uint64_t bb ) { return ( bb >> 7 ) & notAFile; }
uint64_t swesOne( uint64_t bb ) { return ( bb >> 9 ) & notHFile; }

uint64_t black( ) { return bp | br | bn | bh | bk | bq; }
uint64_t white( ) { return wp | wr | wn | wh | wk | wq; }

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

void initIndexBoard(){
   uint64_t bb = 1;
   for( int i = 0; i < 64; i++){
     indexMask[i] = bb; 
     bb <<= 1;
   }
}

void initSoutMask( ){
   uint64_t mask = 0x0080808080808080;
   int i;
   for(i = 63; i > -1; i--){
      soutMask[i] = mask;
      mask >>= 1;
   }
}

void initNortMask( ){
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
      for(bb = neasOne(indexMask[i]); bb != 0; bb = neasOne(bb)){
         diagMask[i] |= bb;
      }

      for(bb = swesOne(indexMask[i]); bb != 0; bb = swesOne(bb)){
         diagMask[i] |= bb;
      }
   }
}

void initAdiaMask(){
   uint64_t bb;

   for(int i = 0; i < 64; i++){
      for(bb = nwesOne(indexMask[i]); bb != 0; bb = nwesOne(bb)){
         adiaMask[i] |= bb;
      }

      for(bb = seasOne(indexMask[i]); bb != 0; bb = seasOne(bb)){
         adiaMask[i] |= bb;
      }
   }
}

void initKngtMask(){
   
}

void initFromTo(){
   for(int i = 0; i < 64; i++){
      uint64_t bb = indexMask[i];

      for(int j = i + 8; j < 64; j += 8){
         bb |= nortOne(bb);
         fromToMask[i][j] = bb ^ indexMask[i];
      }

      bb = indexMask[i];

      for(int j = i - 8; j > -1; j -= 8){
         bb |= soutOne(bb);
         fromToMask[i][j] = bb ^ indexMask[i];
      }

      bb = indexMask[i];
      
      int eastBound = (i/8) * 8 + 8;
      for(int j = i + 1; j < eastBound; j += 1){
         bb |= eastOne(bb);
         fromToMask[i][j] = bb ^ indexMask[i];
      }

      bb = indexMask[i];

      int westBound = (i/8) * 8 - 1;
      for(int j = i - 1; j > westBound; j -= 1){
         bb |= westOne(bb);
         fromToMask[i][j] = bb ^ indexMask[i];
      }

      bb = indexMask[i];

      int j = i + 9;
      while(j < 64 && neasOne(indexMask[j-9])){
         bb |= neasOne(bb);
         fromToMask[i][j] = bb ^ indexMask[i];
         j += 9;
      }

      bb = indexMask[i];

      j = i - 9;
      while(j > 0 && swesOne(indexMask[j + 9])){
         bb |= swesOne(bb);
         fromToMask[i][j] = bb ^ indexMask[i];
         j -= 9;
      }

      bb = indexMask[i];

      j = i + 7;
      while(j > 0 && nwesOne(indexMask[j - 7])){
         bb |= nwesOne(bb);
         fromToMask[i][j] = bb ^ indexMask[i];
         j += 7;
      }

      bb = indexMask[i];

      j = i - 7;
      while(j > 0 && seasOne(indexMask[j + 7])){
         bb |= seasOne(bb);
         fromToMask[i][j] = bb ^ indexMask[i];
         j -= 7;
      }
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
   initFromTo();
}

uint64_t pieceAttack( Piece p, uint8_t sq ) { 
   if( p == Rook){
      return westMask[sq] | eastMask[sq] | nortMask[sq] | soutMask[sq];
   }else if( p = Bishop ){
      return diagMask[sq] | adiaMask[sq];
   }else if( p = Queen ){
      return pieceAttack( Rook, sq ) | pieceAttack( Bishop, sq );
   }else{
      return 0;
   }
}


int main(){
   initMasks();

   /**
   printf("From: %i", i);
   printf(" To: %i\n", j);
   printBoard(fromToMask[i][j]);
   printf("--------\n");
   */

   for(int i = 0; i < 64; i++){
      printf("Index: %i\n", i);
      printBoard(adiaMask[i]);
      printf("-------\n");
   }
   

   return 0;
}
