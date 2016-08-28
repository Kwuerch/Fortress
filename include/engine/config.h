#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define notAFile  0xfefefefefefefefe
#define notHFile  0x7f7f7f7f7f7f7f7f
#define notRank8  0x00ffffffffffffff
#define notRank1  0xffffffffffffff00
#define notAHFile 0x7e7e7e7e7e7e7e7e

#define aFile   0x0101010101010101
#define babMask 0x007e7e7e7e7e7e00

typedef enum { PAWN = 0, ROOK, KNIGHT, BISHOP, QUEEN, KING } Piece;
typedef enum { RANK1 = 0, RANK2, RANK3, RANK4, RANK5, RANK6, RANK7, RANK8 } Rank;
typedef enum { FILEA = 0, FILEB, FILEC, FILED, FILEE, FILEF, FILEG, FILEH } File;

#endif
