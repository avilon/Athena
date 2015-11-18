#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QVector>
#include "common.h"

class GameLaw;
class FENTokenizer;
class Move;
class Position;

class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(GameLaw *gl, QObject *parent = 0);

    void clear();
    void init();
    void setup(const QString &pos);

    int cols() const;
    int rows() const;
    int moveLeft() const;
    int moveRight() const;
    int piecesPerSide() const;
    int moveColor() const;
    int startColor() const;
    int occupiedCount() const;
    int occupiedSquare(int index) const;
    int piecesCount(int color) const;

    /// Возвращает состояние поля square ( если поле пустое, вернется EMPTY
    /// иначе - фигура, которой занято поле)
    /// нумерация полей идет от 0 до boardSize - 1,
    /// где boardSize - количество игровых полей доски
    int pieceAt(const int square) const;
    /// Преобразует "нормальный" номер поля в номер ячейки в массиве m_squares
    int squareNum(const int square) const;

    /// Возвращает количество всех игровых полей + кол-во полей-ограничителей
    int size() const;
    /// Возвращает количество игровых полей (например, 32-для доски 8x8 или 50 для поля 10x10)
    int length() const;
    int promoValue(int color, int square) const;

    void moveDo(const Move &move);
    void moveUndo(const Move &move);

    const int& operator[](int index) const;
    int& operator[](int index);

signals:
    void onChangeSize(int boardSize, int piecesPerSide);

public slots:

private:
    GameLaw *m_gameLaw;
    Position *m_position;
    FENTokenizer *m_ft;

    int m_cols;
    int m_rows;
    int m_piecesPerSide;
    int m_startColor;
    int m_moveColor;
    int m_moveLeft;
    int m_moveRight;

    QVector<int> m_squares;
    QVector<int> m_map;
    QVector<int> m_promoMap[CHANGE_COLOR];

    void createPosition();
    void initMap();
    void initPromoMap();

    void updatePosition();
    void updateSize();

private slots:
    void changeLaw();
};

#endif // BOARD_H
