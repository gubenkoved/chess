#ifndef BITBOARDHELPER_H
#define BITBOARDHELPER_H

#include "typedefs.h"
#include "lightfigureposition.h"

#define BITBOARD_EMPTY 0ULL

class BitboardHelper
{
public:
    static PositionCollection GetPositions(BITBOARD bitboard);

    static BITBOARD AddPosition(BITBOARD bitboard, POSITION p);
    static BITBOARD AddPositionWhenValid(BITBOARD bitboard, POSITION p);

    static BITBOARD RemovePosition(BITBOARD bitboard, POSITION p);

    static BITBOARD Union(BITBOARD bitboard1, BITBOARD bitboard2);
    static BITBOARD Substract(BITBOARD bitboard1, BITBOARD bitboard2);

    static bool Contains(BITBOARD bitboard, POSITION position);
};

inline PositionCollection BitboardHelper::GetPositions(BITBOARD bitboard)
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

inline BITBOARD BitboardHelper::AddPosition(BITBOARD bitboard, POSITION p)
{
    return bitboard | (1ULL << PositionHelper::Serial(p));
}

inline BITBOARD BitboardHelper::RemovePosition(BITBOARD bitboard, POSITION p)
{
    return bitboard & (~ (1ULL << PositionHelper::Serial(p)));
}

inline BITBOARD BitboardHelper::AddPositionWhenValid(BITBOARD bitboard, POSITION p)
{
    if (!PositionHelper::IsInvalid(p))
    {
        return bitboard | (1ULL << PositionHelper::Serial(p));
    } else
    {
        return bitboard;
    }
}

inline BITBOARD BitboardHelper::Union(BITBOARD bitboard1, BITBOARD bitboard2)
{
    return bitboard1 | bitboard2;
}

inline BITBOARD BitboardHelper::Substract(BITBOARD bitboard1, BITBOARD bitboard2)
{
    return bitboard1 & (~bitboard2);
}

inline bool BitboardHelper::Contains(BITBOARD bitboard, POSITION p)
{
    return bitboard & (1ULL << PositionHelper::Serial(p));
}

#endif // BITBOARDHELPER_H
