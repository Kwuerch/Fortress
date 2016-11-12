#ifndef MOVESTACK_H_INCLUDED
#define MOVESTACK_H_INCLUDED

#include <stdint.h>
#include "config.h"

typedef uint16_t Move;

typedef struct stackMove{
   Move mv;
   Piece cap; //Captured piece, NULL represents no captured piece
   Color clr;
}stackMove;

typedef struct moveStackNode moveStackNode;
struct moveStackNode{
   moveStackNode* next;
   stackMove sm;
};

typedef struct moveStack {
   moveStackNode* head;
}moveStack;


void push(moveStack* ms, stackMove sm);
stackMove pop();
void freeMoveStack(moveStack *ms);
void printStackMove(stackMove sm);
void printMoveStack(moveStack *ms);

#endif
