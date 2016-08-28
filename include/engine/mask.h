#ifndef MASK_H_INCLUDED
#define MASK_H_INCLUDED

#include <stdint.h>

extern uint64_t soutMask[64];
extern uint64_t nortMask[64];
extern uint64_t eastMask[64];
extern uint64_t westMask[64];
extern uint64_t diagMask[64];
extern uint64_t adiaMask[64];

extern uint64_t piecMask[6][64];
extern uint64_t blBeMask[6][64];

extern uint64_t indxMask[64];
extern uint64_t fromToMask[64][64];

void initIndexBoard();
void initSoutMask();
void initNortMask();
void initEastMask();
void initWestMask();
void initDiagMask();
void initAdiaMask();
void initKngtMask();
void initFromTo();
void initMasks();
void initPieceMask();
void initBlockersAndBeyond();

#endif
