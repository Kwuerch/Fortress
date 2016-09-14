#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define notAFile  0xfefefefefefefefe
#define notHFile  0x7f7f7f7f7f7f7f7f
#define notGHFile 0x3f3f3f3f3f3f3f3f
#define notABFile 0xfefefefefefefefe
#define notRank8  0x00ffffffffffffff
#define notRank1  0xffffffffffffff00
#define notAHFile 0x7e7e7e7e7e7e7e7e

#define aFile   0x0101010101010101
#define babMask 0x007e7e7e7e7e7e00

typedef enum { WHITE = 0, BLACK } Color;
// Queen, Rook, and Pawn come first to decrease size of blBeMask 
typedef enum { QUEEN = 0, BISHOP, ROOK, PAWN, KNIGHT, KING } Piece;
typedef enum { RANK1 = 0, RANK2, RANK3, RANK4, RANK5, RANK6, RANK7, RANK8 } Rank;
typedef enum { FILEA = 0, FILEB, FILEC, FILED, FILEE, FILEF, FILEG, FILEH } File;
typedef enum { QUIET = 0, DOUBLE_PAWN, KING_CASTLE, QUEEN_CASTLE, CAPTURE, EP_CAPTURE, 
               KNIGHT_PROMO, BISHOP_PROMO, ROOK_PROMO, QUEEN_PROMO,
               KNIGHT_PROMO_CAP, BISHOP_PROMO_CAP, ROOK_PROMO_CAP, QUEEN_PROMO_CAP } MoveFlag;
#endif
