#ifndef CHESSAPP_H
#define CHESSAPP_H

#include <QObject>
#include <QList>

#include "board.h"
#include "rules.h"
#include "asyncai.h"

class ChessApp : public QObject
{
    Q_OBJECT

    Board* m_board;
    Rules* m_rules;
    AsyncAI* m_asyncAI;

    QObject* m_boardControl;

    bool m_aiThinking;
    bool m_boardChangedSinceAiStart;

    static const int DEPTH = 4;

    PositionList ToDestinations(MoveCollection list);
    void CheckForGameOver();
public:
    explicit ChessApp(QObject *parent = 0);
    virtual ~ChessApp();

    void SetBoardControl(QObject* root);   
signals:
private slots:
    void HandleFindedBestMove(Move bestMove);
public slots:
    Q_INVOKABLE void Draw();
    Q_INVOKABLE bool IsFigureCell(int x, int y);
    Q_INVOKABLE bool IsPossibleStep(int fromX, int fromY, int toX, int toY);
    Q_INVOKABLE void Step(int fromX, int fromY, int toX, int toY);    
    Q_INVOKABLE QVariantList GetPossible(int x, int y);
    Q_INVOKABLE void TurnBack();
    Q_INVOKABLE void StartStepByComputer();

    Q_INVOKABLE QString GetFEN();
    Q_INVOKABLE QString BoardToString();
    Q_INVOKABLE void BoardFromString(QString s);
};

#endif // CHESSAPP_H
