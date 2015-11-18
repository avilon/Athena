#include "move.h"

using namespace std;

const int MAX_MOVE_LENGTH = 128;

Move::Move()
{
    m_line.resize(MAX_MOVE_LENGTH);
}

Move::Move(const Move &other)
{
    m_line = other.m_line;
}

int Move::from() const
{
    return m_line[MOVE_FROM_SQUARE_NDX];
}

int Move::to() const
{
    return m_line[MOVE_TO_SQUARE_NDX];
}

int Move::before() const
{
    return m_line[MOVE_FROM_PIECE_NDX];
}

int Move::after() const
{
    return m_line[MOVE_TO_PIECE_NDX];
}

Move& Move::operator = (const Move &other)
{
    if (this == &other)
        return *this;
    m_line = other.m_line;

    return *this;
}

const int& Move::operator[](int index) const
{
    return m_line[index];
}

int& Move::operator[](int index)
{
    return m_line[index];
}
