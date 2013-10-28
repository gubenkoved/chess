#ifndef LIGHTFIGUREPOSITION_H
#define LIGHTFIGUREPOSITION_H

#include <QString>
#include <QDebug>

#include "typedefs.h"

//typedef QVector<POSITION> PositionCollection;
typedef QList<POSITION> PositionList;

// POSITION BYTE LAYOUT
// -----------------------------------------
// uses 8-byte figure position representation
//
// 1 2 3 4 5 6 7 8
// ^ ^ ^ ^ ^ ^ ^ ^
// | | | | | |_|_|__y
// | | |_|_|__x
// | |__unused bit
// invalid indicator bit


// returns x-coordinate from position
// value in [1; 8]; 1 means a, ..., 8 means h
//#define X(p) (((p & 0x38) >> 3) + 1)
INT32 X(POSITION pos);

// returns y-coordinate position
// value in [1; 8];
INT32 Y(POSITION pos);

// returns position's serial number
// value in [0; 63]
// 0 for a1
// 1 for b1,
// ...,
// 63 for h8
INT32 Serial(POSITION pos);

// checks bit that is invalid position indicator
// returns not 0 when position is invalid
BOOL IsInvalid(POSITION pos);

// returns true when x in [1; 8] and y in [1; 8]
BOOL IsValidPosition(int x, int y);

POSITION CreateFigurePosition(int x, int y);

POSITION Shift(POSITION position, int dx, int dy);

POSITION CreateFigurePosition(std::string str);
QString ToString(POSITION p);

static inline QDebug operator<<(QDebug debug, POSITION pos)
{
    debug.nospace() << ToString(pos);

    return debug.space();
}
#endif // LIGHTFIGUREPOSITION_H
