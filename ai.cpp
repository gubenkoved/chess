#include "ai.h"
#include "exception.h"

int AI::m_staticFigurePositionEstimation[SIDE_COUNT][FIGURE_COUNT][8 * 8];

AI::AI(Board* board, Rules *rules)
{
    m_board = board;
    m_rules = rules;

#ifdef USE_TRANSPOSITION_TABLE
    m_transpositionTable = new TranspositionTable();
#endif

    InitStaticFigurePositionEstimations();
}

AI::~AI()
{
#ifdef USE_TRANSPOSITION_TABLE
    delete m_transpositionTable;
#endif
}

void AI::InitStaticFigurePositionEstimations()
{
    for (int x = 1; x <= 8; ++x)
    {
        for (int y = 1; y <= 8; ++y)
        {
            POSITION position = CreateFigurePosition(x, y);
            for (int figureTypeValue = 0; figureTypeValue < FIGURE_COUNT; ++figureTypeValue)
            {
                Figure::FigureType figureType = (Figure::FigureType)figureTypeValue;

                m_staticFigurePositionEstimation[(int)Figure::White][figureTypeValue][Serial(position)] = CalculateFigurePositionEstimation(figureType, position, Figure::White);
                m_staticFigurePositionEstimation[(int)Figure::Black][figureTypeValue][Serial(position)] = CalculateFigurePositionEstimation(figureType, position, Figure::Black);
            }
        }
    }
}

int AI::GetFigurePositionEstimation(Figure::FigureType type, POSITION position, Figure::FigureSide side)
{
    return m_staticFigurePositionEstimation[(int)side][(int)type][Serial(position)];
}


int AI::GetFigureWeight(Figure::FigureType type)
{
    switch (type)
    {
        case Figure::Pawn:
            return 100;
        case Figure::Bishop:
            return 300;
        case Figure::Knight:
            return 300;
        case Figure::Rock:
            return 500;
        case Figure::Queen:
            return 900;
        case Figure::King:
            return 0; // can not be killed
        default:
            throw Exception("Invalid figure type");
    }
}

int AI::CalculateFigurePositionEstimation(Figure::FigureType type, POSITION position, Figure::FigureSide side)
{
    int homeHorizontal = (side == Figure::White ? 1 : 8);

    int x = X(position);
    int y = Y(position);

    float center = 4.5f;

    switch (type)
    {
        case Figure::Pawn:            
            return (int)(0.5f * (5 - abs(x - 5)) * abs(y - homeHorizontal)); // promotion priority
        case Figure::Bishop:       
            return (int)2 * ((5 - abs(x - center)) + (5 - abs(y - center))); // center priority
        case Figure::Knight:
            return (int) 2 * ((5 - abs(x - center)) + (5 - abs(y - center))); // center priority
        case Figure::Rock:
            return 0;
        case Figure::Queen:
            return (int) 3 * ((5 - abs(x - center)) + (5 - abs(y - center))); // center priority
        case Figure::King:
            return 8 - abs(y - homeHorizontal); // home priority
        default:
            throw Exception("Invalid figure type");
    }
}

int AI::GetFiguresEstimation(Figure::FigureSide side) const
{
    int estimation = 0;
    FigureList figures = m_board->FiguresAt(side);

    foreach(Figure* figure, figures)
    {
        estimation += GetFigureWeight(figure->Type) + GetFigurePositionEstimation(figure->Type, figure->Position, figure->Side);
    }

    return estimation;
}

int AI::GetRelativeEstimationFor(Figure::FigureSide side) const
{
    int currentSideEstimation = GetFiguresEstimation(side);
    int opponentSideEstimation = GetFiguresEstimation(m_rules->OpponentSide(side));

    return currentSideEstimation - opponentSideEstimation;
}

int AI::GetTerminalPositionEstimation(Figure::FigureSide side, int depth) const
{
    // when side have no possible turns - is game over

    if (m_rules->IsUnderCheck(side)) // when under check
    {
        // -depth to make AI to use shortest checkmate
        return LOOSE_ESTIMATION - depth; // loose
    } else
    {
        return 0; // draw game
    }
}

// main negamax worker function
int AI::Negamax(Figure::FigureSide side, int depth, int& analyzed)
{
    if (m_rules->IsPassiveEndGame())
        return 0; // draw game

    if (depth == 0)
    {
        analyzed++;
        return GetRelativeEstimationFor(side);
    }

    MoveList possibleMoves = m_rules->GetPossibleMoves(side);

    if (possibleMoves.count() == 0) // is terminal node
        return GetTerminalPositionEstimation(side, depth);

    int bestEstimation = INT_MIN;

    foreach(Move possibleMove, possibleMoves)
    {
        // temporary make move
        m_rules->MakeMove(possibleMove);

        int estimation = -Negamax(m_rules->OpponentSide(side), depth - 1, analyzed);

        // unmake temporary move
        m_rules->UnMakeMove(possibleMove);

        if (estimation > bestEstimation)
        {            
            bestEstimation = estimation;            
        }       
    }

    return bestEstimation;
}

// Negamax search initializer
Move AI::NegamaxSearch(Figure::FigureSide side, int depth, int& bestEstimation, int& analyzed)
{
    MoveList possibleMoves = m_rules->GetPossibleMoves(side);

    Move bestMove;
    bestEstimation = -INT_MAX;
    analyzed = 0;

    foreach(Move possibleMove, possibleMoves)
    {
        m_rules->MakeMove(possibleMove);

        int estimation = -Negamax(m_rules->OpponentSide(side), depth - 1, analyzed);

        m_rules->UnMakeMove(possibleMove);

        if (estimation > bestEstimation)
        {
            bestEstimation = estimation;
            bestMove = possibleMove;
        }
    }

    return bestMove;
}


int AI::MovePriority(Move::MoveType type)
{
    switch (type)
    {
        case Move::PawnPromotion: return 10;
        case Move::Capture: return 8;
        case Move::EnPassant: return 7;
        case Move::LongCastling: return 6;
        case Move::ShortCastling: return 6;
        case Move::LongPawn: return 4;
        default: return 0;
    }
}

static int cmp_count = 0;

// returns true when m1 should be considered before m2
bool AI::MoveComparator(const Move& m1, const Move& m2)
{
    ++cmp_count;
    //return false;

    int m1Priority = MovePriority(m1.Type);
    int m2Priority = MovePriority(m2.Type);

    if (m1Priority != m2Priority)
    {
        return m1Priority > m2Priority;
    }
    else // priorities is equals
    {
        Move::MoveType type = m1.Type; // == m2.Type

        // captures sort
        if (type == Move::Capture)
        {
            // MVV/LVA (Most Valuable Victim - Least Valuable Aggressor) captures sort
            int capture1Profit = GetFigureWeight(m1.CapturedFigure->Type) - GetFigureWeight(m1.MovingFigure->Type);
            int capture2Profit = GetFigureWeight(m2.CapturedFigure->Type) - GetFigureWeight(m2.MovingFigure->Type);

            return capture1Profit > capture2Profit;
        } else //if (type == Move::Normal || type == Move::LongPawn)
        {
            int move1EstimationDelta = GetFigurePositionEstimation(m1.MovingFigure->Type, m1.To, m1.MovingFigure->Side)
                    - GetFigurePositionEstimation(m1.MovingFigure->Type, m1.From, m1.MovingFigure->Side);

            int move2EstimationDelta = GetFigurePositionEstimation(m2.MovingFigure->Type, m2.To, m2.MovingFigure->Side)
                    - GetFigurePositionEstimation(m2.MovingFigure->Type, m2.From, m2.MovingFigure->Side);

            return move1EstimationDelta > move2EstimationDelta;
        }

        return true; // doesn't matter
    }
}

// main alpha beta worker function
int AI::AlphaBetaNegamax(Figure::FigureSide side, int depth, int alpha, int beta, int& analyzed, Move*& bestMove)
{
#ifdef USE_TRANSPOSITION_TABLE
    const TranspositionTableEntry* hashEntry = m_transpositionTable->FindEntry(m_board->GetCurrentPositionHash());
    if (hashEntry != NULL && hashEntry->Depth >= depth)
    {
        return hashEntry->Estimation;
    }
#endif

    if (m_rules->IsPassiveEndGame())
    {
        return 0; // draw game
    }

    if (depth == 0)
    {
        analyzed++;
        return GetRelativeEstimationFor(side);
    }

    MoveList possibleMoves = m_rules->GetPossibleMoves(side);

    // moves sort to increase alpha beta pruning productivity
    qSort(possibleMoves.begin(), possibleMoves.end(), MoveComparator);

    if (possibleMoves.count() == 0) // is terminal node
    {
        return GetTerminalPositionEstimation(side, depth);
    }

    foreach(Move possibleMove, possibleMoves)
    {
        // temporary make move
        m_rules->MakeMove(possibleMove);

        int estimation = -AlphaBetaNegamax(m_rules->OpponentSide(side), depth - 1, -beta, -alpha, analyzed, bestMove);

        // unmake temporary move
        m_rules->UnMakeMove(possibleMove);

        // good move finded
        if (estimation > alpha)
        {
            if (bestMove != NULL)
                delete bestMove;

            bestMove = new Move(possibleMove);

            alpha = estimation;
        } else if (bestMove == NULL) // or best move is not filled yet
        {
            bestMove = new Move(possibleMove);
        }

        if (alpha >= beta)
        {
#ifdef USE_TRANSPOSITION_TABLE
            m_transpositionTable->Store(m_board->GetCurrentPositionHash(), alpha, depth);
#endif

            return alpha; // prune
        }
    }

#ifdef USE_TRANSPOSITION_TABLE
    m_transpositionTable->Store(m_board->GetCurrentPositionHash(), alpha, depth);
#endif

    return alpha;
}

// alpha beta serach initializer
Move AI::BestMoveByAlphaBeta(Figure::FigureSide side, int depth, int& bestEstimation, int& analyzed)
{
#ifdef USE_TRANSPOSITION_TABLE
    m_transpositionTable->Clear();
#endif

    Move* bestMove = NULL;
    analyzed = 0;

    bestEstimation = AlphaBetaNegamax(side, depth, -INT_MAX, +INT_MAX, analyzed, bestMove);

    Move result = *bestMove;
    delete bestMove;

    return result;
}



















