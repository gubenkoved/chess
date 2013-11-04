#include "bitboardhelper.h"

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
