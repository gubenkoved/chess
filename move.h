#ifndef MOVE_H
#define MOVE_H

#include "typedefs.h"
#include "figure.h"
#include "figureposition.h"
#include "exception.h"

enum MoveType
{
    Normal, // just move from one to another position
    Capture, // capture enemy figure
    LongPawn, // first pawn long step
    LongCastling, // long castling
    ShortCastling, // short castling
    EnPassant, // one pawn takes another, when it steps through trapped cell
    PawnPromotion, // when pawn reaches enemy home horizon
    Invalid // when default constructor used
};

struct Move
{
    POSITION From;
    POSITION To;
    MoveType Type;
    Figure* MovingFigure;
    Figure* CapturedFigure; // for Capture and EnPassant not NULL, and NULL otherwise
    FigureType PromotedTo; // for PawnPromotion only

    Move();
    Move(MoveType type, POSITION from, POSITION to, Figure* figure, Figure* captured);    
    Move(const Move& another);

    bool IsCastling() const;

    Move& operator=(const Move& another);
};

inline QDebug operator<<(QDebug debug, const Move& m)
{
    QString moveTypeCode;
    switch (m.Type)
    {
        case Normal:        moveTypeCode = "Normal";        break;
        case Capture:       moveTypeCode = "Capture";       break;
        case EnPassant:     moveTypeCode = "EnPassant";     break;
        case LongCastling:  moveTypeCode = "LongCastling";  break;
        case ShortCastling: moveTypeCode = "ShortCastling"; break;
        case PawnPromotion: moveTypeCode = "PawnPromotion"; break;
        case LongPawn:      moveTypeCode = "LongPawn";      break;
        case Invalid:       moveTypeCode = "Invalid";       break;
        default: throw Exception("Unknown move type");
    }    

    debug << "Move("
        << m.MovingFigure->GetName()
        << moveTypeCode
        << m.PromotedTo
        << "turn from"
        << PositionHelper::ToString(m.From) << "to"
        << PositionHelper::ToString(m.To) << ")";

    return debug;
}
#endif // MOVE_H
