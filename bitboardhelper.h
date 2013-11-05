#ifndef BITBOARDHELPER_H
#define BITBOARDHELPER_H

#include "typedefs.h"
#include "lightfigureposition.h"

#define BITBOARD_EMPTY 0ULL

class BitboardHelper
{
public:
    static inline PositionCollection GetPositions(BITBOARD bitboard);

    static inline BITBOARD AddPosition(BITBOARD bitboard, POSITION p);
    static inline BITBOARD AddPositionWhenValid(BITBOARD bitboard, POSITION p);

    static inline BITBOARD Union(BITBOARD bitboard1, BITBOARD bitboard2);

    static inline bool Contains(BITBOARD bitboard, POSITION position);
};

PositionCollection BitboardHelper::GetPositions(BITBOARD bitboard)
{
    PositionCollection positions;

    for(int idx = 0; idx < 64; ++idx)
    {
        if (bitboard & (1ULL << idx))
        {
            positions.append(PositionHelper::FromSerial(idx));
        }
    }

    return positions;
}

BITBOARD BitboardHelper::AddPosition(BITBOARD bitboard, POSITION p)
{
    return bitboard | (1ULL << PositionHelper::Serial(p));
}

BITBOARD BitboardHelper::AddPositionWhenValid(BITBOARD bitboard, POSITION p)
{
    if (!PositionHelper::IsInvalid(p))
    {
        return bitboard | (1ULL << PositionHelper::Serial(p));
    } else
    {
        return bitboard;
    }
}

BITBOARD BitboardHelper::Union(BITBOARD bitboard1, BITBOARD bitboard2)
{
    return bitboard1 | bitboard2;
}

bool BitboardHelper::Contains(BITBOARD bitboard, POSITION p)
{
    return bitboard & (1ULL << PositionHelper::Serial(p));
}

#endif // BITBOARDHELPER_H
