#include <QStringList>
#include "fentokenizer.h"
#include "common.h"

FENTokenizer::FENTokenizer(QObject *parent) :
    QObject(parent)
{
}

void FENTokenizer::load(const QString &fen)
{
    QStringList pos = fen.split(":");

    m_startColor = WHITE;

    if (pos[0] == "W") {
        m_startColor = WHITE;
    }

    if (pos[0] == "B") {
        m_startColor = BLACK;
    }

    if (pos[WHITE][0] == 'W') {
        pos[WHITE][0] = ' ';
        pos[WHITE] = pos[WHITE].trimmed();

        QStringList squares = pos[WHITE].split(",");
        m_whitePieces.clear();
        m_whiteKings.clear();

        for ( int i = 0; i < squares.count(); ++i ) {
            if (squares[i][0] == 'K') {
                squares[i][0] = ' ';
                squares[i] = squares[i].trimmed();
                m_whiteKings.append(squares[i].toUInt());
            } else {
                m_whitePieces.append(squares[i].toUInt());
            }
        }
    }
    if (pos[WHITE][0] == 'B') {
        pos[WHITE][0] = ' ';
        pos[WHITE] = pos[WHITE].trimmed();

        QStringList squares = pos[WHITE].split(",");
        m_blackPieces.clear();
        m_blackKings.clear();

        for ( int i = 0; i < squares.count(); ++i ) {
            if (squares[i][0] == 'K') {
                squares[i][0] = ' ';
                squares[i] = squares[i].trimmed();
                m_blackKings.append(squares[i].toUInt());
            } else {
                m_blackPieces.append(squares[i].toUInt());
            }
        }
    }

    if (pos[BLACK][0] == 'W') {
        pos[BLACK][0] = ' ';
        pos[BLACK] = pos[BLACK].trimmed();

        QStringList squares = pos[BLACK].split(",");
        m_whitePieces.clear();
        m_whiteKings.clear();

        for ( int i = 0; i < squares.count(); ++i ) {
            if (squares[i][0] == 'K') {
                squares[i][0] = ' ';
                squares[i] = squares[i].trimmed();
                m_whiteKings.append(squares[i].toUInt());
            } else {
                m_whitePieces.append(squares[i].toUInt());
            }
        }
    }
    if (pos[BLACK][0] == 'B') {
        pos[BLACK][0] = ' ';
        pos[BLACK] = pos[BLACK].trimmed();

        QStringList squares = pos[BLACK].split(",");
        m_blackPieces.clear();
        m_blackKings.clear();

        for ( int i = 0; i < squares.count(); ++i ) {
            if (squares[i][0] == 'K') {
                squares[i][0] = ' ';
                squares[i] = squares[i].trimmed();
                m_blackKings.append(squares[i].toUInt());
            } else {
                m_blackPieces.append(squares[i].toUInt());
            }
        }
    }
}

int FENTokenizer::blackPiecesCount() const
{
    return m_blackPieces.count();
}

int FENTokenizer::whitePiecesCount() const
{
    return m_whitePieces.count();
}

int FENTokenizer::blackKingsCount() const
{
    return m_blackKings.count();
}

int FENTokenizer::whiteKingsCount() const
{
    return m_whiteKings.count();
}

int FENTokenizer::whitePieceAt(int index) const
{
    if ((index >= 0) && (index < m_whitePieces.count())) {
        return m_whitePieces[index];
    }
    return 0;
}

int FENTokenizer::whiteKingAt(int index) const
{
    if ((index >= 0) && (index < m_whiteKings.count())) {
        return m_whiteKings[index];
    }
    return 0;
}

int FENTokenizer::blackPieceAt(int index) const
{
    if ((index >= 0) && (index < m_blackPieces.count())) {
        return m_blackPieces[index];
    }
    return 0;
}

int FENTokenizer::blackKingAt(int index) const
{
    if ((index >= 0) && (index < m_blackKings.count())) {
        return m_blackKings[index];
    }
    return 0;
}

int FENTokenizer::startColor() const
{
    return m_startColor;
}
