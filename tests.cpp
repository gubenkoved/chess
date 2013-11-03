#define TEST_EXPAND(x) if (TestIt(#x, &Tests:: x)) { ++ passed; } else {++ failed; }

#include <string>
#include <stdexcept>
#include <QMap>
#include <QTime>

#include "exception.h"
#include "tests.h"
#include "board.h"
#include "figure.h"
#include "rules.h"
#include "ai.h"
#include "boardserializer.h"
#include "fen.h"

Tests::Tests()
{
}

void Tests::TestAll()
{
    qDebug() << "Tests::TestAll...";

    int passed = 0, failed = 0;

    TEST_EXPAND(TestFigurePosition1)
    TEST_EXPAND(TestFigurePosition2)
    TEST_EXPAND(TestFigurePosition3)
    TEST_EXPAND(TestFigurePosition4)
    TEST_EXPAND(TestFigurePosition5)

    TEST_EXPAND(BoardStartPositionTest)
    TEST_EXPAND(BoardGetSideFiguresTest)

    TEST_EXPAND(GetGuardedPositionsTest1)
    TEST_EXPAND(GetGuardedPositionsTest2)
    TEST_EXPAND(GetGuardedPositionsTest3)
    TEST_EXPAND(GetGuardedPositionsTest4)
    TEST_EXPAND(GetGuardedPositionsTest5)
    TEST_EXPAND(GetGuardedPositionsTest6)

    TEST_EXPAND(MoveFigureTest)

    TEST_EXPAND(MakeMoveTest1)
    TEST_EXPAND(MakeMoveTest2)

    TEST_EXPAND(MoveFindingTest1)
    TEST_EXPAND(MoveFindingTest2)
    TEST_EXPAND(MoveFindingTest3)

    TEST_EXPAND(EnPassonTest1)
    TEST_EXPAND(EnPassonTest2)

    TEST_EXPAND(BoardDeepCopyTest);

    TEST_EXPAND(ToFENTest1);
    TEST_EXPAND(ToFENTest2);
    TEST_EXPAND(ToFENTest3);
    TEST_EXPAND(ToFENTest4);

//    TEST_EXPAND(FromFENTest1);
//    TEST_EXPAND(FromFENTest2);
//    TEST_EXPAND(FromFENTest3);
//    TEST_EXPAND(FromFENTest4);

    TEST_EXPAND(PerftTest1)
    TEST_EXPAND(PerftTest2)
    TEST_EXPAND(PerftTest3)
    //TEST_EXPAND(PerftTest4)

    TEST_EXPAND(ABFromStartD1)
    TEST_EXPAND(ABFromStartD2)
    TEST_EXPAND(ABFromStartD3)
    //TEST_EXPAND(ABFromStartD4)
    //TEST_EXPAND(ABFromStartD5)
    //TEST_EXPAND(ABFromStartD6)

    //TEST_EXPAND(ABAfter6PlyD5)
    //TEST_EXPAND(ABAfter6PlyD6)

    TEST_EXPAND(AlphaBetaTest1)
    TEST_EXPAND(AlphaBetaTest2)
    TEST_EXPAND(AlphaBetaTest3)
    //TEST_EXPAND(AlphaBetaTest4)

    TEST_EXPAND(AlphaBetaTest_2_1)
    TEST_EXPAND(AlphaBetaTest_2_2)
    TEST_EXPAND(AlphaBetaTest_2_3)
    //TEST_EXPAND(AlphaBetaTest_2_4)

    TEST_EXPAND(PositionCountingTest1)
    TEST_EXPAND(PositionCountingTest2)

    TEST_EXPAND(BoardSerializationTest)

    TEST_EXPAND(WM1)

    qDebug() << "Passed:" << passed << "test(s)";
    qDebug() << "Failed:" << failed << "test(s)";
}

bool Tests::TestIt(QString str, bool (Tests::*method)(void))
{    
    bool res = false;
    bool exc = true;
    std::string excMessage;

    QString timeMessage;
    QTime timer;
    timer.start();
    int milliseconds = 0;

    try
    {
        res = (this->*method)();
        exc = false;
        milliseconds = timer.elapsed();
    }
    catch (Exception exception)
    {
        excMessage = exception.GetMessage();
    }
    catch (std::overflow_error e)
    {
        excMessage = "std::overflow_error";
    }
    catch (std::out_of_range e)
    {
        excMessage = "std::out_of_range";
    }
    catch (std::range_error e)
    {
        excMessage = "std::rande_error";
    }
    catch (std::exception e)
    {
        excMessage = "std::exception";
    }
    catch (...)
    {
        excMessage = "common exception";
    }

    if (exc)
    {
        excMessage = "(exception: " + excMessage + ")";
    }
    else
    {
        timeMessage = "[" + QString::number(milliseconds) + "ms]";
    }

    qDebug() << " " << str.toStdString().c_str()
             << QString(".").repeated(50 - str.length()).toStdString().c_str()
             << (res ? "Passed" : "FAILED")
             << timeMessage.toStdString().c_str()
             << (exc ? excMessage.c_str() : std::string("").c_str());

    return res;
}

bool Tests::TestFigurePosition1()
{
    POSITION p = CreateFigurePosition("a1");

    return X(p) == 1 && Y(p) == 1;
}
bool Tests::TestFigurePosition2()
{
    POSITION p = CreateFigurePosition("h8");

    return X(p) == 8 && Y(p) == 8;
}
bool Tests::TestFigurePosition3()
{
    POSITION p = CreateFigurePosition("e2");

    return X(p) == 5 && Y(p) == 2;
}
bool Tests::TestFigurePosition4()
{
    POSITION p = CreateFigurePosition("a0");

    return IsInvalid(p);
}
bool Tests::TestFigurePosition5()
{
    POSITION p = CreateFigurePosition("h9");

     return IsInvalid(p);
}

bool Tests::BoardStartPositionTest()
{
    Board board = Board::StartPosition();

    QMap<FigureType, int> counter;

    for (int x = 1; x <= 8; ++x)
    {
        for(int y = 1; y <= 8; ++y)
        {
            POSITION pos = CreateFigurePosition(x, y);
            Figure* figure = board.FigureAt(pos);

            if (figure != NULL)
            {
                if (!counter.contains(figure->Type))
                    counter[figure->Type] = 0;

                counter[figure->Type] += 1;
            }
        }
    }    

    return counter[FigureType::Pawn] == 16
            && counter[FigureType::Rock] == 4
            && counter[FigureType::Bishop] == 4
            && counter[FigureType::Knight] == 4
            && counter[FigureType::King] == 2
            && counter[FigureType::Queen] == 2;
}

bool Tests::BoardGetSideFiguresTest()
{
    Board board = Board::StartPosition();

    return board.FiguresAt(FigureSide::White).count() == 16
            && board.FiguresAt(FigureSide::Black).count() == 16;
}

bool Tests::GetGuardedPositionsTest1()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);

    PositionList guarded = rules.GetGuardedPositions(FigureSide::White);

    return guarded.count() == 22;
}

bool Tests::GetGuardedPositionsTest2()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);

    PositionList guarded = rules.GetGuardedPositions(FigureSide::Black);

    return guarded.count() == 22;
}

bool Tests::GetGuardedPositionsTest3()
{
    Board board = Board();
    Rules rules = Rules(&board);

    board.AddAliveFigure(new Figure(FigureSide::White, FigureType::Knight, CreateFigurePosition("e5")));

    PositionList guarded = rules.GetGuardedPositions(FigureSide::White);

    return guarded.count() == 8;
}
bool Tests::GetGuardedPositionsTest4()
{
    Board board = Board();
    Rules rules = Rules(&board);

    board.AddAliveFigure(new Figure(FigureSide::White, FigureType::Rock, CreateFigurePosition("e4")));

    PositionList guarded = rules.GetGuardedPositions(FigureSide::White);

    return guarded.count() == 14;
}
bool Tests::GetGuardedPositionsTest5()
{
    Board board = Board();
    Rules rules = Rules(&board);

    board.AddAliveFigure(new Figure(FigureSide::White, FigureType::Queen, CreateFigurePosition("b2")));

    PositionList guarded = rules.GetGuardedPositions(FigureSide::White);

    return guarded.count() == 23;
}
bool Tests::GetGuardedPositionsTest6()
{
    Board board = Board();
    Rules rules = Rules(&board);

    board.AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, CreateFigurePosition("h7")));

    PositionList guarded = rules.GetGuardedPositions(FigureSide::White);

    return guarded.count() == 1;
}

bool Tests::MoveFigureTest()
{
    Board board = Board();

    board.SetupStartPosition();

    Figure* pawn = board.FigureAt(CreateFigurePosition("e2"));
    board.MoveFigure(pawn, CreateFigurePosition("e4"));

    return board.FigureAt(CreateFigurePosition("e4")) == pawn;
}

bool Tests::MakeMoveTest1()
{
    Board board = Board();
    Rules rules = Rules(&board);

    board.SetupStartPosition();

    Figure* pawn = board.FigureAt(CreateFigurePosition("e2"));

    rules.MakeMove(CreateFigurePosition("e2"), CreateFigurePosition("e4"));

    return board.FigureAt(CreateFigurePosition("e4")) == pawn;
}

bool Tests::MakeMoveTest2()
{
    return true;
}

bool Tests::MoveFindingTest1()
{
    //Board board = Board::StartPosition();
    Board board = Board();
    Rules rules = Rules(&board);

    board.SetupStartPosition();

    MoveList moves = rules.GetPossibleMoves(FigureSide::White);

    return moves.count() == 20;
}

bool Tests::MoveFindingTest2()
{    
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);

    MoveList moves = rules.GetPossibleMoves(board.FigureAt(CreateFigurePosition("e2")));

    return moves.count() == 2
            && moves[0].To == CreateFigurePosition("e3")
            && moves[1].To == CreateFigurePosition("e4");
}

bool Tests::MoveFindingTest3()
{
    Board board;
    Rules rules = Rules(&board);

    board.AddAliveFigure(new Figure(FigureSide::White, FigureType::King, CreateFigurePosition("d4")));
    board.AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, CreateFigurePosition("e4")));
    board.AddAliveFigure(new Figure(FigureSide::Black, FigureType::King, CreateFigurePosition("f4")));

    return rules.GetPossibleMoves(FigureSide::White).count() == 4;
}

bool Tests::EnPassonTest1()
{
    Board board = Board();
    Rules rules = Rules(&board);

    board.SetupKings();
    board.TurnTransition(); // to black

    board.AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, CreateFigurePosition("e5")));
    board.AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, CreateFigurePosition("d7")));

    rules.MakeMove(CreateFigurePosition("d7"), CreateFigurePosition("d5"));

    // check for enPasson availability
    return rules.GetPossibleDestinations(board.FigureAt(CreateFigurePosition("e5"))).contains(CreateFigurePosition("d6"));
}

bool Tests::EnPassonTest2()
{
    Board board = Board();
    Rules rules = Rules(&board);

    board.SetupKings();

    board.AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, CreateFigurePosition("e2")));
    board.AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, CreateFigurePosition("d4")));

    rules.MakeMove(CreateFigurePosition("e2"), CreateFigurePosition("e4"));

    // check for enPasson availability
    return rules.GetPossibleDestinations(board.FigureAt(CreateFigurePosition("d4"))).contains(CreateFigurePosition("e3"));
}

bool Tests::BoardDeepCopyTest()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);

    rules.MakeMove(CreateFigurePosition("e2"), CreateFigurePosition("e4")); // w
    rules.MakeMove(CreateFigurePosition("d7"), CreateFigurePosition("d5")); // b
    rules.MakeMove(CreateFigurePosition("e4"), CreateFigurePosition("d5")); // w pawn captures b pawn
    rules.MakeMove(CreateFigurePosition("c8"), CreateFigurePosition("e6")); // b bishop
    rules.MakeMove(CreateFigurePosition("d2"), CreateFigurePosition("d4")); // w pawn long
    rules.MakeMove(CreateFigurePosition("e6"), CreateFigurePosition("d5")); // b bishop captures w pawn
    // white's turn

    Board copy = Board(board);
    Rules copyRules = Rules(&copy);

    // checks what figuers has been copied by value, not by reference
    FigureList boardFigures = board.FiguresAt(FigureSide::White); boardFigures.append(board.FiguresAt(FigureSide::Black));
    FigureList copyBoardFigures = copy.FiguresAt(FigureSide::White); copyBoardFigures.append(copy.FiguresAt(FigureSide::Black));

    if (boardFigures.count() != copyBoardFigures.count())
        return false;

    foreach (Figure* boardFigure, boardFigures)
    {
        // checks that figure is actualy copied
        if (copyBoardFigures.contains(boardFigure))
            return false;
    }

    // checks its independency

    // unmake all in copy
    copyRules.UnMakeMove(copy.GetLastMove());
    copyRules.UnMakeMove(copy.GetLastMove());
    copyRules.UnMakeMove(copy.GetLastMove());
    copyRules.UnMakeMove(copy.GetLastMove());
    copyRules.UnMakeMove(copy.GetLastMove());
    copyRules.UnMakeMove(copy.GetLastMove());

    if ((copy.FiguresAt(FigureSide::White).count() + copy.FiguresAt(FigureSide::Black).count()) != 32)
        return false;

    if ((board.FiguresAt(FigureSide::White).count() + board.FiguresAt(FigureSide::Black).count()) != 30)
        return false;

    // unmake all in old one
    rules.UnMakeMove(board.GetLastMove());
    rules.UnMakeMove(board.GetLastMove());
    rules.UnMakeMove(board.GetLastMove());
    rules.UnMakeMove(board.GetLastMove());
    rules.UnMakeMove(board.GetLastMove());
    rules.UnMakeMove(board.GetLastMove());

    if ((board.FiguresAt(FigureSide::White).count() + board.FiguresAt(FigureSide::Black).count()) != 32)
        return false;

    if ((copy.FiguresAt(FigureSide::White).count() + copy.FiguresAt(FigureSide::Black).count()) != 32)
        return false;

    return true;
}

bool Tests::ToFENTest1()
{
    Board board = Board::StartPosition();

    QString fen = FEN::Evaluate(board);
    QString rightFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    if (fen != rightFen)
    {
        qDebug() << "   Result FEN:" << fen;
        qDebug() << "   Right  FEN:" << rightFen;

        return false;
    }

    return true;
}

bool Tests::ToFENTest2()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);

    rules.MakeMove(CreateFigurePosition("e2"), CreateFigurePosition("e4"));

    QString fen = FEN::Evaluate(board);
    QString rightFen = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1";

    if (fen != rightFen)
    {
        qDebug() << "   Result FEN:" << fen;
        qDebug() << "   Right  FEN:" << rightFen;

        return false;
    }

    return true;
}

bool Tests::ToFENTest3()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);

    rules.MakeMove(CreateFigurePosition("e2"), CreateFigurePosition("e4"));
    rules.MakeMove(CreateFigurePosition("c7"), CreateFigurePosition("c5"));

    QString fen = FEN::Evaluate(board);
    QString rightFen = "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2";

    if (fen != rightFen)
    {
        qDebug() << "   Result FEN:" << fen;
        qDebug() << "   Right  FEN:" << rightFen;

        return false;
    }

    return true;
}

bool Tests::ToFENTest4()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);

    rules.MakeMove(CreateFigurePosition("e2"), CreateFigurePosition("e4"));
    rules.MakeMove(CreateFigurePosition("c7"), CreateFigurePosition("c5"));
    rules.MakeMove(CreateFigurePosition("g1"), CreateFigurePosition("f3"));

    QString fen = FEN::Evaluate(board);
    QString rightFen = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2";

    if (fen != rightFen)
    {
        qDebug() << "   Result FEN:" << fen;
        qDebug() << "   Right  FEN:" << rightFen;

        return false;
    }

    return true;
}

//bool Tests::FromFENTest1()
//{
//    QString fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
//    Board board = FEN::BoardFromFEN(fen);
//    QString evaluatedFen = FEN::Evaluate(&board);

//    if (fen != evaluatedFen)
//    {
//        qDebug() << "   Initial   FEN:" << fen;
//        qDebug() << "   Evaluated FEN:" << evaluatedFen;

//        return false;
//    }

//    return true;
//}

//bool Tests::FromFENTest2()
//{
//    QString fen = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1";
//    Board board = FEN::BoardFromFEN(fen);
//    QString evaluatedFen = FEN::Evaluate(&board);

//    if (fen != evaluatedFen)
//    {
//        qDebug() << "   Initial   FEN:" << fen;
//        qDebug() << "   Evaluated FEN:" << evaluatedFen;

//        return false;
//    }

//    return true;
//}

//bool Tests::FromFENTest3()
//{
//    QString fen = "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2";
//    Board board = FEN::BoardFromFEN(fen);
//    QString evaluatedFen = FEN::Evaluate(&board);

//    if (fen != evaluatedFen)
//    {
//        qDebug() << "   Initial   FEN:" << fen;
//        qDebug() << "   Evaluated FEN:" << evaluatedFen;

//        return false;
//    }

//    return true;
//}

//bool Tests::FromFENTest4()
//{
//    QString fen = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2";
//    Board board = FEN::BoardFromFEN(fen);
//    QString evaluatedFen = FEN::Evaluate(&board);

//    if (fen != evaluatedFen)
//    {
//        qDebug() << "   Initial   FEN:" << fen;
//        qDebug() << "   Evaluated FEN:" << evaluatedFen;

//        return false;
//    }

//    return true;
//}

bool Tests::PerftTest1()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);

    int analyzed;
    int bestEstimation;
    ai.NegamaxSearch(FigureSide::White, 1, bestEstimation, analyzed);
    //qDebug() << "   Analyzed:" << analized << ", best estimation:" << bestEstimation;

    return analyzed == 20;
}

bool Tests::PerftTest2()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);

    int analyzed;
    int bestEstimation;
    ai.NegamaxSearch(FigureSide::White, 2, bestEstimation, analyzed);
    //qDebug() << "   Analyzed:" << analized << ", best estimation:" << bestEstimation;

    return analyzed == 400;
}

bool Tests::PerftTest3()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);

    int analyzed;
    int bestEstimation;
    ai.NegamaxSearch(FigureSide::White, 3, bestEstimation, analyzed);
    //qDebug() << "   Analyzed:" << analized << ", best estimation:" << bestEstimation;

    return analyzed == 8902;
}

bool Tests::PerftTest4()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);

    int analyzed;
    int bestEstimation;
    ai.NegamaxSearch(FigureSide::White, 4, bestEstimation, analyzed);
    //qDebug() << "   Analyzed:" << analized << ", best estimation:" << bestEstimation;

    return analyzed == 197281;
}

bool Tests::ABFromStartD1()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);

    int analyzed;
    int bestEstimation;
    ai.BestMoveByAlphaBeta(FigureSide::White, 1, bestEstimation, analyzed);

    qDebug() << "   Analyzed:" << analyzed << ", best estimation:" << bestEstimation;

    return true;
}

bool Tests::ABFromStartD2()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);

    int analyzed;
    int bestEstimation;
    ai.BestMoveByAlphaBeta(FigureSide::White, 2, bestEstimation, analyzed);

    qDebug() << "   Analyzed:" << analyzed << ", best estimation:" << bestEstimation;

    return true;
}

bool Tests::ABFromStartD3()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);

    int analyzed;
    int bestEstimation;
    ai.BestMoveByAlphaBeta(FigureSide::White, 3, bestEstimation, analyzed);

    qDebug() << "   Analyzed:" << analyzed << ", best estimation:" << bestEstimation;

    return true;
}

bool Tests::ABFromStartD4()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);

    int analyzed;
    int bestEstimation;
    ai.BestMoveByAlphaBeta(FigureSide::White, 4, bestEstimation, analyzed);

    qDebug() << "   Analyzed:" << analyzed << ", best estimation:" << bestEstimation;

    return true;
}

bool Tests::ABFromStartD5()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);

    int analyzed;
    int bestEstimation;
    ai.BestMoveByAlphaBeta(FigureSide::White, 5, bestEstimation, analyzed);

    qDebug() << "   Analyzed:" << analyzed << ", best estimation:" << bestEstimation;

    return true;
}

bool Tests::ABFromStartD6()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);

    int analyzed;
    int bestEstimation;
    ai.BestMoveByAlphaBeta(FigureSide::White, 6, bestEstimation, analyzed);

    qDebug() << "   Analyzed:" << analyzed << ", best estimation:" << bestEstimation;

    return true;
}

bool Tests::ABFromStartD7()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);

    int analyzed;
    int bestEstimation;
    ai.BestMoveByAlphaBeta(FigureSide::White, 7, bestEstimation, analyzed);

    qDebug() << "   Analyzed:" << analyzed << ", best estimation:" << bestEstimation;

    return true;
}

bool Tests::ABAfter6PlyD5()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);

    rules.MakeMove(CreateFigurePosition("e2"), CreateFigurePosition("e4")); // w
    rules.MakeMove(CreateFigurePosition("d7"), CreateFigurePosition("d5")); // b
    rules.MakeMove(CreateFigurePosition("e4"), CreateFigurePosition("d5")); // w pawn captures b pawn
    rules.MakeMove(CreateFigurePosition("c8"), CreateFigurePosition("e6")); // b bishop
    rules.MakeMove(CreateFigurePosition("d2"), CreateFigurePosition("d4")); // w pawn long
    rules.MakeMove(CreateFigurePosition("e6"), CreateFigurePosition("d5")); // b bishop captures w pawn

    int analyzed;
    int bestEstimation;
    ai.BestMoveByAlphaBeta(FigureSide::White, 5, bestEstimation, analyzed);

    qDebug() << "   Analyzed:" << analyzed << ", best estimation:" << bestEstimation;

    return true;
}

bool Tests::ABAfter6PlyD6()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);

    rules.MakeMove(CreateFigurePosition("e2"), CreateFigurePosition("e4")); // w
    rules.MakeMove(CreateFigurePosition("d7"), CreateFigurePosition("d5")); // b
    rules.MakeMove(CreateFigurePosition("e4"), CreateFigurePosition("d5")); // w pawn captures b pawn
    rules.MakeMove(CreateFigurePosition("c8"), CreateFigurePosition("e6")); // b bishop
    rules.MakeMove(CreateFigurePosition("d2"), CreateFigurePosition("d4")); // w pawn long
    rules.MakeMove(CreateFigurePosition("e6"), CreateFigurePosition("d5")); // b bishop captures w pawn

    int analyzed;
    int bestEstimation;
    ai.BestMoveByAlphaBeta(FigureSide::White, 6, bestEstimation, analyzed);

    qDebug() << "   Analyzed:" << analyzed << ", best estimation:" << bestEstimation;

    return true;
}


bool Tests::AlphaBetaTest1()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);
    ai.ExtendSearchDepthOnCaptures = false;

    rules.MakeMove(CreateFigurePosition("e2"), CreateFigurePosition("e4"));
    rules.MakeMove(CreateFigurePosition("e7"), CreateFigurePosition("e6"));
    rules.MakeMove(CreateFigurePosition("d2"), CreateFigurePosition("d3"));
    // black's turn

    int alphaBetaAnalyzed;
    int alphaBetaBestEstimation;
    ai.BestMoveByAlphaBeta(board.GetTurningSide(), 1, alphaBetaBestEstimation, alphaBetaAnalyzed);

    int negamaxAnalyzed;
    int negamaxBestEstimation;
    ai.NegamaxSearch(board.GetTurningSide(), 1, negamaxBestEstimation, negamaxAnalyzed);

    if (negamaxBestEstimation != alphaBetaBestEstimation)
    {
        qDebug() << "   Alpha-Beta estimation:" << alphaBetaBestEstimation;
        qDebug() << "   Negamax estimation:" << negamaxBestEstimation;
        return false;
    }

    return negamaxBestEstimation == alphaBetaBestEstimation;
}

bool Tests::AlphaBetaTest2()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);
    ai.ExtendSearchDepthOnCaptures = false;

    rules.MakeMove(CreateFigurePosition("e2"), CreateFigurePosition("e4"));
    rules.MakeMove(CreateFigurePosition("e7"), CreateFigurePosition("e6"));
    rules.MakeMove(CreateFigurePosition("d2"), CreateFigurePosition("d3"));
    // black's turn

    int alphaBetaAnalyzed;
    int alphaBetaBestEstimation;
    ai.BestMoveByAlphaBeta(board.GetTurningSide(), 2, alphaBetaBestEstimation, alphaBetaAnalyzed);

    int negamaxAnalyzed;
    int negamaxBestEstimation;
    ai.NegamaxSearch(board.GetTurningSide(), 2, negamaxBestEstimation, negamaxAnalyzed);

    if (negamaxBestEstimation != alphaBetaBestEstimation)
    {
        qDebug() << "   Alpha-Beta estimation:" << alphaBetaBestEstimation;
        qDebug() << "   Negamax estimation:" << negamaxBestEstimation;
        return false;
    }

    return negamaxBestEstimation == alphaBetaBestEstimation;
}
bool Tests::AlphaBetaTest3()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);
    ai.ExtendSearchDepthOnCaptures = false;

    rules.MakeMove(CreateFigurePosition("e2"), CreateFigurePosition("e4"));
    rules.MakeMove(CreateFigurePosition("e7"), CreateFigurePosition("e6"));
    rules.MakeMove(CreateFigurePosition("d2"), CreateFigurePosition("d3"));
    // black's turn

    int alphaBetaAnalyzed;
    int alphaBetaBestEstimation;
    ai.BestMoveByAlphaBeta(board.GetTurningSide(), 3, alphaBetaBestEstimation, alphaBetaAnalyzed);

    int negamaxAnalyzed;
    int negamaxBestEstimation;
    ai.NegamaxSearch(board.GetTurningSide(), 3, negamaxBestEstimation, negamaxAnalyzed);

    if (negamaxBestEstimation != alphaBetaBestEstimation)
    {
        qDebug() << "   Alpha-Beta estimation:" << alphaBetaBestEstimation;
        qDebug() << "   Negamax estimation:" << negamaxBestEstimation;
        return false;
    }

    return negamaxBestEstimation == alphaBetaBestEstimation;
}

bool Tests::AlphaBetaTest4()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);
    ai.ExtendSearchDepthOnCaptures = false;

    rules.MakeMove(CreateFigurePosition("e2"), CreateFigurePosition("e4"));
    rules.MakeMove(CreateFigurePosition("e7"), CreateFigurePosition("e6"));
    rules.MakeMove(CreateFigurePosition("d2"), CreateFigurePosition("d3"));
    // black's turn

    int alphaBetaAnalyzed;
    int alphaBetaBestEstimation;
    ai.BestMoveByAlphaBeta(board.GetTurningSide(), 4, alphaBetaBestEstimation, alphaBetaAnalyzed);

    int negamaxAnalyzed;
    int negamaxBestEstimation;
    ai.NegamaxSearch(board.GetTurningSide(), 4, negamaxBestEstimation, negamaxAnalyzed);

    if (negamaxBestEstimation != alphaBetaBestEstimation)
    {
        qDebug() << "   Alpha-Beta estimation:" << alphaBetaBestEstimation;
        qDebug() << "   Negamax estimation:" << negamaxBestEstimation;
        return false;
    }

    return negamaxBestEstimation == alphaBetaBestEstimation;
}

bool Tests::AlphaBetaTest_2_1()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);
    ai.ExtendSearchDepthOnCaptures = false;

    rules.MakeMove(CreateFigurePosition("e2"), CreateFigurePosition("e4"));
    rules.MakeMove(CreateFigurePosition("e7"), CreateFigurePosition("e5"));
    rules.MakeMove(CreateFigurePosition("d2"), CreateFigurePosition("d3"));
    rules.MakeMove(CreateFigurePosition("g8"), CreateFigurePosition("f6"));
    rules.MakeMove(CreateFigurePosition("d1"), CreateFigurePosition("f3"));
    rules.MakeMove(CreateFigurePosition("b8"), CreateFigurePosition("c6"));
    // white's turn

    int alphaBetaAnalyzed;
    int alphaBetaBestEstimation;
    ai.BestMoveByAlphaBeta(board.GetTurningSide(), 1, alphaBetaBestEstimation, alphaBetaAnalyzed);

    int negamaxAnalyzed;
    int negamaxBestEstimation;
    ai.NegamaxSearch(board.GetTurningSide(), 1, negamaxBestEstimation, negamaxAnalyzed);

    if (negamaxBestEstimation != alphaBetaBestEstimation)
    {
        qDebug() << "   Alpha-Beta estimation:" << alphaBetaBestEstimation;
        qDebug() << "   Negamax estimation:" << negamaxBestEstimation;
        return false;
    }

    return negamaxBestEstimation == alphaBetaBestEstimation;
}

bool Tests::AlphaBetaTest_2_2()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);
    ai.ExtendSearchDepthOnCaptures = false;

    rules.MakeMove(CreateFigurePosition("e2"), CreateFigurePosition("e4"));
    rules.MakeMove(CreateFigurePosition("e7"), CreateFigurePosition("e5"));
    rules.MakeMove(CreateFigurePosition("d2"), CreateFigurePosition("d3"));
    rules.MakeMove(CreateFigurePosition("g8"), CreateFigurePosition("f6"));
    rules.MakeMove(CreateFigurePosition("d1"), CreateFigurePosition("f3"));
    rules.MakeMove(CreateFigurePosition("b8"), CreateFigurePosition("c6"));
    // white's turn

    int alphaBetaAnalyzed;
    int alphaBetaBestEstimation;
    ai.BestMoveByAlphaBeta(board.GetTurningSide(), 2, alphaBetaBestEstimation, alphaBetaAnalyzed);

    int negamaxAnalyzed;
    int negamaxBestEstimation;
    ai.NegamaxSearch(board.GetTurningSide(), 2, negamaxBestEstimation, negamaxAnalyzed);

    if (negamaxBestEstimation != alphaBetaBestEstimation)
    {
        qDebug() << "   Alpha-Beta estimation:" << alphaBetaBestEstimation;
        qDebug() << "   Negamax estimation:" << negamaxBestEstimation;
        return false;
    }

    return negamaxBestEstimation == alphaBetaBestEstimation;
}

bool Tests::AlphaBetaTest_2_3()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);
    ai.ExtendSearchDepthOnCaptures = false;

    rules.MakeMove(CreateFigurePosition("e2"), CreateFigurePosition("e4"));
    rules.MakeMove(CreateFigurePosition("e7"), CreateFigurePosition("e5"));
    rules.MakeMove(CreateFigurePosition("d2"), CreateFigurePosition("d3"));
    rules.MakeMove(CreateFigurePosition("g8"), CreateFigurePosition("f6"));
    rules.MakeMove(CreateFigurePosition("d1"), CreateFigurePosition("f3"));
    rules.MakeMove(CreateFigurePosition("b8"), CreateFigurePosition("c6"));
    // white's turn

    int alphaBetaAnalyzed;
    int alphaBetaBestEstimation;
    ai.BestMoveByAlphaBeta(board.GetTurningSide(), 3, alphaBetaBestEstimation, alphaBetaAnalyzed);

    int negamaxAnalyzed;
    int negamaxBestEstimation;
    ai.NegamaxSearch(board.GetTurningSide(), 3, negamaxBestEstimation, negamaxAnalyzed);

    if (negamaxBestEstimation != alphaBetaBestEstimation)
    {
        qDebug() << "   Alpha-Beta estimation:" << alphaBetaBestEstimation;
        qDebug() << "   Negamax estimation:" << negamaxBestEstimation;
        return false;
    }

    return negamaxBestEstimation == alphaBetaBestEstimation;
}

bool Tests::AlphaBetaTest_2_4()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);
    ai.ExtendSearchDepthOnCaptures = false;

    rules.MakeMove(CreateFigurePosition("e2"), CreateFigurePosition("e4"));
    rules.MakeMove(CreateFigurePosition("e7"), CreateFigurePosition("e5"));
    rules.MakeMove(CreateFigurePosition("d2"), CreateFigurePosition("d3"));
    rules.MakeMove(CreateFigurePosition("g8"), CreateFigurePosition("f6"));
    rules.MakeMove(CreateFigurePosition("d1"), CreateFigurePosition("f3"));
    rules.MakeMove(CreateFigurePosition("b8"), CreateFigurePosition("c6"));
    // white's turn

    int alphaBetaAnalyzed;
    int alphaBetaBestEstimation;
    ai.BestMoveByAlphaBeta(board.GetTurningSide(), 4, alphaBetaBestEstimation, alphaBetaAnalyzed);

    int negamaxAnalyzed;
    int negamaxBestEstimation;
    ai.NegamaxSearch(board.GetTurningSide(), 4, negamaxBestEstimation, negamaxAnalyzed);

    if (negamaxBestEstimation != alphaBetaBestEstimation)
    {
        qDebug() << "   Alpha-Beta estimation:" << alphaBetaBestEstimation;
        qDebug() << "   Negamax estimation:" << negamaxBestEstimation;
        return false;
    }

    return negamaxBestEstimation == alphaBetaBestEstimation;
}

bool Tests::PositionCountingTest1()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);

    rules.MakeMove(CreateFigurePosition("g1"), CreateFigurePosition("f3"));
    rules.MakeMove(CreateFigurePosition("g8"), CreateFigurePosition("f6"));
    rules.MakeMove(CreateFigurePosition("f3"), CreateFigurePosition("g1"));
    rules.MakeMove(CreateFigurePosition("f6"), CreateFigurePosition("g8"));
    rules.MakeMove(CreateFigurePosition("g1"), CreateFigurePosition("f3"));

    return board.GetCurrentPositionCount() == 2;
}

bool Tests::PositionCountingTest2()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);

    rules.MakeMove(CreateFigurePosition("g1"), CreateFigurePosition("f3"));
    rules.MakeMove(CreateFigurePosition("g8"), CreateFigurePosition("f6"));
    rules.MakeMove(CreateFigurePosition("f3"), CreateFigurePosition("g1"));
    rules.MakeMove(CreateFigurePosition("f6"), CreateFigurePosition("g8"));
    rules.MakeMove(CreateFigurePosition("g1"), CreateFigurePosition("f3"));
    rules.MakeMove(CreateFigurePosition("g8"), CreateFigurePosition("f6"));
    rules.MakeMove(CreateFigurePosition("f3"), CreateFigurePosition("g1"));
    rules.MakeMove(CreateFigurePosition("f6"), CreateFigurePosition("g8"));
    rules.MakeMove(CreateFigurePosition("g1"), CreateFigurePosition("f3"));

    return board.GetCurrentPositionCount() == 3;
}

bool Tests::BoardSerializationTest()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);

    rules.MakeMove(CreateFigurePosition("e2"), CreateFigurePosition("e4"));
    rules.MakeMove(CreateFigurePosition("d7"), CreateFigurePosition("d5"));
    rules.MakeMove(CreateFigurePosition("e4"), CreateFigurePosition("d5"));
    rules.MakeMove(CreateFigurePosition("d8"), CreateFigurePosition("d5"));

    QString boardSerializedString = BoardSerializer::Save(board);

    Board deserializedBoard = BoardSerializer::Load(boardSerializedString);

    foreach(Figure* figure, deserializedBoard.GetAllAliveFigures())
    {
        if (!board.HasFigureAt(figure->Position, figure->Side))
            return false;
    }

    return board.GetAllAliveFigures().count() == deserializedBoard.GetAllAliveFigures().count();
}

bool Tests::WM1()
{

    return true;
}
