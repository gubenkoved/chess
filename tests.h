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

    bool ToFENTest1();
    bool ToFENTest2();
    bool ToFENTest3();
    bool ToFENTest4();

//    bool FromFENTest1();
//    bool FromFENTest2();
//    bool FromFENTest3();
//    bool FromFENTest4();

    bool PerftTest1();
    bool PerftTest2();
    bool PerftTest3();
    bool PerftTest4();

    bool ABFromStartD1();
    bool ABFromStartD2();
    bool ABFromStartD3();
    bool ABFromStartD4();
    bool ABFromStartD5();
    bool ABFromStartD6();
    bool ABFromStartD7();

    bool ABAfter6PlyD4();
    bool ABAfter6PlyD5();
    bool ABAfter6PlyD6();

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

    bool BoardSerializationTest();

    // countertest for detected wrong moves
    bool WM1();

    bool ParsePuzzlesTest();
    bool ReconstructBoardFromPuzzlesTest();
};

#endif // TESTS_H
