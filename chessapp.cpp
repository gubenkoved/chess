#include <QMessageBox>

#include "chessapp.h"

#include "fen.h"
#include "figure.h"
#include "figureposition.h"
#include "boardserializer.h"
#include "exception.h"

//#include <QtConcurrentRun>


ChessApp::ChessApp(QObject *parent)
    : QObject(parent)
{
    //m_board = new Board();
    //m_board->SetupStartPosition();
    m_board = new Board(FEN::PositionFromFEN("4B3/5P1k/4KP1b/6R1/8/8/8/8"));

    m_rules = new Rules(m_board);
    m_asyncAI = new AsyncAI(m_board);

    m_aiThinking = false;

    connect(m_asyncAI, SIGNAL(BestMoveFinded(Move)), this, SLOT(HandleFindedBestMove(Move)), Qt::DirectConnection);
}

ChessApp::~ChessApp()
{
    delete m_board;
    delete m_rules;
    delete m_asyncAI;
}

void ChessApp::SetBoardControl(QObject *board)
{
    m_boardControl = board;
}

void ChessApp::CheckForGameOver()
{
    FigureSide turningSide = m_board->GetTurningSide();

    if (m_rules->GetPossibleMoves(turningSide).count() == 0)
    {
        if (m_rules->IsUnderCheck(turningSide))
        {
            if (turningSide == FigureSide::White)
            {
                QMetaObject::invokeMethod(m_boardControl, "showNotification", Q_ARG(QVariant, "Black wins"));
                qDebug() << "Black wins";
            } else
            {
                QMetaObject::invokeMethod(m_boardControl, "showNotification", Q_ARG(QVariant, "White wins"));
                qDebug() << "White wins";
            }
        } else
        {
            QMetaObject::invokeMethod(m_boardControl, "showNotification", Q_ARG(QVariant, "Draw game"));
            qDebug() << "Draw game";
        }
    }

}

void ChessApp::StartStepByComputer()
{
    if (!m_aiThinking)
    {
        if (m_rules->GetPossibleMoves(m_board->GetTurningSide()).count() != 0)
        {
            qDebug() << "Search started...";

            m_asyncAI->StartBestMoveSearch(m_board->GetTurningSide(), DEPTH);
            m_aiThinking = true;
            m_boardChangedSinceAiStart = false;
        } else
        {
            qDebug() << "There are no moves available to turning side.";
        }
    } else
    {
        qDebug() << "Computer is already thinking now...";
    }
}

void ChessApp::HandleFindedBestMove(Move bestMove)
{
    qDebug() << "AI best move:" << bestMove;

    if (!m_boardChangedSinceAiStart)
    {
        m_rules->MakeMove(bestMove.From, bestMove.To);

        CheckForGameOver();
    } else
    {
        qDebug() << "Board was changed since AI start.";
    }

    m_aiThinking = false;

    Draw();
}

void ChessApp::Draw()
{
    //qDebug() << "ChessApp::Draw";

    QMetaObject::invokeMethod(m_boardControl, "updateField");

    for(int x = 1; x <= 8; ++x)
    {
        for (int y = 1; y <= 8; ++y)
        {            
            POSITION p = PositionHelper::Create(x, y);

            Figure* f = m_board->FigureAt(p);
            QString text = f != NULL ? QString(f->GetUnicodeChessChar()) : "";

            QMetaObject::invokeMethod(m_boardControl, "setCellText", Q_ARG(QVariant, x), Q_ARG(QVariant, y), Q_ARG(QVariant, text));
        }
    }

    // highlight last move
    if (!m_board->IsHistoryEmpty())
    {
        Move lastMove = m_board->GetLastMove();
        POSITION from = lastMove.From;
        POSITION to = lastMove.To;

        QMetaObject::invokeMethod(m_boardControl, "setCellColor", Q_ARG(QVariant, PositionHelper::X(from)), Q_ARG(QVariant, PositionHelper::Y(from)), Q_ARG(QVariant, "Orange"));
        QMetaObject::invokeMethod(m_boardControl, "setCellColor", Q_ARG(QVariant, PositionHelper::X(to)), Q_ARG(QVariant, PositionHelper::Y(to)), Q_ARG(QVariant, "Orange"));
    }
}

bool ChessApp::IsFigureCell(int x, int y)
{
    return m_board->FigureAt(PositionHelper::Create(x,y)) != NULL;
}

bool ChessApp::IsPossibleStep(int fromX, int fromY, int toX, int toY)
{
    //return true;
    //qDebug() << "ChessApp::IsPossibleStep";

    Figure* f = m_board->FigureAt(PositionHelper::Create(fromX, fromY));

    if (f != NULL)
    {
        PositionList positions = m_rules->GetPossibleDestinations(f);

        //qDebug() << positions << positions.contains(LightFigurePosition(toX, toY));

        return positions.contains(PositionHelper::Create(toX, toY));
    } else
    {
        return false;
    }
}

void ChessApp::Step(int fromX, int fromY, int toX, int toY)
{
    //qDebug() << "ChessApp::Step";

    if (m_aiThinking)
    {
        m_boardChangedSinceAiStart = true;
    }

    m_rules->MakeMove(PositionHelper::Create(fromX, fromY), PositionHelper::Create(toX, toY));

    FigureSide turningSide = m_board->GetTurningSide();

    CheckForGameOver();

    if (!m_aiThinking && m_rules->GetPossibleMoves(turningSide).count() != 0)
    {
        StartStepByComputer();
    }

    //QSharedPointer<Move> bestAIMove = m_ai->BestMove(m_board->GetTurningSide(), 5);
    //QFuture<QSharedPointer<Move>> future = QtConcurrent::run(m_ai, &AI::BestMove, m_board->GetTurningSide(), 5);
    //QSharedPointer<Move> bestAIMove = future.result();

//    qDebug() << "AI best move:" << *bestAIMove.data();

//    m_rules->MakeMove(*bestAIMove.data());

//    if (m_rules->IsUnderCheck(FigureSide::White))
//    {
//        if (m_rules->GetPossibleMoves(FigureSide::White).count() == 0)
//        {
//            qDebug() << "You loose";
//        } else
//        {
//            qDebug() << "Check to you!";
//        }
//    };
}

QVariantList ChessApp::GetPossible(int x, int y)
{
    //qDebug() << "ChessApp::GetPossible";

    QVariantList lst;

    Figure* f = m_board->FigureAt(PositionHelper::Create(x, y));

    if (f != NULL)
    {        
        PositionList positions = m_rules->GetPossibleDestinations(f);

        foreach(POSITION p, positions)
        {                                    
            lst << QVariant(PositionHelper::X(p)) << QVariant(PositionHelper::Y(p));
        }
    }

    return lst;
}

void ChessApp::TurnBack()
{
    qDebug() << "ChessApp::TurnBack";

    if (!m_aiThinking)
    {
        if (!m_board->IsHistoryEmpty())
        {
            // turn back AI move and user move

            //m_rules->UnMakeMove(m_board->GetLastMove());
            m_rules->UnMakeMove(m_board->GetLastMove());
        }
    }
}

QString ChessApp::GetFEN()
{
    return FEN::Evaluate(*m_board);
}

QString ChessApp::BoardToString()
{
    //return FEN::Evaluate(*m_board);
    return BoardSerializer::Save(*m_board);
}

void ChessApp::BoardFromString(QString s)
{
    qDebug() << "ChessApp::BoardFromString";

    try
    {
        Board deserialized = BoardSerializer::Load(s);

        // delete old
        disconnect(m_asyncAI, SIGNAL(BestMoveFinded(Move)), this, SLOT(HandleFindedBestMove(Move)));

        delete m_board;
        delete m_rules;
        delete m_asyncAI;

        // create new
        m_board = new Board(deserialized);
        m_rules = new Rules(m_board);
        m_asyncAI = new AsyncAI(m_board);

        connect(m_asyncAI, SIGNAL(BestMoveFinded(Move)), this, SLOT(HandleFindedBestMove(Move)), Qt::DirectConnection);

        //return FEN::Evaluate(*m_board);
        //m_board
    } catch (Exception e)
    {
        QMessageBox m;
        m.setText("Error while loading...");
        m.setInformativeText(QString::fromStdString(e.GetMessage()));
        m.exec();
    }

}
