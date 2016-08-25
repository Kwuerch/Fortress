#include <iostream>
#include <stdint.h>
#include <cstdio>
uint64_t db = 0x03f79d71b4cb0a89;
int main(){
   for(uint64_t i = 1; i != 0; i <<= 1){
      printf("%x\n", int((db * i) >> 58));
   }
   return 0;
}


