#include "movelist.h"

const int MAX_MOVE_COUNT = 64;

MoveList::MoveList(QObject *parent) :
    QObject(parent)
{
    m_takeMost = false;
    m_counts.resize(MAX_DEPTH);
    m_level = 0;
    m_counts[m_level] = 0;

    for ( int i = 0; i < MAX_DEPTH; i++ )
        m_data[i].resize(MAX_MOVE_COUNT);

}

void MoveList::addMove(const Move &move)
{
    if (m_takeMost) {
        if (move[0] > m_maxDeep) {
            m_maxDeep = move[0];
            m_counts[m_level] = 0;
        }
        if (move[0] >= m_maxDeep) {
            addMoveToList(move);
        }
    } else {
        addMoveToList(move);
    }
}

void MoveList::clear()
{
    m_counts[m_level] = 0;
    m_maxDeep = 0;
}

void MoveList::incPly()
{
    m_level++;
    m_counts[m_level] = 0;
}

void MoveList::decPly()
{
    m_level--;
}

void MoveList::setTakeMost(bool value)
{
    m_takeMost = value;
}

int MoveList::movesCount() const
{
    return m_counts[m_level];
}

const Move& MoveList::operator[](int index) const
{
    return m_data[m_level][index];
}

void MoveList::addMoveToList(const Move &move)
{
    m_data[m_level][m_counts[m_level]] = move;
    m_counts[m_level]++;
}
