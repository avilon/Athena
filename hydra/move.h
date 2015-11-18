#ifndef MOVE_H
#define MOVE_H

#include <vector>

const int MOVE_FROM_SQUARE_NDX = 1;
const int MOVE_FROM_PIECE_NDX  = 2;
const int MOVE_TO_SQUARE_NDX   = 3;
const int MOVE_TO_PIECE_NDX    = 4;
const int MOVE_FIRST_TAKE_NDX  = 5;

class Move
{
public:
    Move();
    Move(const Move &other);
    ~Move() {}

    Move& operator = (const Move &other);

    int from() const;
    int to() const;
    int before() const;
    int after() const;

    const int& operator[](int index) const;
    int& operator[](int index);

private:
    std::vector<int> m_line;
};

#endif // MOVE_H
