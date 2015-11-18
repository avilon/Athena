#include "board.h"
#include "gamelaw.h"
#include "move.h"
#include "position.h"
#include "fentokenizer.h"

Board::Board(GameLaw *gl, QObject *parent) :
    QObject(parent)
{
    m_gameLaw = gl;
    updateSize();
    createPosition();

    connect(m_gameLaw, SIGNAL(onChangeLaw()), this, SLOT(changeLaw()));
    connect(this, SIGNAL(onChangeSize(int, int)), m_position, SLOT(changeSize(int,int)));

    m_ft = new FENTokenizer(this);

    init();
}

void Board::clear()
{
    for ( int i = 0; i < m_squares.size(); ++i ) {
        m_squares[i] = EDGE;
    }
    for ( int i = 0; i < m_map.size(); ++i )
        m_squares[m_map[i]] = EMPTY;

    if (m_position)
        m_position->clear();
}

void Board::init()
{
    clear();

    for ( int i = 0; i < m_piecesPerSide; ++i )
        m_squares[m_map[i]] = BLACK_MAN;
    for ( int i = m_map.size() - 1; i > (m_map.size() - 1 - m_piecesPerSide); i-- )
        m_squares[m_map[i]] = WHITE_MAN;

    m_moveColor = m_startColor;
    updatePosition();
}

void Board::setup(const QString &pos)
{
    m_ft->load(pos);
    clear();

    if (m_ft->whitePiecesCount() > 0) {
        for ( int i = 0; i < m_ft->whitePiecesCount(); ++i ) {
            m_squares[m_map[m_ft->whitePieceAt(i) - 1]] = WHITE_MAN;
        }
    }

    if (m_ft->whiteKingsCount() > 0) {
        for ( int i = 0; i < m_ft->whiteKingsCount(); ++i ) {
            m_squares[m_map[m_ft->whiteKingAt(i) - 1]] = WHITE_KING;
        }
    }

    if (m_ft->blackPiecesCount() > 0) {
        for ( int i = 0; i < m_ft->blackPiecesCount(); ++i ) {
            m_squares[m_map[m_ft->blackPieceAt(i) - 1]] = BLACK_MAN;
        }
    }

    if (m_ft->blackKingsCount() > 0) {
        for ( int i = 0; i < m_ft->blackKingsCount(); ++i ) {
            m_squares[m_map[m_ft->blackKingAt(i) - 1]] = BLACK_KING;
        }
    }

    m_moveColor = m_ft->startColor();
    updatePosition();
}

int Board::cols() const
{
    return m_cols;
}

int Board::rows() const
{
    return m_rows;
}

int Board::moveLeft() const
{
    return m_moveLeft;
}

int Board::moveRight() const
{
    return m_moveRight;
}

int Board::piecesPerSide() const
{
    return m_piecesPerSide;
}

int Board::moveColor() const
{
    return m_moveColor;
}

int Board::startColor() const
{
    return m_startColor;
}

int Board::occupiedCount() const
{
    return m_position->occupiedCount(m_moveColor);
}

int Board::occupiedSquare(int index) const
{
    return m_position->occupiedItem(m_moveColor, index);
}

int Board::piecesCount(int color) const
{
    if (m_position)
        return m_position->occupiedCount(color);

    return 0;
}

const int& Board::operator[](int index) const
{
    return m_squares[index];
}

int Board::pieceAt(const int square) const
{
    return m_squares[m_map[square]];
}

int Board::squareNum(const int square) const
{
    return m_map[square];
}

int Board::size() const
{
    return m_squares.size();
}

int Board::length() const
{
    return m_map.size();
}

int Board::promoValue(int color, int square) const
{
    return m_promoMap[color][square];
}

void Board::moveDo(const Move &move)
{
    int from, to;
    int piece;
    int n = move[0];

    from = move[MOVE_FROM_SQUARE_NDX];
    m_squares[from] = EMPTY;
    to = move[MOVE_TO_SQUARE_NDX];
    piece = move[MOVE_TO_PIECE_NDX];
    m_squares[to] = piece;

    m_position->movePiece(piece&CHANGE_COLOR, from, to);

    for ( int i = MOVE_FIRST_TAKE_NDX; i < n; i += 2 ) {
            from = move[i];
            piece = m_squares[from];
            m_squares[from] = EMPTY;
            m_position->takePiece(piece&CHANGE_COLOR, from);
    }

    m_moveColor ^= CHANGE_COLOR;
}

void Board::moveUndo(const Move &move)
{
    int from;
    int to;
    int piece;
    int n = move[0];

    to = move[MOVE_TO_SQUARE_NDX];
    m_squares[to] = EMPTY;
    from = move[MOVE_FROM_SQUARE_NDX];
    piece = move[MOVE_FROM_PIECE_NDX];
    m_squares[from] = piece;

    m_position->movePiece(piece&CHANGE_COLOR, to, from);

    for ( int i = MOVE_FIRST_TAKE_NDX; i < n; i += 2) {
            to = move[i];
            piece = move[i+1];
            m_squares[to] = piece;
            m_position->setPiece(piece&CHANGE_COLOR, to);
    }

    m_moveColor ^= CHANGE_COLOR;
}

int& Board::operator[](int index)
{
    return m_squares[index];
}

void Board::createPosition()
{
    m_position = new Position(m_squares.size(), m_piecesPerSide);
}

void Board::initMap()
{
    int n = m_cols/2 + 1;
    int nn = n + m_cols;
    int cnt = m_rows / 2;
    int p = 0;

    for ( int i = 0; i < cnt; i++ ) {

        for ( int j = n; j < nn; j++ ) {
            m_map[p++] = j;
        }
        n = nn + 1;
        nn = n + m_cols;
    }
}

void Board::initPromoMap()
{
    int n = m_squares.size();

    for ( int i = 0; i < n; ++i ) {
        m_promoMap[WHITE][i] = EMPTY;
        m_promoMap[BLACK][i] = EMPTY;
    }
    for ( int i = 0; i < m_map.size(); ++i ) {
        m_promoMap[WHITE][m_map[i]] = WHITE_MAN;
        m_promoMap[BLACK][m_map[i]] = BLACK_MAN;
    }
    for ( int i = 0; i < m_cols/2; ++i )
        m_promoMap[WHITE][m_map[i]] = WHITE_KING;
    for ( int i = m_map.size() - 1; i > (m_map.size() - 1 - m_cols/2); i-- )
        m_promoMap[BLACK][m_map[i]] = BLACK_KING;
}

void Board::updatePosition()
{
    m_position->clear();
    for ( int i = 0; i < m_map.size(); i++ ) {
        if (m_squares[m_map[i]] != EMPTY) {
            m_position->setPiece(m_squares[m_map[i]]&CHANGE_COLOR, m_map[i]);
        }
    }
}

void Board::updateSize()
{
    int newSize;

    m_cols = m_gameLaw->boardWidth();
    m_rows = m_gameLaw->boardHeight();
    m_piecesPerSide = m_gameLaw->piecesPerSide();
    m_startColor = m_gameLaw->startColor();

    newSize = m_cols * m_rows / 2;
    m_map.resize(newSize);
    newSize += m_cols + (m_rows / 2 - 1) + 2;
    m_squares.resize(newSize);
    m_promoMap[WHITE].resize(newSize);
    m_promoMap[BLACK].resize(newSize);

    m_moveRight = m_cols / 2;
    m_moveLeft = m_moveRight + 1;

    initMap();
    initPromoMap();

    emit(onChangeSize(m_squares.size(), m_piecesPerSide));
}

void Board::changeLaw()
{
    updateSize();
}

