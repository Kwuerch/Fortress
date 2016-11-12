#include <stdlib.h>
#include "moveList.h"
#include "move.h"

#include <stdio.h>

void addMove(moveList* ml, Move mv){
   if(ml -> tail != NULL){
      
      // Allocate memory for next element
      ml -> tail -> next = malloc(sizeof(moveListNode));
      ml -> tail -> next -> mv = mv;
      ml -> tail -> next -> next = NULL;

      // Readjust tail
      ml -> tail = ml -> tail -> next; 
   }else{
      // Allocate head and tail and add move
      ml -> head = malloc(sizeof(moveListNode));
      ml -> tail =  ml -> head;

      ml -> head -> mv = mv;
      ml -> head -> next = NULL;
   }
}


void printMoves(moveList* ml){
    moveListNode* cur = ml -> head;

    while(cur != NULL){
        printMove(cur -> mv);
        cur = cur -> next;
    }
}

void freeMoveList(moveList* ml){
    if(ml -> head != NULL){
        moveListNode* cur = ml -> head;

        moveListNode* next; 

        do{
            next = cur -> next;
            free(cur);
            cur = next;
        }while(cur != NULL);
    }

    free(ml);
}
