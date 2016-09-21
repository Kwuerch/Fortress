#ifndef MOVELIST_H_INCLUDED
#define MOVELIST_H_INCLUDED

#include <stdint.h>

typedef uint16_t Move;

typedef struct moveListNode moveListNode;
struct moveListNode {
   moveListNode* next;
   Move mv;
};

typedef struct moveList {
   moveListNode* head;
   moveListNode* tail;
}moveList;

void addMove(moveList* ml, Move mv);
void printMoves(moveList* ml);

#endif
