#ifndef MOVELIST_H
#define MOVELIST_H

#include <QObject>
#include <QVector>
#include "move.h"

const int MAX_DEPTH = 256;

class MoveList : public QObject
{
    Q_OBJECT
public:
    explicit MoveList(QObject *parent = 0);
    void addMove(const Move &move);
    void clear();
    void incPly();
    void decPly();
    void setTakeMost(bool value);
    int movesCount() const;
    const Move& operator[](int index) const;

private:
    QVector<Move> m_data[MAX_DEPTH];
    QVector<int> m_counts;
    int m_level;
    int m_maxDeep;
    bool m_takeMost;

    void addMoveToList(const Move &move);

};

#endif // MOVELIST_H
