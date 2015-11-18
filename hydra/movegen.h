#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <QObject>
#include "move.h"

class GameLaw;
class Board;
class MoveList;

class MoveGen : public QObject
{
    Q_OBJECT
public:
    explicit MoveGen(GameLaw *gl, Board *board, MoveList *moveList, QObject *parent = 0);

    void generateAllCaptures();
    void generateAllMoves();
    void generateAllSilentMoves();

signals:

public slots:
    void changeSize(int boardSize, int piecesPerSide);
    //void changeSize();
    void changeLaw();

private:
    GameLaw *m_gameLaw;
    Board *m_board;
    MoveList *m_moveList;
    Move m_move;

    bool m_takeMost;
    bool m_manTakeBack;
    bool m_manTakeKing;
    bool m_promoteImmediately;
    bool m_flyingKing;
    int m_moveLeft;
    int m_moveRight;
    int m_ownColor;
    int m_ownMan;
    int m_ownKing;
    int m_oppColor;
    int m_oppKing;
    int m_dirs[4];
    int m_manDirs[3][2];

    void addKingCapture(int square, int fromDir, int deep);
    void addManCapture(int square, int fromDir, int deep);
    void addPromoteCapture(int square, int fromDir, int deep);
    void checkKingCapture(int square, int dirId);
    void checkKingMove(int square, int dir);
    void checkManCapture(int square, int dirId);
    void checkManMove(int square, int dir);
    void findKingCapture(int square, int dir, int deep, bool &found);
    void findManCapture(int square, int fromDir, int dir, int deep, bool &found);

    void init();
    void updateRules();
    void updateSizes();
};

#endif // MOVEGEN_H
