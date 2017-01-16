#include <stdlib.h>
#include "moveStack.h"
#include "move.h"

#include <stdio.h>

moveStack *moveStackNew(){
    return calloc(1, sizeof(moveStack));
}

void push(moveStack* ms, stackMove *sm){

    if(ms -> head == NULL){
        ms -> head = calloc(1, sizeof(moveStackNode));

        ms -> head -> sm = *sm;
        ms -> head -> next = NULL;
    }else{
        moveStackNode *top = ms -> head;
        ms -> head = calloc(1, sizeof(moveStackNode));

        ms -> head -> sm = *sm;
        ms -> head -> next = top;
    }

}

stackMove *stackMoveNew(){
    return malloc(sizeof(stackMove));
}

// Assuming ms is not null
stackMove pop(moveStack* ms){
    moveStackNode *temp = ms -> head;
    ms -> head = ms -> head -> next;

    stackMove sm = temp -> sm;
    free(temp);

    return sm;
}

// Precondition moveStack* is not Null
// moveStack should contain no elements inside if used correctly
void freeMoveStack(moveStack* ms){
    if(ms -> head != NULL){
        moveStackNode* cur = ms -> head;

        moveStackNode* next; 

        do{
            next = cur -> next;
            free(cur);
            cur = next;
        }while(cur != NULL);

    }

    free(ms);
}

void printMoveStack(moveStack* ms){
    moveStackNode* cur = ms -> head;

    while(cur != NULL){
        printStackMove(cur -> sm);
        cur = cur -> next;
    }
}

void printStackMove(stackMove sm){
    printf("Color:%i\t", sm.clr);
    printf("From:%i\t", getFrom(sm.mv));
    printf("To:%i\t", getTo(sm.mv));
    printf("Flags:%i", getMoveFlags(sm.mv));
}
