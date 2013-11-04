#ifndef RULES_H
#define RULES_H

#include <QList>

#include "typedefs.h"
#include "board.h"
#include "move.h"
#include "lightfigureposition.h"
#include "figure.h"

class Rules
{
    Board* m_board;    

    POSITION ForwardFor(POSITION position, FigureSide side, int dx, int dy) const;

    int PawnPromotionYFor(FigureSide side) const;
    int EnPassantPawnYFor(FigureSide side) const;

    bool IsUnderCheckImpl(FigureSide side) const;
    Figure* GetObstacleInDirection(POSITION position, FigureSide side, int xMult, int yMult) const;
    bool IsUnderCheckFastImpl(FigureSide side) const;

    // DeleteMovesToCheck
    // -------------------
    // Function erases all moves which causes check to turning player
    MoveCollection &DeleteMovesToCheck(MoveCollection &moves);
    // NOTE: ...PossibleDestinations function family returns NOT FULLY VALID destination yet!
    // because it SHOULD BE filtered by DeleteMovesToCheck and DeleteSelfCaptureDesination functions
    // to avoid moves, that causes check to turning side and self-capture turns
    // ONLY FOR PRIVATE USAGE!
    void DeleteSelfCaptureDesination(PositionList* destinations, FigureSide selfSide) const;
    PositionList _GetPossibleDestinations(Figure* figure) const;    
    PositionList _GetPawnPossibleDestinations(Figure* figure) const;
    PositionList _GetKnightPossibleDestinations(Figure* figure) const;
    PositionList _GetBishopPossibleDestinations(Figure* figure) const;
    PositionList _GetRockPossibleDestinations(Figure* figure) const;
    PositionList _GetQueenPossibleDestinations(Figure* figure) const;
    PositionList _GetKingPossibleDestinations(Figure* figure) const;

    Move CreateMove(POSITION from, POSITION to);

    // GetOnLinePositions function
    // ---------------------------
    // This function was written because many of figures can be described in general manner.
    // e.g.
    //  - bishop can guard in 4 diagonal lines (forward-right, forward-left, backward-right, backward-left) to
    //     the end of board, and guard line interrupted by obstacle (figure)
    //  - rock have simular situation, but have anothe line definition
    //  - queen is actually bishop + rock
    //  - pawn, yes even pawn have obly 2 guard lines (forward-left and forward-right) but it len limited by one
    //
    // RETURNS: Function returns positions on some line before obstacle (figure) but inclusive it
    //
    // NOTE: valid xMult and yMult values in set {-1, 0, +1}!!!
    //
    // yMult = 1 means what line has forward direction, -1 - backward
    // xMult = 1 means what line has direction which x are increasing
    //
    // EXAMPLE: function runned with xMult = 0, yMult = 1 and lenLimit = 2 (...,0,1,2) returns
    // pawn first move possibilities;
    PositionList GetOnLinePositions(POSITION position, FigureSide side, int xMult, int yMult, int lenLimit) const;
    BITBOARD GetOnLinePositions2(POSITION position, FigureSide side, int xMult, int yMult, int lenLimit) const;

    // Returns positions that fuarded for specified figure.
    // Position is guarded when figure can kill another figure on that position.
    PositionList GetGuardedPositions        (Figure* figure) const;
    PositionList GetPawnGuardedPositions    (Figure* figure) const;
    PositionList GetKinghtGuardedPositions  (Figure* figure) const;
    PositionList GetBishopGuardedPositions  (Figure* figure) const;
    PositionList GetRockGuardedPositions    (Figure* figure) const;
    PositionList GetQueenGuardedPositions   (Figure* figure) const;
    PositionList GetKingGuardedPositions    (Figure* figure) const;

    PositionList GetGuardedPositions(FigureSide side) const;

    // Bitboard implementation

    BITBOARD GetGuardedPositions2        (Figure* figure) const;
    BITBOARD GetPawnGuardedPositions2    (Figure* figure) const;
    BITBOARD GetKinghtGuardedPositions2  (Figure* figure) const;
    BITBOARD GetBishopGuardedPositions2  (Figure* figure) const;
    BITBOARD GetRockGuardedPositions2    (Figure* figure) const;
    BITBOARD GetQueenGuardedPositions2   (Figure* figure) const;
    BITBOARD GetKingGuardedPositions2    (Figure* figure) const;

    BITBOARD GetGuardedPositions2(FigureSide side) const;
public:
    Rules(Board* board);

    FigureSide OpponentSide(FigureSide side) const;
    int FirstHorizonatalYFor(FigureSide side) const;

    MoveCollection GetPossibleMoves(FigureSide side);

    // !! USE IN GUI ONLY !!
    // Very slow: evaluates possible moves for all figures and then extrude it for specified figure
    MoveCollection GetPossibleMoves(Figure* figure);

    // !! USE IN GUI ONLY !!
    // Very slow: evaluates possible destination for all figures and then extrude it for specified figure
    PositionList GetPossibleDestinations(Figure* figure);

    bool IsUnderCheck(FigureSide side) const;
    bool IsPassiveEndGame() const;

    void MakeMove(Move move);
    void MakeMove(POSITION from, POSITION to);
    void UnMakeMove(Move move);

    friend class Tests;
};

#endif // RULES_H
