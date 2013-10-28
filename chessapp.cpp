#include "chessapp.h"

#include "figure.h"
#include "lightfigureposition.h"

//#include <QtConcurrentRun>


ChessApp::ChessApp(QObject *parent)
    : QObject(parent)
{
    m_board = new Board();
    m_board->SetupStartPosition();

    m_rules = new Rules(m_board);
    m_asyncAI = new AsyncAI(m_board);

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

void ChessApp::HandleFindedBestMove(Move bestMove)
{
    qDebug() << "AI best move:" << bestMove;

    m_rules->MakeMove(bestMove.From, bestMove.To);

    if (m_rules->GetPossibleMoves(Figure::White).count() == 0)
    {
        if (m_rules->IsUnderCheck(Figure::White))
        {
            QMetaObject::invokeMethod(m_boardControl, "showNotification", Q_ARG(QVariant, "You lose"));
            qDebug() << "You lose";
        } else
        {
            QMetaObject::invokeMethod(m_boardControl, "showNotification", Q_ARG(QVariant, "Draw game"));
            qDebug() << "Draw game";
        }
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
            POSITION p = CreateFigurePosition(x, y);

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

        QMetaObject::invokeMethod(m_boardControl, "setCellColor", Q_ARG(QVariant, X(from)), Q_ARG(QVariant, Y(from)), Q_ARG(QVariant, "Orange"));
        QMetaObject::invokeMethod(m_boardControl, "setCellColor", Q_ARG(QVariant, X(to)), Q_ARG(QVariant, Y(to)), Q_ARG(QVariant, "Orange"));
    }
}

bool ChessApp::IsFigureCell(int x, int y)
{
    return m_board->FigureAt(CreateFigurePosition(x,y)) != NULL;
}

bool ChessApp::IsPossibleStep(int fromX, int fromY, int toX, int toY)
{
    //return true;
    //qDebug() << "ChessApp::IsPossibleStep";

    if (m_board->GetTurningSide() == Figure::Black) // is AI turn
        return false;

    Figure* f = m_board->FigureAt(CreateFigurePosition(fromX, fromY));

    if (f != NULL)
    {
        PositionList positions = m_rules->GetPossibleDestinations(f);

        //qDebug() << positions << positions.contains(LightFigurePosition(toX, toY));

        return positions.contains(CreateFigurePosition(toX, toY));
    } else
    {
        return false;
    }
}

void ChessApp::Step(int fromX, int fromY, int toX, int toY)
{
    //qDebug() << "ChessApp::Step";

    m_rules->MakeMove(CreateFigurePosition(fromX, fromY), CreateFigurePosition(toX, toY));

    if (m_rules->GetPossibleMoves(Figure::Black).count() == 0)
    {
        if (m_rules->IsUnderCheck(Figure::Black))
        {
            QMetaObject::invokeMethod(m_boardControl, "showNotification", Q_ARG(QVariant, "You win!"));
            qDebug() << "You win";
        } else
        {
            QMetaObject::invokeMethod(m_boardControl, "showNotification", Q_ARG(QVariant, "Draw game"));
            qDebug() << "Draw game";
        }
    } else
    {
        m_asyncAI->StartBestMoveSearch(m_board->GetTurningSide(), DEPTH);
        m_aiThinking = true;
    }

    //QSharedPointer<Move> bestAIMove = m_ai->BestMove(m_board->GetTurningSide(), 5);
    //QFuture<QSharedPointer<Move>> future = QtConcurrent::run(m_ai, &AI::BestMove, m_board->GetTurningSide(), 5);
    //QSharedPointer<Move> bestAIMove = future.result();

//    qDebug() << "AI best move:" << *bestAIMove.data();

//    m_rules->MakeMove(*bestAIMove.data());

//    if (m_rules->IsUnderCheck(Figure::White))
//    {
//        if (m_rules->GetPossibleMoves(Figure::White).count() == 0)
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

    Figure* f = m_board->FigureAt(CreateFigurePosition(x, y));

    if (f != NULL)
    {        
        PositionList positions = m_rules->GetPossibleDestinations(f);

        foreach(POSITION p, positions)
        {                                    
            lst << QVariant(X(p)) << QVariant(Y(p));
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
            m_rules->UnMakeMove(m_board->GetLastMove());
            m_rules->UnMakeMove(m_board->GetLastMove());
        }
    }
}
