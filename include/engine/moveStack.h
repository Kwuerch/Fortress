#ifndef MOVESTACK_H_INCLUDED
#define MOVESTACK_H_INCLUDED

#include <stdint.h>
#include "config.h"

typedef uint16_t Move;

typedef struct stackMove{
   Move mv;
   Color clr;
   uint64_t *to;   // Pointer to capture or promo piece board
   uint64_t *from; // Pointer to original piece board
}stackMove;

typedef struct moveStackNode moveStackNode;
struct moveStackNode{
   moveStackNode* next;
   stackMove sm;
};

typedef struct moveStack {
   moveStackNode* head;
}moveStack;


moveStack *moveStackNew();
stackMove *stackMoveNew();
void push(moveStack* ms, stackMove *sm);
stackMove pop();
void freeMoveStack(moveStack *ms);
void printStackMove(stackMove sm);
void printMoveStack(moveStack *ms);

#endif
