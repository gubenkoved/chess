#ifndef AI_H
#define AI_H

#include <QSharedPointer>

#include "rules.h"
#include "move.h"
#include "figure.h"
#include "typedefs.h"
#include "transpositiontable.h"

//#define USE_TRANSPOSITION_TABLE

// This class contains algorithms to find best move
class AI
{
private:    
    static const int WIN_ESTIMATION     = +1000000;
    static const int LOOSE_ESTIMATION   = -WIN_ESTIMATION;

    Board* m_board;
    Rules* m_rules;

#ifdef USE_TRANSPOSITION_TABLE
    TranspositionTable* m_transpositionTable;
#endif

    struct PrioritizedMove
    {
        UCHAR PriorityClass;
        UCHAR PriorityValue;
        Move UnderlyingMove;
    };

    static PrioritizedMove CalculatePriority(const Move &move);
    static bool MoveComparator2(const PrioritizedMove& m1, const PrioritizedMove& m2);

    // usage example:
    // m_staticFigurePositionEstimation[Figure::White][Figure::Pawn][Serial(CreateFigurePosition(1, 1))]
    static int m_staticFigurePositionEstimation[SIDE_COUNT][FIGURE_COUNT][8 * 8];
    static void InitStaticFigurePositionEstimations();

    static int GetFigureWeight(Figure::FigureType type);
    static int GetFigurePositionEstimation(Figure::FigureType type, POSITION position, Figure::FigureSide side);
    static int CalculateFigurePositionEstimation(Figure::FigureType type, POSITION position, Figure::FigureSide side);

    int GetFiguresEstimation(Figure::FigureSide side) const;
    // Returns positive value if current side has benefits compared with the opponent
    int GetRelativeEstimationFor(Figure::FigureSide side) const;
    int GetTerminalPositionEstimation(Figure::FigureSide side, int depth) const;

    int Negamax(Figure::FigureSide side, int depth, int &analyzed);

    static int MovePriority(Move::MoveType type);
    static bool MoveComparator(const Move& m1, const Move& m2);

    // NOTE:
    // Do not use INT_MIN as alpha - it provokes overflow.
    // Call with (-INT_MAX to INT_MAX) alpha beta window to avoid int overflow issues when alpha and beta will be switched.
    // bestMove is only filled by function when isTopLEvel function.
    int AlphaBetaNegamax(Figure::FigureSide side, int depth, int alpha, int beta, int& analyzed, Move*& bestMove, bool isTopLevel);

public:
    AI(Board* board, Rules* rules);
    ~AI();

    bool ExtendSearchDepthOnCaptures;
    bool UseMovesOrdering;

    Move BestMoveByAlphaBeta(Figure::FigureSide side, int depth, int& bestEstimation, int& analyzed);
    Move NegamaxSearch(Figure::FigureSide side, int depth, int& bestEstimation, int& analyzed);

    //friend class Tests;
};

#endif // AI_H
