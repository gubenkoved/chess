#include "ai.h"
#include "exception.h"

int AI::m_staticFigurePositionEstimation[SIDE_COUNT][FIGURE_COUNT][8 * 8];

AI::AI(Board* board, Rules *rules)
{
    m_board = board;
    m_rules = rules;

    m_transpositionTable = new TranspositionTable();

    ExtendSearchDepthOnCaptures = true;
    MaxCurrentDepthToExtendSearchOnCaptures = 1;

    UseMovesOrdering = true;
    UseTranspositionTable = true;

    InitStaticFigurePositionEstimations();
}

AI::~AI()
{
    delete m_transpositionTable;
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
            return 0; // can not be killed -> it weight doesn't not matter
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


AI::PrioritizedMove AI::CalculatePriority(const Move& move)
{
    PrioritizedMove prioritizedMove;
    prioritizedMove.UnderlyingMove = move;

    // claculate priority class
    switch (move.Type)
    {
        case Move::PawnPromotion:   prioritizedMove.PriorityClass = 10; break;
        case Move::Capture:         prioritizedMove.PriorityClass = 8; break;
        case Move::EnPassant:       prioritizedMove.PriorityClass = 7; break;
        case Move::LongCastling:    prioritizedMove.PriorityClass = 6; break;
        case Move::ShortCastling:   prioritizedMove.PriorityClass = 6; break;
        case Move::LongPawn:        prioritizedMove.PriorityClass = 4; break;
        default:                    prioritizedMove.PriorityClass = 0; break;
    }

    // calculate priority value
    if (move.Type == Move::Capture)// captures sort
    {
        // MVV/LVA (Most Valuable Victim - Least Valuable Aggressor) captures sort
        int captureProfit =
                GetFigureWeight(move.CapturedFigure->Type)
              - GetFigureWeight(move.MovingFigure->Type);

        prioritizedMove.PriorityValue = captureProfit;

    } else // other moves sort
    {
        int moveEstimationDelta =
                GetFigurePositionEstimation(move.MovingFigure->Type, move.To, move.MovingFigure->Side)
              - GetFigurePositionEstimation(move.MovingFigure->Type, move.From, move.MovingFigure->Side);

        prioritizedMove.PriorityValue = moveEstimationDelta;
    }

    return prioritizedMove;
}

// returns true when m1 move has greater priority than m2 move
bool AI::MoveByPriorityGreaterThan(const PrioritizedMove& pm1, const PrioritizedMove& pm2)
{
    if (pm1.PriorityClass != pm2.PriorityClass)
    {
        return pm1.PriorityClass > pm2.PriorityClass;
    } else
    {
        return pm1.PriorityValue > pm2.PriorityValue;
    }
}

// main alpha beta worker function
int AI::AlphaBetaNegamax(Figure::FigureSide side, int depth, int alpha, int beta, int& analyzed, Move*& bestMove, bool isTopLevel)
{
    if (UseTranspositionTable)
    {
        // if position evaluated before with equal or bigger depth use it as estimation
        const TranspositionTableEntry* hashEntry = m_transpositionTable->FindEntry(m_board->GetCurrentPositionHash());
        if (hashEntry != NULL && hashEntry->Depth >= depth)
        {
            return hashEntry->Estimation;
        }
    }

    // checks passive end game rule
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
    int possibleMovesCount = possibleMoves.count();

    QVector<PrioritizedMove> prioritizedMoves(possibleMovesCount);
    for (int i = 0; i < possibleMovesCount; ++i)
    {
        prioritizedMoves[i] = CalculatePriority(possibleMoves.at(i));
    }

    // moves sort to increase alpha beta pruning productivity
    if (UseMovesOrdering)
    {
        qSort(prioritizedMoves.begin(), prioritizedMoves.end(), MoveByPriorityGreaterThan);
    }

    if (possibleMoves.count() == 0) // is terminal node
    {
        return GetTerminalPositionEstimation(side, depth);
    }

    foreach(const PrioritizedMove& prioritizedPossibleMove, prioritizedMoves)
    {
        Move move = prioritizedPossibleMove.UnderlyingMove;

        // temporary make move
        m_rules->MakeMove(move);

        int estimation;

        // extend search depth when move is capture
        if (ExtendSearchDepthOnCaptures && depth <= MaxCurrentDepthToExtendSearchOnCaptures && move.Type == Move::Capture)
        {
            estimation = -AlphaBetaNegamax(m_rules->OpponentSide(side), depth, -beta, -alpha, analyzed, bestMove, false);
        }
        else
        {
            estimation = -AlphaBetaNegamax(m_rules->OpponentSide(side), depth - 1, -beta, -alpha, analyzed, bestMove, false);
        }

        // unmake temporary move
        m_rules->UnMakeMove(move);

        // good move finded
        if (estimation > alpha)
        {
            if (isTopLevel)
            {
                if (bestMove != NULL)
                {
                    delete bestMove;
                }

                bestMove = new Move(move);
            }

            alpha = estimation;
        } else if (bestMove == NULL) // or best move is not filled yet
        {
            if (isTopLevel)
            {
                bestMove = new Move(move);
            }
        }

        if (alpha >= beta)
        {
            return alpha; // alpha prune
        }
    }

    if (UseTranspositionTable)
    {
        m_transpositionTable->Store(m_board->GetCurrentPositionHash(), alpha, depth);
    }

    return alpha;
}

// alpha beta serach initializer
Move AI::BestMoveByAlphaBeta(Figure::FigureSide side, int depth, int& bestEstimation, int& analyzed)
{
    m_transpositionTable->Clear();

    Move* bestMove = NULL;
    analyzed = 0;

    bestEstimation = AlphaBetaNegamax(side, depth, -INT_MAX, +INT_MAX, analyzed, bestMove, true);

    Move result = *bestMove;
    delete bestMove;

#ifdef QT_DEBUG
    qDebug() << "Amount of records in transposition table: " << m_transpositionTable->Count();
#endif

    return result;
}



















