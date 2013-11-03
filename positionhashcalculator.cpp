#include "positionhashcalculator.h"
#include "lightfigureposition.h"
#include "board.h"
#include "fen.h"
#include "figure.h"

PositionHash PositionHashCalculator::Calculate(Board* board)
{
    PositionHash hash = PositionHash(64, '-');

    foreach (Figure* figure, board->GetAllAliveFigures())
    {
        hash.replace(Serial(figure->Position) - 1, 1, FEN::GetFigureChar(*figure));
    }

    return hash;
}

void PositionHashCalculator::Update(PositionHash &hash, Board *board, POSITION position)
{
    QChar newChar = '-';

    Figure* figureAtPosition = board->FigureAt(position);

    if (figureAtPosition != NULL)
    {
        newChar = FEN::GetFigureChar(*figureAtPosition);
    }

    hash.replace(Serial(position) - 1, 1, newChar);
}
