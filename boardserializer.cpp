#include <QStringList>

#include "boardserializer.h"

QString BoardSerializer::Save(const Board& board)
{
    // save all moves history
    QString result;

    QStack<Move> history = board.GetMoveHistory();

    QStack<Move>::const_iterator it = history.constBegin();
    while (it != history.constEnd())
    {
        Move move = *it;

        result.append(ToString(move.From));
        result.append("-");
        result.append(ToString(move.To));

        if (it != history.constEnd() - 1)
        {
             result.append(", ");
        }

        ++it;
    }

    return result;
}

Board BoardSerializer::Load(QString boardString)
{
    Board board = Board();
    Rules rules = Rules(&board);

    board.SetupStartPosition();

    QStringList moveStrings = boardString.split(", ");

    foreach(QString moveStirng, moveStrings)
    {
        QStringList m = moveStirng.split('-');

        POSITION from = CreateFigurePosition(m[0].toStdString());
        POSITION to = CreateFigurePosition(m[1].toStdString());

        rules.MakeMove(from, to);
    }

    return board;
}
