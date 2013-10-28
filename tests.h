#ifndef TESTS_H
#define TESTS_H

#include <QString>

class Tests
{
private:
    bool TestIt(QString str, bool (Tests::*method)(void));
public:
    Tests();
    void TestAll();

    bool TestFigurePosition1();
    bool TestFigurePosition2();
    bool TestFigurePosition3();
    bool TestFigurePosition4();
    bool TestFigurePosition5();

    bool BoardStartPositionTest();    
    bool BoardGetSideFiguresTest();
    bool GetGuardedPositionsTest1();
    bool GetGuardedPositionsTest2();
    bool GetGuardedPositionsTest3();
    bool GetGuardedPositionsTest4();
    bool GetGuardedPositionsTest5();
    bool GetGuardedPositionsTest6();
    bool MoveFigureTest();
    bool MakeMoveTest1();
    bool MakeMoveTest2();
    bool MoveFindingTest1();
    bool MoveFindingTest2();    
    bool MoveFindingTest3();
    bool EnPassonTest1();
    bool EnPassonTest2();

    bool BoardDeepCopyTest();

    bool FENTest1();
    bool FENTest2();
    bool FENTest3();
    bool FENTest4();

    bool PerftTest1();
    bool PerftTest2();
    bool PerftTest3();
    bool PerftTest4();

    bool AlphaBetaTime1();
    bool AlphaBetaTime2();
    bool AlphaBetaTime3();
    bool AlphaBetaTime4();
    bool AlphaBetaTime5();
    bool AlphaBetaTime5_2();
    bool AlphaBetaTime6();
    bool AlphaBetaTime7();

    bool AlphaBetaTest1();
    bool AlphaBetaTest2();
    bool AlphaBetaTest3();
    bool AlphaBetaTest4();

    bool AlphaBetaTest_2_1();
    bool AlphaBetaTest_2_2();
    bool AlphaBetaTest_2_3();
    bool AlphaBetaTest_2_4();

    bool PositionCountingTest1();
    bool PositionCountingTest2();

    bool BoardSerializationTest1();
};

#endif // TESTS_H
