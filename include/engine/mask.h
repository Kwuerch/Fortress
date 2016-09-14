#ifndef MASK_H_INCLUDED
#define MASK_H_INCLUDED

#include <stdint.h>

extern uint64_t soutMask[64];
extern uint64_t nortMask[64];
extern uint64_t eastMask[64];
extern uint64_t westMask[64];
extern uint64_t nwesMask[64];
extern uint64_t neasMask[64];
extern uint64_t swesMask[64];
extern uint64_t seasMask[64];
extern uint64_t diagMask[64];
extern uint64_t adiaMask[64];

extern uint64_t piecMask[6][64];
extern uint64_t blBeMask[3][64];

extern uint64_t indxMask[64];
extern uint64_t fromToMask[64][64];
extern uint64_t shadMask[64][64];

void initIndexBoard();
void initSoutMask();
void initNortMask();
void initEastMask();
void initWestMask();
void initNeasMask();
void initNwesMask();
void initSeasMask();
void initSwesMask();
void initDiagMask();
void initAdiaMask();
void initKngtMask();
void initFromTo();
void initShadowMask();
void initPieceMask();
void initBlockersAndBeyond();
void initMasks();

#endif
