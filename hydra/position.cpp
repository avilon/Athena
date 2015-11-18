#include "position.h"

Position::Position(int boardSize, int piecesPerSide, QObject *parent) :
    QObject(parent),
    m_boardSize(boardSize)
{
    changeSize(boardSize, piecesPerSide);
}

void Position::clear()
{
    for ( int i = 0; i < m_boardSize; ++i ) {
        m_occupiedNdx[WHITE][i] = EMPTY;
        m_occupiedNdx[BLACK][i] = EMPTY;
    }
    for ( int i = 0; i < m_piecesPerSide; ++i ) {
        m_occupied[WHITE][i] = EMPTY;
        m_occupied[BLACK][i] = EMPTY;
    }

    m_occupiedCnt[WHITE] = 0;
    m_occupiedCnt[BLACK] = 0;
}

void Position::movePiece(int color, int from, int to)
{
    int ndx = m_occupiedNdx[color][from];

    m_occupiedNdx[color][from] = -1;
    m_occupiedNdx[color][to] = ndx;
    m_occupied[color][ndx] = to;
}

void Position::setPiece(int color, int to)
{
    m_occupiedNdx[color][to] = m_occupiedCnt[color];
    m_occupied[color][m_occupiedCnt[color]] = to;
    m_occupiedCnt[color]++;
}

void Position::takePiece(int color, int from)
{
    int lastSquare;
    int ndx = m_occupiedNdx[color][from];

    m_occupiedCnt[color]--;
    lastSquare = m_occupied[color][m_occupiedCnt[color]];
    m_occupied[color][m_occupiedCnt[color]] = 0;

    if ( ndx < m_occupiedCnt[color] ) {
            m_occupied[color][ndx] = lastSquare;
    }
    m_occupiedNdx[color][lastSquare] = ndx;
    m_occupiedNdx[color][from] = -1;
}

int Position::occupiedCount(int color) const
{
    return m_occupiedCnt[color];
}

int Position::occupiedItem(int color, int index) const
{
    return m_occupied[color][index];
}

void Position::changeSize(int boardSize, int piecesPerSide)
{
    m_boardSize = boardSize;
    m_piecesPerSide = piecesPerSide;
    resize();
    clear();
}


void Position::resize()
{
    m_occupied[WHITE].resize(m_piecesPerSide);
    m_occupied[BLACK].resize(m_piecesPerSide);
    m_occupiedNdx[WHITE].resize(m_boardSize);
    m_occupiedNdx[BLACK].resize(m_boardSize);
    m_occupiedCnt[WHITE] = 0;
    m_occupiedCnt[BLACK] = 0;
}
