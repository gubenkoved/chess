#ifndef BITBOARDHELPER_H
#define BITBOARDHELPER_H

#include "typedefs.h"

struct BitBoardHelper
{
    static PositionCollection GetPositions(BITBOARD bitboard);

    static BITBOARD FromPosition(POSITION p);

    static BITBOARD AddPosition(BITBOARD bitboard, POSITION p);
};

#endif // BITBOARDHELPER_H
