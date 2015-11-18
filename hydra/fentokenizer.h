#ifndef FENTOKENIZER_H
#define FENTOKENIZER_H

#include <QObject>
#include <QVector>

class FENTokenizer : public QObject
{
    Q_OBJECT
public:
    explicit FENTokenizer(QObject *parent = 0);
    void load(const QString &fen);
    int blackPiecesCount() const;
    int whitePiecesCount() const;
    int blackKingsCount() const;
    int whiteKingsCount() const;

    int whitePieceAt(int index) const;
    int whiteKingAt(int index) const;
    int blackPieceAt(int index) const;
    int blackKingAt(int index) const;

    int startColor() const;
private:
    QVector<int> m_whitePieces;
    QVector<int> m_whiteKings;
    QVector<int> m_blackPieces;
    QVector<int> m_blackKings;

    int m_startColor;
 };

#endif // FENTOKENIZER_H
