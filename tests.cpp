#define TEST_EXPAND(x) if (TestIt(#x, &Tests:: x)) { ++ passed; } else {++ failed; }

#include <string>
#include <stdexcept>
#include <QMap>
#include <QTime>

#include "tests.h"

#include "exception.h"
#include "board.h"
#include "figure.h"
#include "rules.h"
#include "ai.h"
#include "boardserializer.h"
#include "fen.h"
#include "bitboardhelper.h"
#include "puzzle.h"

Tests::Tests()
{
}

void Tests::TestAll()
{
    qDebug() << "Tests::TestAll...";

    int passed = 0, failed = 0;

//    TEST_EXPAND(TestFigurePosition1)
//    TEST_EXPAND(TestFigurePosition2)
//    TEST_EXPAND(TestFigurePosition3)
//    TEST_EXPAND(TestFigurePosition4)
//    TEST_EXPAND(TestFigurePosition5)

//    TEST_EXPAND(BoardStartPositionTest)
//    TEST_EXPAND(BoardGetSideFiguresTest)

//    TEST_EXPAND(GetGuardedPositionsTest1)
//    TEST_EXPAND(GetGuardedPositionsTest2)
//    TEST_EXPAND(GetGuardedPositionsTest3)
//    TEST_EXPAND(GetGuardedPositionsTest4)
//    TEST_EXPAND(GetGuardedPositionsTest5)
//    TEST_EXPAND(GetGuardedPositionsTest6)

//    TEST_EXPAND(MoveFigureTest)

//    TEST_EXPAND(MakeMoveTest1)
//    TEST_EXPAND(MakeMoveTest2)

//    TEST_EXPAND(MoveFindingTest1)
//    TEST_EXPAND(MoveFindingTest2)
//    TEST_EXPAND(MoveFindingTest3)

//    TEST_EXPAND(EnPassonTest1)
//    TEST_EXPAND(EnPassonTest2)

//    TEST_EXPAND(BoardDeepCopyTest);

//    TEST_EXPAND(ToFENTest1);
//    TEST_EXPAND(ToFENTest2);
//    TEST_EXPAND(ToFENTest3);
//    TEST_EXPAND(ToFENTest4);

////    TEST_EXPAND(FromFENTest1);
////    TEST_EXPAND(FromFENTest2);
////    TEST_EXPAND(FromFENTest3);
////    TEST_EXPAND(FromFENTest4);

//    TEST_EXPAND(PerftTest1)
//    TEST_EXPAND(PerftTest2)
//    TEST_EXPAND(PerftTest3)
//    //TEST_EXPAND(PerftTest4)

//    TEST_EXPAND(ABFromStartD1)
//    TEST_EXPAND(ABFromStartD2)
//    TEST_EXPAND(ABFromStartD3)
//    TEST_EXPAND(ABFromStartD4)
//    //TEST_EXPAND(ABFromStartD5)
//    //TEST_EXPAND(ABFromStartD6)


    TEST_EXPAND(ABAfter6PlyD4)
//    TEST_EXPAND(ABAfter6PlyD5)
//    //TEST_EXPAND(ABAfter6PlyD6)

//    TEST_EXPAND(AlphaBetaTest1)
//    TEST_EXPAND(AlphaBetaTest2)
//    TEST_EXPAND(AlphaBetaTest3)
//    //TEST_EXPAND(AlphaBetaTest4)

//    TEST_EXPAND(AlphaBetaTest_2_1)
//    TEST_EXPAND(AlphaBetaTest_2_2)
//    TEST_EXPAND(AlphaBetaTest_2_3)
//    //TEST_EXPAND(AlphaBetaTest_2_4)

//    TEST_EXPAND(PositionCountingTest1)
//    TEST_EXPAND(PositionCountingTest2)

//    TEST_EXPAND(BoardSerializationTest)

//    TEST_EXPAND(WM1)

//    TEST_EXPAND(ParsePuzzlesTest)
//    TEST_EXPAND(ReconstructBoardFromPuzzlesTest)
//    TEST_EXPAND(PuzzlesBoardFromFenThenToFenTest)
//    TEST_EXPAND(SolveMateInTwoPuzzles)

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
    POSITION p = PositionHelper::FromString("a1");

    return PositionHelper::X(p) == 1 && PositionHelper::Y(p) == 1;
}
bool Tests::TestFigurePosition2()
{
    POSITION p = PositionHelper::FromString("h8");

    return PositionHelper::X(p) == 8 && PositionHelper::Y(p) == 8;
}
bool Tests::TestFigurePosition3()
{
    POSITION p = PositionHelper::FromString("e2");

    return PositionHelper::X(p) == 5 && PositionHelper::Y(p) == 2;
}
bool Tests::TestFigurePosition4()
{
    POSITION p = PositionHelper::FromString("a0");

    return PositionHelper::IsInvalid(p);
}
bool Tests::TestFigurePosition5()
{
    POSITION p = PositionHelper::FromString("h9");

     return PositionHelper::IsInvalid(p);
}

bool Tests::BoardStartPositionTest()
{
    Board board = Board::StartPosition();

    QMap<FigureType, int> counter;

    for (int x = 1; x <= 8; ++x)
    {
        for(int y = 1; y <= 8; ++y)
        {
            POSITION pos = PositionHelper::Create(x, y);
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

    PositionCollection guarded = BitboardHelper::GetPositions(rules.GetGuardedPositions2(FigureSide::White));

    return guarded.count() == 22;
}

bool Tests::GetGuardedPositionsTest2()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);

    PositionCollection guarded = BitboardHelper::GetPositions(rules.GetGuardedPositions2(FigureSide::Black));

    return guarded.count() == 22;
}

bool Tests::GetGuardedPositionsTest3()
{
    Board board = Board();
    Rules rules = Rules(&board);

    board.AddAliveFigure(new Figure(FigureSide::White, FigureType::Knight, PositionHelper::FromString("e5")));

    PositionCollection guarded = BitboardHelper::GetPositions(rules.GetGuardedPositions2(FigureSide::White));

    return guarded.count() == 8;
}
bool Tests::GetGuardedPositionsTest4()
{
    Board board = Board();
    Rules rules = Rules(&board);

    board.AddAliveFigure(new Figure(FigureSide::White, FigureType::Rock, PositionHelper::FromString("e4")));

    PositionCollection guarded = BitboardHelper::GetPositions(rules.GetGuardedPositions2(FigureSide::White));

    return guarded.count() == 14;
}
bool Tests::GetGuardedPositionsTest5()
{
    Board board = Board();
    Rules rules = Rules(&board);

    board.AddAliveFigure(new Figure(FigureSide::White, FigureType::Queen, PositionHelper::FromString("b2")));

    PositionCollection guarded = BitboardHelper::GetPositions(rules.GetGuardedPositions2(FigureSide::White));

    return guarded.count() == 23;
}
bool Tests::GetGuardedPositionsTest6()
{
    Board board = Board();
    Rules rules = Rules(&board);

    board.AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, PositionHelper::FromString("h7")));

    PositionCollection guarded = BitboardHelper::GetPositions(rules.GetGuardedPositions2(FigureSide::White));

    return guarded.count() == 1;
}

bool Tests::MoveFigureTest()
{
    Board board = Board();

    board.SetupStartPosition();

    Figure* pawn = board.FigureAt(PositionHelper::FromString("e2"));
    board.MoveFigure(pawn, PositionHelper::FromString("e4"));

    return board.FigureAt(PositionHelper::FromString("e4")) == pawn;
}

bool Tests::MakeMoveTest1()
{
    Board board = Board();
    Rules rules = Rules(&board);

    board.SetupStartPosition();

    Figure* pawn = board.FigureAt(PositionHelper::FromString("e2"));

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4"));

    return board.FigureAt(PositionHelper::FromString("e4")) == pawn;
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

    MoveCollection moves = rules.GetPossibleMoves(FigureSide::White);

    return moves.count() == 20;
}

bool Tests::MoveFindingTest2()
{    
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);

    MoveCollection moves = rules.GetPossibleMoves(board.FigureAt(PositionHelper::FromString("e2")));

    return moves.count() == 2
            && moves[0].To == PositionHelper::FromString("e3")
            && moves[1].To == PositionHelper::FromString("e4");
}

bool Tests::MoveFindingTest3()
{
    Board board;
    Rules rules = Rules(&board);

    board.AddAliveFigure(new Figure(FigureSide::White, FigureType::King, PositionHelper::FromString("d4")));
    board.AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, PositionHelper::FromString("e4")));
    board.AddAliveFigure(new Figure(FigureSide::Black, FigureType::King, PositionHelper::FromString("f4")));

    return rules.GetPossibleMoves(FigureSide::White).count() == 4;
}

bool Tests::EnPassonTest1()
{
    Board board = Board();
    Rules rules = Rules(&board);

    board.SetupKings();
    board.TurnTransition(); // to black

    board.AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, PositionHelper::FromString("e5")));
    board.AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, PositionHelper::FromString("d7")));

    rules.MakeMove(PositionHelper::FromString("d7"), PositionHelper::FromString("d5"));

    // check for enPasson availability
    return rules.GetPossibleDestinations(board.FigureAt(PositionHelper::FromString("e5"))).contains(PositionHelper::FromString("d6"));
}

bool Tests::EnPassonTest2()
{
    Board board = Board();
    Rules rules = Rules(&board);

    board.SetupKings();

    board.AddAliveFigure(new Figure(FigureSide::White, FigureType::Pawn, PositionHelper::FromString("e2")));
    board.AddAliveFigure(new Figure(FigureSide::Black, FigureType::Pawn, PositionHelper::FromString("d4")));

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4"));

    // check for enPasson availability
    return rules.GetPossibleDestinations(board.FigureAt(PositionHelper::FromString("d4"))).contains(PositionHelper::FromString("e3"));
}

bool Tests::BoardDeepCopyTest()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4")); // w
    rules.MakeMove(PositionHelper::FromString("d7"), PositionHelper::FromString("d5")); // b
    rules.MakeMove(PositionHelper::FromString("e4"), PositionHelper::FromString("d5")); // w pawn captures b pawn
    rules.MakeMove(PositionHelper::FromString("c8"), PositionHelper::FromString("e6")); // b bishop
    rules.MakeMove(PositionHelper::FromString("d2"), PositionHelper::FromString("d4")); // w pawn long
    rules.MakeMove(PositionHelper::FromString("e6"), PositionHelper::FromString("d5")); // b bishop captures w pawn
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

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4"));

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

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4"));
    rules.MakeMove(PositionHelper::FromString("c7"), PositionHelper::FromString("c5"));

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

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4"));
    rules.MakeMove(PositionHelper::FromString("c7"), PositionHelper::FromString("c5"));
    rules.MakeMove(PositionHelper::FromString("g1"), PositionHelper::FromString("f3"));

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

bool Tests::ABAfter6PlyD4()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4")); // w
    rules.MakeMove(PositionHelper::FromString("d7"), PositionHelper::FromString("d5")); // b
    rules.MakeMove(PositionHelper::FromString("e4"), PositionHelper::FromString("d5")); // w pawn captures b pawn
    rules.MakeMove(PositionHelper::FromString("c8"), PositionHelper::FromString("e6")); // b bishop
    rules.MakeMove(PositionHelper::FromString("d2"), PositionHelper::FromString("d4")); // w pawn long
    rules.MakeMove(PositionHelper::FromString("e6"), PositionHelper::FromString("d5")); // b bishop captures w pawn

    int analyzed;
    int bestEstimation;
    ai.BestMoveByAlphaBeta(FigureSide::White, 4, bestEstimation, analyzed);

    qDebug() << "   Analyzed:" << analyzed << ", best estimation:" << bestEstimation;

    return true;
}

bool Tests::ABAfter6PlyD5()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);
    AI ai = AI(&board, &rules);

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4")); // w
    rules.MakeMove(PositionHelper::FromString("d7"), PositionHelper::FromString("d5")); // b
    rules.MakeMove(PositionHelper::FromString("e4"), PositionHelper::FromString("d5")); // w pawn captures b pawn
    rules.MakeMove(PositionHelper::FromString("c8"), PositionHelper::FromString("e6")); // b bishop
    rules.MakeMove(PositionHelper::FromString("d2"), PositionHelper::FromString("d4")); // w pawn long
    rules.MakeMove(PositionHelper::FromString("e6"), PositionHelper::FromString("d5")); // b bishop captures w pawn

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

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4")); // w
    rules.MakeMove(PositionHelper::FromString("d7"), PositionHelper::FromString("d5")); // b
    rules.MakeMove(PositionHelper::FromString("e4"), PositionHelper::FromString("d5")); // w pawn captures b pawn
    rules.MakeMove(PositionHelper::FromString("c8"), PositionHelper::FromString("e6")); // b bishop
    rules.MakeMove(PositionHelper::FromString("d2"), PositionHelper::FromString("d4")); // w pawn long
    rules.MakeMove(PositionHelper::FromString("e6"), PositionHelper::FromString("d5")); // b bishop captures w pawn

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

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4"));
    rules.MakeMove(PositionHelper::FromString("e7"), PositionHelper::FromString("e6"));
    rules.MakeMove(PositionHelper::FromString("d2"), PositionHelper::FromString("d3"));
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

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4"));
    rules.MakeMove(PositionHelper::FromString("e7"), PositionHelper::FromString("e6"));
    rules.MakeMove(PositionHelper::FromString("d2"), PositionHelper::FromString("d3"));
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

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4"));
    rules.MakeMove(PositionHelper::FromString("e7"), PositionHelper::FromString("e6"));
    rules.MakeMove(PositionHelper::FromString("d2"), PositionHelper::FromString("d3"));
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

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4"));
    rules.MakeMove(PositionHelper::FromString("e7"), PositionHelper::FromString("e6"));
    rules.MakeMove(PositionHelper::FromString("d2"), PositionHelper::FromString("d3"));
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

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4"));
    rules.MakeMove(PositionHelper::FromString("e7"), PositionHelper::FromString("e5"));
    rules.MakeMove(PositionHelper::FromString("d2"), PositionHelper::FromString("d3"));
    rules.MakeMove(PositionHelper::FromString("g8"), PositionHelper::FromString("f6"));
    rules.MakeMove(PositionHelper::FromString("d1"), PositionHelper::FromString("f3"));
    rules.MakeMove(PositionHelper::FromString("b8"), PositionHelper::FromString("c6"));
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

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4"));
    rules.MakeMove(PositionHelper::FromString("e7"), PositionHelper::FromString("e5"));
    rules.MakeMove(PositionHelper::FromString("d2"), PositionHelper::FromString("d3"));
    rules.MakeMove(PositionHelper::FromString("g8"), PositionHelper::FromString("f6"));
    rules.MakeMove(PositionHelper::FromString("d1"), PositionHelper::FromString("f3"));
    rules.MakeMove(PositionHelper::FromString("b8"), PositionHelper::FromString("c6"));
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

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4"));
    rules.MakeMove(PositionHelper::FromString("e7"), PositionHelper::FromString("e5"));
    rules.MakeMove(PositionHelper::FromString("d2"), PositionHelper::FromString("d3"));
    rules.MakeMove(PositionHelper::FromString("g8"), PositionHelper::FromString("f6"));
    rules.MakeMove(PositionHelper::FromString("d1"), PositionHelper::FromString("f3"));
    rules.MakeMove(PositionHelper::FromString("b8"), PositionHelper::FromString("c6"));
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

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4"));
    rules.MakeMove(PositionHelper::FromString("e7"), PositionHelper::FromString("e5"));
    rules.MakeMove(PositionHelper::FromString("d2"), PositionHelper::FromString("d3"));
    rules.MakeMove(PositionHelper::FromString("g8"), PositionHelper::FromString("f6"));
    rules.MakeMove(PositionHelper::FromString("d1"), PositionHelper::FromString("f3"));
    rules.MakeMove(PositionHelper::FromString("b8"), PositionHelper::FromString("c6"));
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

    rules.MakeMove(PositionHelper::FromString("g1"), PositionHelper::FromString("f3"));
    rules.MakeMove(PositionHelper::FromString("g8"), PositionHelper::FromString("f6"));
    rules.MakeMove(PositionHelper::FromString("f3"), PositionHelper::FromString("g1"));
    rules.MakeMove(PositionHelper::FromString("f6"), PositionHelper::FromString("g8"));
    rules.MakeMove(PositionHelper::FromString("g1"), PositionHelper::FromString("f3"));

    int expectedCurrentPositionCount = 2;

    if (board.GetCurrentPositionCount() != expectedCurrentPositionCount)
    {
        qDebug() << "Expected current position count:" << expectedCurrentPositionCount;
        qDebug() << "Actual current position count:" << board.GetCurrentPositionCount();
    }

    return board.GetCurrentPositionCount() == expectedCurrentPositionCount;
}

bool Tests::PositionCountingTest2()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);

    rules.MakeMove(PositionHelper::FromString("g1"), PositionHelper::FromString("f3"));
    rules.MakeMove(PositionHelper::FromString("g8"), PositionHelper::FromString("f6"));
    rules.MakeMove(PositionHelper::FromString("f3"), PositionHelper::FromString("g1"));
    rules.MakeMove(PositionHelper::FromString("f6"), PositionHelper::FromString("g8"));
    rules.MakeMove(PositionHelper::FromString("g1"), PositionHelper::FromString("f3"));
    rules.MakeMove(PositionHelper::FromString("g8"), PositionHelper::FromString("f6"));
    rules.MakeMove(PositionHelper::FromString("f3"), PositionHelper::FromString("g1"));
    rules.MakeMove(PositionHelper::FromString("f6"), PositionHelper::FromString("g8"));
    rules.MakeMove(PositionHelper::FromString("g1"), PositionHelper::FromString("f3"));

    int expectedCurrentPositionCount = 3;

    if (board.GetCurrentPositionCount() != expectedCurrentPositionCount)
    {
        qDebug() << "Expected current position count:" << expectedCurrentPositionCount;
        qDebug() << "Actual current position count:" << board.GetCurrentPositionCount();
    }

    return board.GetCurrentPositionCount() == expectedCurrentPositionCount;
}

bool Tests::BoardSerializationTest()
{
    Board board = Board::StartPosition();
    Rules rules = Rules(&board);

    rules.MakeMove(PositionHelper::FromString("e2"), PositionHelper::FromString("e4"));
    rules.MakeMove(PositionHelper::FromString("d7"), PositionHelper::FromString("d5"));
    rules.MakeMove(PositionHelper::FromString("e4"), PositionHelper::FromString("d5"));
    rules.MakeMove(PositionHelper::FromString("d8"), PositionHelper::FromString("d5"));

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

bool Tests::ParsePuzzlesTest()
{
    QList<Puzzle> puzzles = Puzzle::Parse("C:/Users/nrj/Documents/QT Creator Workspace/ChessApp/trunk/mate-in-two.csv");

//    foreach (const Puzzle& p, puzzles) {
//        qDebug() << p;
//    }

    return puzzles.count() == 922;
}

bool Tests::ReconstructBoardFromPuzzlesTest()
{
    QList<Puzzle> puzzles = Puzzle::Parse("C:/Users/nrj/Documents/QT Creator Workspace/ChessApp/trunk/mate-in-two.csv");

    foreach (const Puzzle& p, puzzles)
    {
        FEN::PositionFromFEN(p.PositionFEN);
    }

    // if we got here no errors occured while reconstuction
    return true;
}

bool Tests::PuzzlesBoardFromFenThenToFenTest()
{
    QList<Puzzle> puzzles = Puzzle::Parse("C:/Users/nrj/Documents/QT Creator Workspace/ChessApp/trunk/mate-in-two.csv");
    foreach (const Puzzle& p, puzzles)
    {
        Board puzzleBoard = FEN::PositionFromFEN(p.PositionFEN);
        // puzzle contains only position, then short it
        QString shortedFen = FEN::Evaluate(puzzleBoard).mid(0, p.PositionFEN.length());

        if (shortedFen != p.PositionFEN)
        {
            return false;
        }
    }

    return true;
}

bool Tests::SolveMateInTwoPuzzles()
{
    int solved = 0;

    QList<Puzzle> puzzles = Puzzle::Parse("C:/Users/nrj/Documents/QT Creator Workspace/ChessApp/trunk/mate-in-two.csv");
    foreach (const Puzzle& p, puzzles)
    {
        // try to find mate in two
        Board puzzleBoard = FEN::PositionFromFEN(p.PositionFEN);
        Rules rules(&puzzleBoard);
        // setup AI to fastest solve mate in two puzzles
        AI ai(&puzzleBoard, &rules);
        ai.ExtendSearchDepthOnCaptures = false;

        int bestEstimation;
        int analyzed;

        // mate in two means three ply only
        ai.BestMoveByAlphaBeta(FigureSide::White, 4, bestEstimation, analyzed);

        // if mate finded
        if (bestEstimation >= AI::WIN_ESTIMATION)
        {
            solved += 1;
            qDebug() << "     SOLVED #" << p.Id << p.PositionFEN << ". Estimation:" << bestEstimation;
        } else
        {
            qDebug() << "     NOT SOLVED #" << p.Id << p.PositionFEN << ". Estimation:" << bestEstimation;
        }
    }

    qDebug() << "   --";
    qDebug() << "   SOLVED " << solved;
    qDebug() << "   TOTAL " << puzzles.count();

    return solved == puzzles.count();
}
