#include "lightfigureposition.h"

INT32 PositionHelper::X(POSITION pos)
{
    return ((pos & POS_X) >> 3) + 1;
}

INT32 PositionHelper::Y(POSITION pos)
{
    return (pos & POS_Y) + 1;
}

INT32 PositionHelper::Serial(POSITION pos)
{
    return pos;
}

POSITION PositionHelper::FromSerial(INT32 serial)
{
    return serial;
}

BOOL PositionHelper::IsInvalid(POSITION pos)
{
    return pos & POS_INVALID;
}

BOOL PositionHelper::IsValidPosition(int x, int y)
{
    return x >= 1 && x <= 8 && y >= 1 && y <= 8;
}

POSITION PositionHelper::Create(int x, int y)
{
    if (IsValidPosition(x, y))
    {
        return (y - 1) | ((x - 1) << 3);
    } else
    {
        return POS_INVALID;
    }
}

POSITION PositionHelper::Shift(POSITION position, int dx, int dy)
{
    return PositionHelper::Create(PositionHelper::X(position) + dx, PositionHelper::Y(position) + dy);
}

POSITION PositionHelper::FromString(std::string str)
{
    char xChar = str[0];
    char yChar = str[1];

    int x = ((int)xChar - (int)'a') + 1;
    int y = ((int)yChar - (int)'1') + 1;

    return PositionHelper::Create(x, y);
}

QString PositionHelper::ToString(POSITION p)
{
    QChar c1 = 'a' + PositionHelper::X(p) - 1; //QChar::fromAscii('a' + PositionHelper::X(p) - 1);
    QChar c2 = '1' + PositionHelper::Y(p) - 1; //QChar::fromAscii('1' + PositionHelper::Y(p) - 1);

    QString s = QString(c1) + QString(c2);

    return s;
}
