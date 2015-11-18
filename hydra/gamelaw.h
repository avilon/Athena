#ifndef GAMELAW_H
#define GAMELAW_H

#include <QObject>
#include "common.h"

class GameLaw : public QObject
{
    Q_OBJECT
public:
    explicit GameLaw(const QString &name, QObject *parent = 0);

    const QString& name() const;
    int code() const;
    int boardHeight() const;
    int boardWidth() const;
    int piecesPerSide() const;
    int startColor() const;
    bool manTakeBack() const;
    bool manTakeKing() const;
    bool takeMost() const;
    bool promoteImmediate() const;
    bool flyingKing() const;
    bool changeLaw(const QString &name);

signals:
    void onChangeLaw();

public slots:

private:
    QString m_name;
    int m_code;
    int m_boardHeight;
    int m_boardWidth;
    int m_piecesPerSide;
    int m_startColor;
    bool m_manTakeBack;
    bool m_manTakeKing;
    bool m_takeMost;
    bool m_promoteImmediate;
    bool m_flyingKing;
};

#endif // GAMELAW_H
