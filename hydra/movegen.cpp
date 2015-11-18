#include "movegen.h"
#include "gamelaw.h"
#include "board.h"
#include "movelist.h"

MoveGen::MoveGen(GameLaw *gl, Board *board, MoveList *moveList, QObject *parent) :
    QObject(parent)
{
    m_gameLaw = gl;
    m_board = board;
    m_moveList = moveList;

    connect(gl, SIGNAL(onChangeLaw()), this, SLOT(changeLaw()));
    connect(board, SIGNAL(onChangeSize(int,int)), this, SLOT(changeSize(int,int)));

    updateRules();
    updateSizes();
}

void MoveGen::generateAllCaptures()
{
    init();

    for ( int i = 0; i < m_board->occupiedCount(); ++i ) {
        int square = m_board->occupiedSquare(i);
        if ( ((*m_board)[square]) & m_ownColor) {
            if ( ((*m_board)[square]) == m_ownMan) {
                if (m_manTakeBack) {
                    checkManCapture(square, 0);
                    checkManCapture(square, 1);
                    checkManCapture(square, 2);
                    checkManCapture(square, 3);
                } else {
                    if (m_board->moveColor() == WHITE) {
                        checkManCapture(square, 2);
                        checkManCapture(square, 3);
                    } else {
                        checkManCapture(square, 0);
                        checkManCapture(square, 1);
                    }
                }
            } else {
                checkKingCapture(square, 0);
                checkKingCapture(square, 1);
                checkKingCapture(square, 2);
                checkKingCapture(square, 3);
            }
        }
    }
}

void MoveGen::generateAllMoves()
{
    generateAllCaptures();
    if (!m_moveList->movesCount()) {
        generateAllSilentMoves();
    }
}

void MoveGen::generateAllSilentMoves()
{
    for ( int i = 0; i < m_board->occupiedCount(); ++i ) {
        int square = m_board->occupiedSquare(i);
        if ( ((*m_board)[square]) & m_ownColor) {
            if ( ((*m_board)[square]) == m_ownMan ) {
                checkManMove(square, m_manDirs[m_ownColor][0]);
                checkManMove(square, m_manDirs[m_ownColor][1]);
            } else {
                checkKingMove(square, m_moveLeft);
                checkKingMove(square, m_moveRight);
                checkKingMove(square, -m_moveLeft);
                checkKingMove(square, -m_moveRight);
            }
        }
    }
}

void MoveGen::changeLaw()
{
    updateRules();
}


void MoveGen::changeSize(int boardSize, int piecesPerSide)
{
    updateSizes();
}


void MoveGen::addKingCapture(int square, int fromDir, int deep)
{
    bool found = false;
    int next = square;
    int dir = m_dirs[fromDir];

    if (m_flyingKing) {
        while( (*m_board)[next] == EMPTY) {
            findKingCapture(next, fromDir^1, deep, found);
            findKingCapture(next, fromDir^3, deep, found);
            next += dir;
        }

        if (( (*m_board)[next] & m_oppColor) && ( (*m_board)[next+dir] == EMPTY)) {
            m_move[deep+1] = next;
            m_move[deep+2] = (*m_board)[next];
            (*m_board)[next] ^= CHANGE_COLOR;
            addKingCapture(next+dir, fromDir, deep+2);
            (*m_board)[next] ^= CHANGE_COLOR;
            return;
        }

        if (!found) {
            m_move[0] = deep;
            next = square;
            m_move[MOVE_TO_PIECE_NDX] = m_ownKing;
            while((*m_board)[next] == EMPTY) {
                m_move[MOVE_TO_SQUARE_NDX] = next;
                m_moveList->addMove(m_move);
                next += dir;
            }
        }
    } else {
        findKingCapture(square, 0, deep, found);
        findKingCapture(square, 1, deep, found);
        findKingCapture(square, 2, deep, found);
        findKingCapture(square, 3, deep, found);
        if (!found) {
            m_move[0] = deep;
            m_move[MOVE_TO_SQUARE_NDX] = square;
            m_move[MOVE_TO_PIECE_NDX] = m_ownKing;
            m_moveList->addMove(m_move);
        }
    }
}

void MoveGen::addManCapture(int square, int fromDir, int deep)
{
    bool found = false;

    if (m_manTakeBack) {
        findManCapture(square, fromDir, 0, deep, found);
        findManCapture(square, fromDir, 1, deep, found);
        findManCapture(square, fromDir, 2, deep, found);
        findManCapture(square, fromDir, 3, deep, found);
    } else {
        if ( m_board->moveColor() == WHITE) {
            findManCapture(square, fromDir, 2, deep, found);
            findManCapture(square, fromDir, 3, deep, found);
        } else {
            findManCapture(square, fromDir, 0, deep, found);
            findManCapture(square, fromDir, 1, deep, found);
        }
    }
    if (!found) {
        m_move[0] = deep;
        m_move[MOVE_TO_SQUARE_NDX] = square;
        m_move[MOVE_TO_PIECE_NDX] = m_ownMan;
        m_moveList->addMove(m_move);
    }
}

void MoveGen::addPromoteCapture(int square, int fromDir, int deep)
{
    int dir = m_dirs[fromDir^3];
    int next = square + dir;

    if (!m_flyingKing) {
        m_move[0] = deep;
        m_move[MOVE_TO_SQUARE_NDX] = square;
        m_move[MOVE_TO_PIECE_NDX] = m_ownKing;
        m_moveList->addMove(m_move);
        return;
    }
    if (m_promoteImmediately) {
        while( (*m_board)[next] == EMPTY ) {
            next += dir;
        }
    }
    if (( (*m_board)[next] & m_oppColor) && ( (*m_board)[next+dir] == EMPTY )) {
        m_move[deep+1] = next;
        m_move[deep+2] = (*m_board)[next];
        (*m_board)[next] ^= CHANGE_COLOR;
        if (m_promoteImmediately) {
            addKingCapture(next+dir, fromDir^3, deep+2);
        } else {
            addManCapture(next+dir, fromDir^3, deep+2);
        }
        (*m_board)[next] ^= CHANGE_COLOR;
    } else {
        m_move[0] = deep;
        m_move[MOVE_TO_SQUARE_NDX] = square;
        m_move[MOVE_TO_PIECE_NDX] = m_ownKing;
        m_moveList->addMove(m_move);
    }
}

void MoveGen::checkKingCapture(int square, int dirId)
{
    int dir = m_dirs[dirId];
    int next = square + dir;

    if (m_flyingKing) {
        while( (*m_board)[next] == EMPTY)
            next += dir;
    }

    if (( (*m_board)[next] & m_oppColor) && ( (*m_board)[next+dir] == EMPTY)) {
        m_move[MOVE_FROM_SQUARE_NDX] = square;
        m_move[MOVE_FROM_PIECE_NDX]  = m_ownKing;
        m_move[MOVE_FIRST_TAKE_NDX]  = next;
        m_move[MOVE_FIRST_TAKE_NDX + 1] = (*m_board)[next];

        (*m_board)[square] = EMPTY;
        (*m_board)[next] ^= CHANGE_COLOR;
        addKingCapture(next+dir, dirId, 6);
        (*m_board)[next] ^= CHANGE_COLOR;
        (*m_board)[square] = m_ownKing;
    }
}

void MoveGen::checkKingMove(int square, int dir)
{
    int next = square + dir;

    if (m_flyingKing) {
        while((*m_board)[next] == EMPTY) {
            m_move[0] = 4;
            m_move[MOVE_FROM_SQUARE_NDX] = square;
            m_move[MOVE_FROM_PIECE_NDX] = m_ownKing;
            m_move[MOVE_TO_SQUARE_NDX] = next;
            m_move[MOVE_TO_PIECE_NDX] = m_ownKing;
            m_moveList->addMove(m_move);
            next += dir;
        }
    } else {
        if ( (*m_board)[next] == EMPTY ) {
            m_move[0] = 4;
            m_move[MOVE_FROM_SQUARE_NDX] = square;
            m_move[MOVE_FROM_PIECE_NDX] = m_ownKing;
            m_move[MOVE_TO_SQUARE_NDX] = next;
            m_move[MOVE_TO_PIECE_NDX] = m_ownKing;
            m_moveList->addMove(m_move);
        }
    }
}

void MoveGen::checkManCapture(int square, int dirId)
{
    int next = square + m_dirs[dirId];
    if ( (*m_board)[next] & m_oppColor ) {
        if ((!m_manTakeKing) && ((*m_board)[next] == m_oppKing))
            return;

        int dest = next + m_dirs[dirId];
        if ( (*m_board)[dest] == EMPTY) {
            // Запомним, откуда начался ход и первую взятую фигуру, походу к ней могут добавится и другие
            m_move[MOVE_FROM_SQUARE_NDX] = square;
            m_move[MOVE_FROM_PIECE_NDX]  = m_ownMan;
            m_move[MOVE_FIRST_TAKE_NDX]  = next;
            m_move[MOVE_FIRST_TAKE_NDX + 1] = (*m_board)[next];
            (*m_board)[square] = EMPTY;
            (*m_board)[next] ^= CHANGE_COLOR;
            if (m_board->promoValue(m_ownColor, dest) == m_ownMan) {
                addManCapture(dest, dirId, 6);
            } else {
                addPromoteCapture(dest, dirId, 6);
            }
            (*m_board)[next] ^= CHANGE_COLOR;
            (*m_board)[square] = m_ownMan;
        }
    }
}

void MoveGen::checkManMove(int square, int dir)
{
    int next = square + dir;

    if ( (*m_board)[next] == EMPTY) {
        m_move[0] = 4;
        m_move[MOVE_FROM_SQUARE_NDX] = square;
        m_move[MOVE_FROM_PIECE_NDX] = m_ownMan;
        m_move[MOVE_TO_SQUARE_NDX] = next;
        m_move[MOVE_TO_PIECE_NDX] = m_board->promoValue(m_ownColor, next);
        m_moveList->addMove(m_move);
    }
}

void MoveGen::findKingCapture(int square, int dir, int deep, bool &found)
{
    int next;
    int d = m_dirs[dir];

    next = square + d;
    if (m_flyingKing) {
        while( (*m_board)[next] == EMPTY )
            next += d;
    }

    if (( (*m_board)[next] & m_oppColor) && ( (*m_board)[next+d] == EMPTY)) {
        found = true;
        m_move[deep+1] = next;
        m_move[deep+2] = (*m_board)[next];
        (*m_board)[next] ^= CHANGE_COLOR;
        addKingCapture(next+d, dir, deep+2);
        (*m_board)[next] ^= CHANGE_COLOR;
    }
}

void MoveGen::findManCapture(int square, int fromDir, int dir, int deep, bool &found)
{
    if ((fromDir^2) == dir)
        return;

    int next = square + m_dirs[dir];

    if ( ((*m_board)[next]) & m_oppColor ) {
        if ((!m_manTakeKing) && ( (*m_board)[next] == m_oppKing ))
            return;

        int dest = next + m_dirs[dir];
        if ( ((*m_board)[dest]) == EMPTY ) {
            found = true;
            m_move[deep+1] = next;
            m_move[deep+2] = ((*m_board)[next]);
            (*m_board)[next] ^= CHANGE_COLOR;
            if ( m_board->promoValue(m_ownColor, dest) == m_ownMan ) {
                addManCapture(dest, dir, deep+2);
            } else {
                addPromoteCapture(dest, dir, deep+2);
            }
            (*m_board)[next] ^= CHANGE_COLOR;
        }
    }
}

void MoveGen::init()
{
    int color = m_board->moveColor();

    m_ownColor = color;
    m_ownMan = m_ownColor + MAN;
    m_ownKing = m_ownColor + KING;
    m_oppColor = color ^ CHANGE_COLOR;
    m_oppKing = m_oppColor + KING;
    m_moveList->clear();
}

void MoveGen::updateRules()
{
    m_takeMost = m_gameLaw->takeMost();
    m_manTakeBack = m_gameLaw->manTakeBack();
    m_manTakeKing = m_gameLaw->manTakeKing();
    m_promoteImmediately = m_gameLaw->promoteImmediate();
    m_flyingKing = m_gameLaw->flyingKing();
}

void MoveGen::updateSizes()
{
    if (!m_board)
        return;

    m_moveLeft = m_board->moveLeft();
    m_moveRight = m_board->moveRight();

    m_dirs[0] = m_moveRight;
    m_dirs[1] = m_moveLeft;
    m_dirs[2] = -m_moveRight;
    m_dirs[3] = -m_moveLeft;

    m_manDirs[0][0] = 0;
    m_manDirs[0][1] = 0;
    m_manDirs[WHITE][0] = -m_moveRight;
    m_manDirs[WHITE][1] = -m_moveLeft;
    m_manDirs[BLACK][0] = m_moveLeft;
    m_manDirs[BLACK][1] = m_moveRight;
}
