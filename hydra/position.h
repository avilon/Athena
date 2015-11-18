#ifndef POSITION_H
#define POSITION_H

#include <QObject>
#include <QVector>
#include "common.h"

class Position : public QObject
{
    Q_OBJECT
public:
    explicit Position(int boardSize, int piecesPerSide, QObject *parent = 0);

    void clear();
    void movePiece(int color, int from, int to);
    void setPiece(int color, int to);
    void takePiece(int color, int from);

    int occupiedCount(int color) const;
    int occupiedItem(int color, int index) const;
signals:

public slots:
    void changeSize(int boardSize, int piecesPerSide);

private:
    int m_boardSize;
    int m_piecesPerSide;
    QVector<int> m_occupied[CHANGE_COLOR];
    QVector<int> m_occupiedNdx[CHANGE_COLOR];
    int m_occupiedCnt[CHANGE_COLOR];

    void resize();
};

#endif // POSITION_H
