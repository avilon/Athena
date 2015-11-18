#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include "common.h"

class QStringList;
class Board;
class GameLaw;
class MoveGen;
class MoveList;

class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(const QString &name, QObject *parent = 0);

    void changeName(const QString &newName);
    void perft(int depth);
    unsigned long long perftNodes() const;

    int boardHeight() const;
    int boardWidth() const;
    int boardSize() const;
    int pieceAt(int index) const;

    int isLegalMove(int from, int to, QStringList *moveList);
private:
    GameLaw *m_gameLaw;
    Board *m_board;
    MoveGen *m_moveGen;
    MoveList *m_moveList;

    unsigned long long m_perftNodes;

    void doPerft(int depth);
};

#endif // ENGINE_H
