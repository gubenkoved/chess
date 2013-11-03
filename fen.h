#ifndef FEN_H
#define FEN_H

#include "figure.h"
#include "board.h"

class FEN
{
public:
    static QChar GetFigureChar(const Figure& figure);
    static Figure FigureFromChar(QChar c);

    static QString Evaluate(const Board& board);
    //static Board BoardFromFEN(QString fen);
};

#endif // FEN_H
