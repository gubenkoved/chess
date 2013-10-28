#include "lightfigureposition.h"

INT32 X(POSITION pos)
{
    return ((pos & 0x38) >> 3) + 1;
}

INT32 Y(POSITION pos)
{
    return (pos & 0x07) + 1;
}

INT32 Serial(POSITION pos)
{
    return (pos & 0x38) + (pos & 0x07);
}

BOOL IsInvalid(POSITION pos)
{
    return pos & 0x80;
}

BOOL IsValidPosition(int x, int y)
{
    return x >= 1 && x <= 8 && y >= 1 && y <= 8;
}

POSITION CreateFigurePosition(int x, int y)
{
    if (IsValidPosition(x, y))
    {
        return (y - 1) | ((x - 1) << 3);
    } else
    {
        return 0x80;
    }
}

POSITION Shift(POSITION position, int dx, int dy)
{
    return CreateFigurePosition(X(position) + dx, Y(position) + dy);
}

POSITION CreateFigurePosition(std::string str)
{
    char xChar = str[0];
    char yChar = str[1];

    int x = ((int)xChar - (int)'a') + 1;
    int y = ((int)yChar - (int)'1') + 1;

    return CreateFigurePosition(x, y);
}

QString ToString(POSITION p)
{
    QChar c1 = QChar::fromAscii('a' + X(p) - 1);
    QChar c2 = QChar::fromAscii('1' + Y(p) - 1);

    QString s = QString(c1) + QString(c2);

    return s;
}
