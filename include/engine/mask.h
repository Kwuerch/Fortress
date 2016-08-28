#ifndef MASK_H_INCLUDED
#define MASK_H_INCLUDED

#include <stdint.h>

typedef struct chess_mask chess_mask;
struct chess_mask{
    uint64_t soutMask[64];
    uint64_t nortMask[64];
    uint64_t eastMask[64];
    uint64_t westMask[64];
    uint64_t diagMask[64];
    uint64_t adiaMask[64];

    uint64_t piecMask[6][64];
    uint64_t blBeMask[6][64];

    uint64_t indxMask[64];
    uint64_t fromToMask[64][64];
};

void initIndexBoard( chess_mask * cm );
void initSoutMask( chess_mask * cm );
void initNortMask( chess_mask * cm );
void initEastMask( chess_mask * cm );
void initWestMask( chess_mask * cm );
void initDiagMask( chess_mask * cm );
void initAdiaMask( chess_mask * cm );
void initKngtMask( chess_mask * cm );
void initFromTo( chess_mask * cm );
void initMasks( chess_mask * cm );
void initPieceMask( chess_mask * cm );
void initBlockersAndBeyond( chess_mask * cm );

#endif
