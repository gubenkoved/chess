#include <algorithm>

#include "rules.h"
#include "typedefs.h"
#include "exception.h"
#include "helpers.h"
#include "bitboardhelper.h"

Rules::Rules(Board *board)
    :m_board(board)
{
}

PositionList Rules::GetOnLinePositions(POSITION position, FigureSide side, int xMult, int yMult, int lenLimit = 7) const
{
    if (xMult != -1 && xMult != 0 && xMult != 1 || yMult != -1 && yMult != 0 && yMult != 1)
    {
        throw Exception("Only {-1, 0, 1} values are valid for x and y mult");
    }

    PositionList guarded;    

    for (int delta = 1; delta <= lenLimit; ++delta)
    {
        POSITION p = ForwardFor(position, side, delta * xMult, delta * yMult);

        if (!PositionHelper::IsInvalid(p))
        {
            guarded.append(p);

            if (m_board->FigureAt(p) != NULL) // obstacle
            {
                break;
            }
        } else
        {
            break;
        }
    }

    return guarded;
}

BITBOARD Rules::GetOnLinePositions2(POSITION position, FigureSide side, int xMult, int yMult, int lenLimit = 7) const
{
#ifdef QT_DEBUG
    if (xMult != -1 && xMult != 0 && xMult != 1 || yMult != -1 && yMult != 0 && yMult != 1)
    {
        throw Exception("Only {-1, 0, 1} values are valid for x and y mult");
    }
#endif

    BITBOARD bitboard = BITBOARD_EMPTY;

    for (int delta = 1; delta <= lenLimit; ++delta)
    {
        POSITION p = ForwardFor(position, side, delta * xMult, delta * yMult);

        if (!PositionHelper::IsInvalid(p))
        {
            bitboard = BitboardHelper::AddPosition(bitboard, p);

            if (m_board->FigureAt(p) != NULL) // obstacle
            {
                break;
            }
        } else
        {
            break;
        }
    }

    return bitboard;
}

PositionList Rules::GetPawnGuardedPositions(Figure *figure) const
{
    POSITION p = figure->Position;
    FigureSide side = figure->Side;

    PositionList guarded;

    Helpers::AppendIfValid(guarded, ForwardFor(p, side, +1, +1));
    Helpers::AppendIfValid(guarded, ForwardFor(p, side, -1, +1));

    return guarded;
}

BITBOARD Rules::GetPawnGuardedPositions2(Figure *figure) const
{
    POSITION p = figure->Position;
    FigureSide side = figure->Side;
    INT32 x = PositionHelper::X(p);

    BITBOARD bitboard = BITBOARD_EMPTY;

    if (x >= 2)
    {
        bitboard = BitboardHelper::AddPosition(bitboard, ForwardFor(p, side, -1, +1));
    }

    if (x <= 7)
    {
        bitboard = BitboardHelper::AddPosition(bitboard, ForwardFor(p, side, +1, +1));
    }

    return bitboard;
}

PositionList Rules::GetKinghtGuardedPositions(Figure *figure) const
{
    POSITION p = figure->Position;
    FigureSide side = figure->Side;

    PositionList guarded;

    Helpers::AppendIfValid(guarded, ForwardFor(p, side, +2, +1));
    Helpers::AppendIfValid(guarded, ForwardFor(p, side, +1, +2));
    Helpers::AppendIfValid(guarded, ForwardFor(p, side, -1, +2));
    Helpers::AppendIfValid(guarded, ForwardFor(p, side, -2, +1));

    Helpers::AppendIfValid(guarded, ForwardFor(p, side, -2, -1));
    Helpers::AppendIfValid(guarded, ForwardFor(p, side, -1, -2));
    Helpers::AppendIfValid(guarded, ForwardFor(p, side, +1, -2));
    Helpers::AppendIfValid(guarded, ForwardFor(p, side, +2, -1));

    return guarded;
}

BITBOARD Rules::GetKinghtGuardedPositions2(Figure *figure) const
{
    POSITION p = figure->Position;

    BITBOARD bitboard = BITBOARD_EMPTY;

    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, +2, +1));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, +1, +2));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, -1, +2));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, -2, +1));

    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, -2, -1));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, -1, -2));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, +1, -2));
    bitboard = BitboardHelper::AddPositionWhenValid(bitboard, PositionHelper::Shift(p, +2, -1));

    return bitboard;
}

PositionList Rules::GetBishopGuardedPositions(Figure *figure) const
{
    POSITION p = figure->Position;
    FigureSide side = figure->Side;

    PositionList guarded;

    guarded.append(GetOnLinePositions(p, side, +1, +1, 7)); // forward-right
    guarded.append(GetOnLinePositions(p, side, +1, -1, 7)); // backward-right
    guarded.append(GetOnLinePositions(p, side, -1, +1, 7)); // forawrd-left
    guarded.append(GetOnLinePositions(p, side, -1, -1, 7)); // forward-right

    return guarded;
}

BITBOARD Rules::GetBishopGuardedPositions2(Figure *figure) const
{
    POSITION p = figure->Position;
    FigureSide side = figure->Side;

    BITBOARD bitboard = BITBOARD_EMPTY;

    //bitboard = BitBoardHelper::AddPositionWhenValid(bitboard, )
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions2(p, side, +1, +1, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions2(p, side, +1, -1, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions2(p, side, -1, +1, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions2(p, side, -1, -1, 7));

    return bitboard;
}

PositionList Rules::GetRockGuardedPositions(Figure *figure) const
{    
    POSITION p = figure->Position;
    FigureSide side = figure->Side;

    PositionList guarded;

    guarded.append(GetOnLinePositions(p, side, +1, 0, 7)); // right
    guarded.append(GetOnLinePositions(p, side, -1, 0, 7)); // left
    guarded.append(GetOnLinePositions(p, side, 0, +1, 7)); // forward
    guarded.append(GetOnLinePositions(p, side, 0, -1, 7)); // backward

    return guarded;
}

BITBOARD Rules::GetRockGuardedPositions2(Figure *figure) const
{
    POSITION p = figure->Position;
    FigureSide side = figure->Side;

    BITBOARD bitboard = BITBOARD_EMPTY;

    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions2(p, side, +1, 0, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions2(p, side, -1, 0, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions2(p, side, 0, +1, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions2(p, side, 0, -1, 7));

    return bitboard;
}

PositionList Rules::GetQueenGuardedPositions(Figure *figure) const
{
    POSITION p = figure->Position;
    FigureSide side = figure->Side;

    PositionList guarded;

    guarded.append(GetOnLinePositions(p, side, +1, +1, 7)); // forward-right
    guarded.append(GetOnLinePositions(p, side, +1, -1, 7)); // backward-right
    guarded.append(GetOnLinePositions(p, side, -1, +1, 7)); // forawrd-left
    guarded.append(GetOnLinePositions(p, side, -1, -1, 7)); // forward-right
    guarded.append(GetOnLinePositions(p, side, +1, 0, 7)); // right
    guarded.append(GetOnLinePositions(p, side, -1, 0, 7)); // left
    guarded.append(GetOnLinePositions(p, side, 0, +1, 7)); // forward
    guarded.append(GetOnLinePositions(p, side, 0, -1, 7)); // backward

    return guarded;
}

BITBOARD Rules::GetQueenGuardedPositions2(Figure *figure) const
{
    POSITION p = figure->Position;
    FigureSide side = figure->Side;

    BITBOARD bitboard = BITBOARD_EMPTY;

    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions2(p, side, +1, 0, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions2(p, side, -1, 0, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions2(p, side, 0, +1, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions2(p, side, 0, -1, 7));

    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions2(p, side, +1, +1, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions2(p, side, +1, -1, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions2(p, side, -1, +1, 7));
    bitboard = BitboardHelper::Union(bitboard, GetOnLinePositions2(p, side, -1, -1, 7));

    return bitboard;
}

PositionList Rules::GetKingGuardedPositions(Figure *figure) const
{
    POSITION p = figure->Position;
    FigureSide side = figure->Side;

    PositionList guarded;

    Helpers::AppendIfValid(guarded, ForwardFor(p, side, +1, +1)); // forward-right
    Helpers::AppendIfValid(guarded, ForwardFor(p, side, +1, -1)); // backward-right
    Helpers::AppendIfValid(guarded, ForwardFor(p, side, -1, +1)); // forawrd-left
    Helpers::AppendIfValid(guarded, ForwardFor(p, side, -1, -1)); // forward-right
    Helpers::AppendIfValid(guarded, ForwardFor(p, side, +1, 0)); // right
    Helpers::AppendIfValid(guarded, ForwardFor(p, side, -1, 0)); // left
    Helpers::AppendIfValid(guarded, ForwardFor(p, side, 0, +1)); // forward
    Helpers::AppendIfValid(guarded, ForwardFor(p, side, 0, -1)); // backward

    return guarded;
}

BITBOARD Rules::GetKingGuardedPositions2(Figure *figure) const
{
    POSITION p = figure->Position;
    BITBOARD bitboard = BITBOARD_EMPTY;

    bitboard = BitboardHelper::AddPosition(bitboard, PositionHelper::Shift(p, +1, +1));
    bitboard = BitboardHelper::AddPosition(bitboard, PositionHelper::Shift(p, +1, -1));
    bitboard = BitboardHelper::AddPosition(bitboard, PositionHelper::Shift(p, -1, +1));
    bitboard = BitboardHelper::AddPosition(bitboard, PositionHelper::Shift(p, -1, -1));

    bitboard = BitboardHelper::AddPosition(bitboard, PositionHelper::Shift(p, +1, 0));
    bitboard = BitboardHelper::AddPosition(bitboard, PositionHelper::Shift(p, -1, 0));
    bitboard = BitboardHelper::AddPosition(bitboard, PositionHelper::Shift(p, 0, +1));
    bitboard = BitboardHelper::AddPosition(bitboard, PositionHelper::Shift(p, 0, -1));

    return bitboard;
}

bool Rules::IsUnderCheck(FigureSide side) const
{
    //return IsUnderCheckImpl(side);
    return IsUnderCheckFastImpl(side);
}

bool Rules::IsPassiveEndGame() const
{
    return m_board->GetCurrentPositionCount() >= 3
            || m_board->GetAfterLastCaptureOrPawnMoveHalfMoveCount() >= 50;
}

PositionList Rules::GetGuardedPositions(Figure *figure) const
{
    switch (figure->Type)
    {
        case FigureType::Pawn:      return GetPawnGuardedPositions(figure);
        case FigureType::Knight:    return GetKinghtGuardedPositions(figure);
        case FigureType::Bishop:    return GetBishopGuardedPositions(figure);
        case FigureType::Rock:      return GetRockGuardedPositions(figure);
        case FigureType::Queen:     return GetQueenGuardedPositions(figure);
        case FigureType::King:      return GetKingGuardedPositions(figure);

        default: throw Exception("Unknown figure type");
    }
}

BITBOARD Rules::GetGuardedPositions2(Figure *figure) const
{
    switch (figure->Type)
    {
        case FigureType::Pawn:      return GetPawnGuardedPositions2(figure);
        case FigureType::Knight:    return GetKinghtGuardedPositions2(figure);
        case FigureType::Bishop:    return GetBishopGuardedPositions2(figure);
        case FigureType::Rock:      return GetRockGuardedPositions2(figure);
        case FigureType::Queen:     return GetQueenGuardedPositions2(figure);
        case FigureType::King:      return GetKingGuardedPositions2(figure);

        default: throw Exception("Unknown figure type");
    }
}

POSITION Rules::ForwardFor(POSITION position, FigureSide side, int dx, int dy) const
{    
    return side == FigureSide::White ?
                PositionHelper::Shift(position, dx, dy)
              : PositionHelper::Shift(position, dx, -dy);
}

int Rules::FirstHorizonatalYFor(FigureSide side) const
{
    return side == FigureSide::White ? 1 : 8;
}

int Rules::PawnPromotionYFor(FigureSide side) const
{
    return side == FigureSide::White ? 8 : 1;
}

int Rules::EnPassantPawnYFor(FigureSide side) const
{
    return side == FigureSide::White ? 5 : 4;
}

bool Rules::IsUnderCheckImpl(FigureSide side) const
{
    PositionList opponentGuarded = GetGuardedPositions(OpponentSide(side));

    Figure* king = m_board->KingAt(side);

    return opponentGuarded.contains(king->Position);
}

Figure* Rules::GetObstacleInDirection(POSITION position, FigureSide side, int xMult, int yMult) const
{
    if (xMult != -1 && xMult != 0 && xMult != 1 || yMult != -1 && yMult != 0 && yMult != 1)
    {
        throw Exception("Only {-1, 0, 1} values are valid for x and y mult");
    }

    for (int delta = 1; delta <= 7; ++delta)
    {
        POSITION p = ForwardFor(position, side, delta * xMult, delta * yMult);

        if (!PositionHelper::IsInvalid(p))
        {
            Figure* figure = m_board->FigureAt(p);

            if (figure != NULL) // obstacle finded
                return figure;
        } else
        {
            break;
        }
    }

    return NULL;
}

bool Rules::IsUnderCheckFastImpl(FigureSide side) const
{
    Figure* king = m_board->KingAt(side);
    POSITION p = king->Position;
    FigureSide opponentSide = OpponentSide(side);

    // checks for knight threat
    QVarLengthArray<POSITION> knightCheckPositions;

    Helpers::AppendIfValid(knightCheckPositions, ForwardFor(p, side, +2, +1));
    Helpers::AppendIfValid(knightCheckPositions, ForwardFor(p, side, +1, +2));
    Helpers::AppendIfValid(knightCheckPositions, ForwardFor(p, side, -1, +2));
    Helpers::AppendIfValid(knightCheckPositions, ForwardFor(p, side, -2, +1));
    Helpers::AppendIfValid(knightCheckPositions, ForwardFor(p, side, -2, -1));
    Helpers::AppendIfValid(knightCheckPositions, ForwardFor(p, side, -1, -2));
    Helpers::AppendIfValid(knightCheckPositions, ForwardFor(p, side, +1, -2));
    Helpers::AppendIfValid(knightCheckPositions, ForwardFor(p, side, +2, -1));

    foreach(POSITION position, knightCheckPositions)
    {
        Figure* figure = m_board->FigureAt(position);

        if (figure != NULL && figure->Type == FigureType::Knight && figure->Side == opponentSide)
        {
            return true;
        }
    }

    // checks for long-range figures and pawns

    // check for enemy rocks and queen in non tilted directions
    QVarLengthArray<Figure*> nonTiltedDirectionObstacles;
    nonTiltedDirectionObstacles.reserve(4); // to speed up

    Helpers::AppendIfNotNull(nonTiltedDirectionObstacles, GetObstacleInDirection(p, side, 0, +1)); // forward direction
    Helpers::AppendIfNotNull(nonTiltedDirectionObstacles, GetObstacleInDirection(p, side, 0, -1)); // backward direction
    Helpers::AppendIfNotNull(nonTiltedDirectionObstacles, GetObstacleInDirection(p, side, +1, 0)); // x incresing direction
    Helpers::AppendIfNotNull(nonTiltedDirectionObstacles, GetObstacleInDirection(p, side, -1, 0)); // x decreasing direction

    foreach(Figure* figure, nonTiltedDirectionObstacles)
    {
        if (figure->Side == opponentSide)
        {
            if (figure->Type == FigureType::Queen || figure->Type == FigureType::Rock)
            {
                return true;
            } else if (figure->Type == FigureType::King)
            {
                int dx = abs(PositionHelper::X(p) - PositionHelper::X(figure->Position));
                int dy = abs(PositionHelper::Y(p) - PositionHelper::Y(figure->Position));

                bool isNeighbor = std::max(dx, dy) == 1;

                if (isNeighbor)
                {
                    return true;
                }
            }
        }
    }

    // check for enemy bishops, queen and pawns in tilted direction
    QVarLengthArray<Figure*> tiltedDirectionObstacles;
    tiltedDirectionObstacles.reserve(4); // to speed up

    Helpers::AppendIfNotNull(tiltedDirectionObstacles, GetObstacleInDirection(p, side, +1, +1)); // forward+ direction
    Helpers::AppendIfNotNull(tiltedDirectionObstacles, GetObstacleInDirection(p, side, +1, -1)); // backward+ direction
    Helpers::AppendIfNotNull(tiltedDirectionObstacles, GetObstacleInDirection(p, side, -1, +1)); // forward- direction
    Helpers::AppendIfNotNull(tiltedDirectionObstacles, GetObstacleInDirection(p, side, -1, -1)); // backward- direction

    //PositionList pawnThreatPotentialyPositions;
    //AppendIfValid(&pawnThreatPotentialyPositions, ForwardFor(p, side, +1, +1));
    //AppendIfValid(&pawnThreatPotentialyPositions, ForwardFor(p, side, -1, +1));

    foreach(Figure* figure, tiltedDirectionObstacles)
    {
        if (figure->Side == opponentSide)
        {
            if (figure->Type == FigureType::Queen || figure->Type == FigureType::Bishop)
            {
                return true;
            }
            else if (figure->Type == FigureType::Pawn)
            {
                bool isPawnForwardByOne = (PositionHelper::Y(figure->Position) - PositionHelper::Y(p)) == (side == FigureSide::White ? 1 : -1);

                if (isPawnForwardByOne && abs(PositionHelper::X(figure->Position) - PositionHelper::X(p)) == 1)
                {
                    return true;
                }
            } else if (figure->Type == FigureType::King)
            {
                int dx = abs(PositionHelper::X(p) - PositionHelper::X(figure->Position));
                int dy = abs(PositionHelper::Y(p) - PositionHelper::Y(figure->Position));

                bool isNeighbor = std::max(dx, dy) == 1;

                if (isNeighbor)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

FigureSide Rules::OpponentSide(FigureSide side) const
{
    return side == FigureSide::White ?
                FigureSide::Black
              : FigureSide::White;
}

MoveCollection& Rules::DeleteMovesToCheck(MoveCollection& moves)
{
    MoveCollection::iterator it = moves.begin();

    while (it != moves.end())
    {
        Move move = *it;

        // temporary make move to verify that it doesn't causes check
        MakeMove(move);

        // when check after move
        if (IsUnderCheck(move.MovingFigure->Side))
        {
            it = moves.erase(it); // erase it
        } else
        {
            ++it;
        }

        // unmake temporary move
        UnMakeMove(move);
    }

    return moves;
}

PositionList Rules::_GetPossibleDestinations(Figure *figure) const
{
    PositionList list;

    switch (figure->Type)
    {
        case FigureType::Pawn:
            list = _GetPawnPossibleDestinations(figure);
            break;
        case FigureType::Knight:
            list = _GetKnightPossibleDestinations(figure);
            break;
        case FigureType::Bishop:
            list = _GetBishopPossibleDestinations(figure);
            break;
        case FigureType::Rock:
            list = _GetRockPossibleDestinations(figure);
            break;
        case FigureType::Queen:
            list = _GetQueenPossibleDestinations(figure);
            break;
        case FigureType::King:
            list = _GetKingPossibleDestinations(figure);
            break;
        default:
            throw Exception("Unknown figure type");
    }

    DeleteSelfCaptureDesination(&list, figure->Side);

    return list;
}

void Rules::DeleteSelfCaptureDesination(PositionList *destinations, FigureSide selfSide) const
{
    PositionList::iterator it = destinations->begin();
    while (it != destinations->end())
    {
        if (m_board->HasFigureAt(*it, selfSide)) // if same side figure here
        {
            it = destinations->erase(it); // erase it
        }
        else
        {
            ++it;
        }
    }
}

PositionList Rules::_GetPawnPossibleDestinations(Figure *figure) const
{
    PositionList destinations;

    // captures handling
    PositionList guarded = GetPawnGuardedPositions(figure);
    foreach (POSITION p, guarded)
    {
        Figure* pFigure = m_board->FigureAt(p);

        // pawn captures another figure
        if (pFigure != NULL && pFigure->Side == OpponentSide(figure->Side))
        {
            destinations.append(p);
        }
    }

    POSITION pShort = ForwardFor(figure->Position, figure->Side, 0, 1);
    if (!m_board->HasFigureAt(pShort))
    {
        destinations.append(pShort);

        if (figure->MovesCount == 0) // long first pawn turn handling
        {
            POSITION pLong = ForwardFor(figure->Position, figure->Side, 0, 2);

            if (!m_board->HasFigureAt(pLong))
            {
                destinations.append(pLong);
            }
        }
    }

    // en passant handling
    // can be only on 5 horizontal for white, and only on 4 for black side
    // also en passant can be only immediately after opponent crossing pawn guarded cell
    int enPassantY = EnPassantPawnYFor(figure->Side);
    int figureX = PositionHelper::X(figure->Position);
    if (PositionHelper::Y(figure->Position) == enPassantY)
    {
        Move opponentLastMove = m_board->GetLastMove();

        if (opponentLastMove.Type == MoveType::LongPawn)
        {
            POSITION opponentStartPawnPosition = opponentLastMove.From;
            int opponentPawnX = PositionHelper::X(opponentStartPawnPosition);
            FigureSide opponentSide = OpponentSide(figure->Side);

            if (opponentPawnX == figureX + 1 || opponentPawnX == figureX - 1)
            {
                POSITION enPassantDestination = ForwardFor(opponentStartPawnPosition, opponentSide, 0, 1);
                destinations.append(enPassantDestination);
            }
        }
    }

    return destinations;
}

PositionList Rules::_GetKnightPossibleDestinations(Figure *figure) const
{
    // yeah! is matches with knight guarded positions
    return GetKinghtGuardedPositions(figure);
}

PositionList Rules::_GetBishopPossibleDestinations(Figure *figure) const
{
    // also matches bishop guarded positions
    return GetBishopGuardedPositions(figure);
}

PositionList Rules::_GetRockPossibleDestinations(Figure *figure) const
{
    // matches rock guarded positions
    return GetRockGuardedPositions(figure);
}

PositionList Rules::_GetQueenPossibleDestinations(Figure *figure) const
{
    // matches queen guarded positions î_Î
    return GetQueenGuardedPositions(figure);
}

PositionList Rules::_GetKingPossibleDestinations(Figure *king) const
{
    PositionList destinations = GetKingGuardedPositions(king);
    PositionList opponentGuarded = GetGuardedPositions(OpponentSide(king->Side));
    POSITION currentKingPosition = king->Position;

    // check for short castling
    POSITION r1 = PositionHelper::Shift(king->Position, 1, 0);
    POSITION r2 = PositionHelper::Shift(king->Position, 2, 0);
    Figure* rRock = m_board->FigureAt(PositionHelper::Create(8,FirstHorizonatalYFor(king->Side)));

    if (king->MovesCount == 0
            && rRock != NULL
            && rRock->MovesCount == 0
            && !m_board->HasFigureAt(r1) && !m_board->HasFigureAt(r2)
            && !opponentGuarded.contains(currentKingPosition) // castling from check is forbidden
            && !opponentGuarded.contains(r1)   // castling through guarded cells is forbidden
            && !opponentGuarded.contains(r2))  // also

    {
            destinations.append(r2);
    }

    // long castling checks
    POSITION l1 = PositionHelper::Shift(king->Position, -1, 0);
    POSITION l2 = PositionHelper::Shift(king->Position, -2, 0);
    POSITION l3 = PositionHelper::Shift(king->Position, -3, 0);
    Figure* lRock = m_board->FigureAt(PositionHelper::Create(1, FirstHorizonatalYFor(king->Side)));

    if (king->MovesCount == 0
            && lRock != NULL
            && lRock->MovesCount == 0
            && !m_board->HasFigureAt(l1) && !m_board->HasFigureAt(l2) && !m_board->HasFigureAt(l3)
            && !opponentGuarded.contains(currentKingPosition) // castling from check is forbidden
            && !opponentGuarded.contains(l1)   // castling through guarded cells is forbidden
            && !opponentGuarded.contains(l2)   // also
            && !opponentGuarded.contains(l3))  // also

    {
            destinations.append(l2);
    }

    return destinations;
}

Move Rules::CreateMove(POSITION from, POSITION to)
{
    MoveType moveType;

    Figure* captured = m_board->FigureAt(to);
    Figure* figure = m_board->FigureAt(from);
    FigureType figureType = figure->Type;

    // long king step -> castling
    if (figureType == FigureType::King && abs(PositionHelper::X(to) - PositionHelper::X(from)) == 2)
    {
        if (PositionHelper::X(to) > PositionHelper::X(from))
            moveType = MoveType::ShortCastling;
        else
            moveType = MoveType::LongCastling;
    }
    // long pawn turn handling
    else if (figureType == FigureType::Pawn && abs(PositionHelper::Y(to) - PositionHelper::Y(from)) == 2)
    {
        moveType = MoveType::LongPawn;
    }
    // pawn's position x changing and target cell is empty -> en passant
    else if (figureType == FigureType::Pawn && !m_board->HasFigureAt(to) && PositionHelper::X(to) != PositionHelper::X(from))
    {
        moveType = MoveType::EnPassant;
        captured = m_board->FigureAt(ForwardFor(to, figure->Side, 0, -1));
    }
    // pawn promote
    else if (figureType == FigureType::Pawn && PositionHelper::Y(to) == PawnPromotionYFor(figure->Side))
    {
        moveType = MoveType::PawnPromotion;
    } else // only Normal and Capture turns
    {
        if (captured != NULL)
        {
            moveType = MoveType::Capture;
        } else
        {
            moveType = MoveType::Normal;
        }
    }

    return Move(moveType, from, to, figure, captured);
}

PositionList Rules::GetGuardedPositions(FigureSide side) const
{
    FigureList figures = m_board->FiguresAt(side);

    PositionList guarded;

    foreach(Figure* figure, figures)
    {
        PositionList currentFigureGuarded = GetGuardedPositions(figure);

        Helpers::AppendIfNotExists(guarded, currentFigureGuarded);
    }

    return guarded;
}

BITBOARD Rules::GetGuardedPositions2(FigureSide side) const
{
    FigureList figures = m_board->FiguresAt(side);

    BITBOARD bitboard = BITBOARD_EMPTY;

    foreach(Figure* figure, figures)
    {
        bitboard = BitboardHelper::Union(bitboard, GetGuardedPositions2(figure));
    }

    return bitboard;
}

MoveCollection Rules::GetPossibleMoves(FigureSide side)
{
    FigureList figures = m_board->FiguresAt(side);
    MoveCollection moves;

    if (m_board->GetTurningSide() != side)
    {
        return moves;
    }

    foreach(Figure* figure, figures)
    {
        PositionList currentFigurePossibleDestinations = _GetPossibleDestinations(figure);

        foreach(POSITION curFigurePossiblePosition, currentFigurePossibleDestinations)
        {
            Move currentMove = CreateMove(figure->Position, curFigurePossiblePosition);

            moves.append(currentMove);
        }
    }

    DeleteMovesToCheck(moves);

    return moves;
}

MoveCollection Rules::GetPossibleMoves(Figure *figure)
{
    MoveCollection moves = GetPossibleMoves(figure->Side);

    MoveCollection::iterator it = moves.begin();

    while (it != moves.end())
    {
        if ((*it).MovingFigure != figure)
        {
            it = moves.erase(it);
        }
        else
        {
            ++it;
        }
    }

    return moves;
}

PositionList Rules::GetPossibleDestinations(Figure *figure)
{
    MoveCollection moves = GetPossibleMoves(figure);
    PositionList destinations;

    foreach(const Move& move, moves)
    {
        destinations.append(move.To);
    }

    return destinations;
}

void Rules::MakeMove(Move move)
{
    // turn transition
    m_board->TurnTransition();

    // handle move count
    move.MovingFigure->MovesCount++;

    // capture enemy figure if needed
    if (move.CapturedFigure != NULL)
    {
        m_board->KillFigure(move.CapturedFigure);
    }

    // moving own figure
    m_board->MoveFigure(move.MovingFigure, move.To);

    // if castling move accordingly rock
    if (move.IsCastling())
    {
        int y = PositionHelper::Y(move.MovingFigure->Position);
        int rockBeforeCastlingX;
        int rockAfterCastringX;

        if (move.Type == MoveType::ShortCastling)
        {
            rockBeforeCastlingX = 8;
            rockAfterCastringX = 6;
        } else // LongCastling
        {
            rockBeforeCastlingX = 1;
            rockAfterCastringX = 4;
        }

        Figure* rock = m_board->FigureAt(PositionHelper::Create(rockBeforeCastlingX, y));

        // move rock
        m_board->MoveFigure(rock, PositionHelper::Create(rockAfterCastringX, y));
    } else if (move.Type == MoveType::PawnPromotion)
    {
        m_board->PromotePawn(move.MovingFigure, FigureType::Queen);
    }

    // add to history
    m_board->PushToHistory(move);

    // position counting to detect draw game situations
    m_board->IncreaseCurrentPositionCount();
}

void Rules::MakeMove(POSITION from, POSITION to)
{
    Figure* f = m_board->FigureAt(from);

    if (f != NULL)
    {
        MoveCollection possible = GetPossibleMoves(f);
        bool isValidMove = false;

        foreach(const Move& move, possible)
        {
            if (move.From == from && move.To == to)
            {
                MakeMove(move);
                isValidMove = true;
                break;
            }
        }

        if (!isValidMove)
        {
            // if we get were then move not finded
            throw Exception(QString("Move from %1 to %2 is incorrect.").arg(PositionHelper::ToString(from), PositionHelper::ToString(to)).toStdString());
        }
    } else
    {
        throw Exception("Invalid source position");
    }
}

void Rules::UnMakeMove(Move move)
{
    // position counting to detect draw game situations
    m_board->DecreaseCurrentPositionCount();

    // turn transition
    m_board->TurnTransition();

    // remove from history
    m_board->PopHistory();

    // handle move count
    move.MovingFigure->MovesCount--;

    // unmoving own figure
    m_board->MoveFigure(move.MovingFigure, move.From);

    // uncapture enemy figure if needed
    if (move.CapturedFigure != NULL)
    {
        m_board->ResurrectFigure(move.CapturedFigure);
    }    

    // if castling move rock back
    if (move.IsCastling())
    {
        int y = PositionHelper::Y(move.MovingFigure->Position);
        int rockBeforeCastlingX;
        int rockAfterCastringX;

        if (move.Type == MoveType::ShortCastling)
        {
            rockBeforeCastlingX = 8;
            rockAfterCastringX = 6;
        } else // LongCastling
        {
            rockBeforeCastlingX = 1;
            rockAfterCastringX = 4;
        }

        Figure* rock = m_board->FigureAt(PositionHelper::Create(rockAfterCastringX, y));

        // move rock
        m_board->MoveFigure(rock, PositionHelper::Create(rockBeforeCastlingX, y));
    } else if (move.Type == MoveType::PawnPromotion)
    {
        m_board->UnpromotePawn(move.MovingFigure);
    }
}
