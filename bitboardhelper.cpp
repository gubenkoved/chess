#include "bitboardhelper.h"
#include "lightfigureposition.h"

PositionCollection BitBoardHelper::GetPositions(BITBOARD bitboard)
{
    PositionCollection positions;

    for(int idx = 0; i < 64; ++i)
    {
        if (bitboard && (1 << idx))
        {
            positions.append(FromSerial(idx));
        }
    }

    return positions;
}

BITBOARD BitBoardHelper::FromPosition(POSITION p)
{
    return 1 << PositionHelper::Serial(p);
}

BITBOARD BitBoardHelper::AddPosition(BITBOARD bitboard, POSITION p)
{
    return bitboard | (1 << PositionHelper::Serial(p));
}

