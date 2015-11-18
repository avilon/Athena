#include <QStringList>
#include "engine.h"
#include "board.h"
#include "gamelaw.h"
#include "movegen.h"
#include "movelist.h"

Engine::Engine(const QString &name, QObject *parent) :
    QObject(parent)
{
    m_gameLaw = new GameLaw(name, this);
    m_board = new Board(m_gameLaw, this);
    m_moveList = new MoveList(this);
    m_moveGen = new MoveGen(m_gameLaw, m_board, m_moveList, this);

    changeName(name);
}

void Engine::changeName(const QString &newName)
{
    m_gameLaw->changeLaw(newName);
    m_moveList->setTakeMost(m_gameLaw->takeMost());
    m_board->init();
}

void Engine::perft(int depth)
{
    m_perftNodes = 0;
    doPerft(depth);
}

unsigned long long Engine::perftNodes() const
{
    return m_perftNodes;
}

int Engine::boardHeight() const
{
    return m_board->rows();
}

int Engine::boardWidth() const
{
    return m_board->cols();
}

int Engine::boardSize() const
{
    return m_board->length();
}

int Engine::pieceAt(int index) const
{
    return m_board->pieceAt(index);
}

int Engine::isLegalMove(int from, int to, QStringList *moveList)
{
    int checkFrom = m_board->squareNum(from);
    int checkTo = m_board->squareNum(to);
    int onlyFromCnt = 0;
    int checkCnt = 0;

    m_moveGen->generateAllMoves();

    for ( int i = 0; i < m_moveList->movesCount(); i++ ) {
        Move move = (*m_moveList)[i];
        int fromSquare = move.from();
        int toSquare = move.to();

        if (fromSquare == checkFrom) {
            onlyFromCnt++;

            if ( toSquare == checkTo ) {
                checkCnt++;
            }
        }
    }
    return checkCnt;
}

void Engine::doPerft(int depth)
{
    m_moveGen->generateAllMoves();
    if (depth == 1) {
        m_perftNodes += m_moveList->movesCount();
        return;
    }
    depth--;
    for ( int i = 0; i < m_moveList->movesCount(); ++i ) {
        m_board->moveDo((*m_moveList)[i]);
        m_moveList->incPly();
        doPerft(depth);
        m_moveList->decPly();
        m_board->moveUndo( (*m_moveList)[i] );
    }
}
