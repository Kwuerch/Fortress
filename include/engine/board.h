#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#define DEBRUIJN_CONST 0x03f79d71b4cb0a89

typedef struct board board;
struct board{
    uint64_t wp;
    uint64_t wr;
    uint64_t wn;
    uint64_t wh;
    uint64_t wq;
    uint64_t wk;
    uint64_t bp;
    uint64_t br;
    uint64_t bn;
    uint64_t bh;
    uint64_t bk;
    uint64_t bq;
};

void initBoard( board *b);

uint64_t black( board b );
uint64_t white( board b );

uint8_t bitScanForward( uint64_t bb);

const char *byteToBinary( char bt );
void printBoard( uint64_t bb );

#endif
