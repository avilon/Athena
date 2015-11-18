#include "gamelaw.h"

struct LawDefine
{
    int code;
    QString name;
    int boardHeight;
    int boardWidth;
    int piecesPerSide;
    int startColor;

    bool manTakeBack;
    bool takeMost;
    bool promoteImmediate;
    bool flyingKing;
    bool manTakeKing;
};

LawDefine lawDefines[] = {{27, "Canadian",      12, 12, 30, WHITE, true,  true,  false, true,  true},
                          {21, "Checkers",      8,  8,  12, BLACK, false, false, false, false, true},
                          {26, "Brazilian",     8,  8,  12, WHITE, true,  true,  false, true,  true},
                          {20, "International", 10, 10, 20, WHITE, true,  true,  false, true,  true},
                          {22, "Italian",       8,  8,  12, WHITE, false, true,  false, false, false},
                          {23, "Pool",          8,  8,  12, WHITE, true,  false, false, true,  true},
                          {25, "Russian",       8,  8,  12, WHITE, true,  false, true,  true,  true},
                          {0,  "",              0,  0,  0,  0,     false, false, false, false, false}};

GameLaw::GameLaw(const QString &name, QObject *parent) :
    QObject(parent),
    m_name(name)
{
    changeLaw(name);
}

const QString& GameLaw::name() const
{
    return m_name;
}

int GameLaw::code() const
{
    return m_code;
}

int GameLaw::boardHeight() const
{
    return m_boardHeight;
}

int GameLaw::boardWidth() const
{
    return m_boardWidth;
}

int GameLaw::piecesPerSide() const
{
    return m_piecesPerSide;
}

int GameLaw::startColor() const
{
    return m_startColor;
}

bool GameLaw::manTakeBack() const
{
    return m_manTakeBack;
}

bool GameLaw::manTakeKing() const
{
    return m_manTakeKing;
}

bool GameLaw::takeMost() const
{
    return m_takeMost;
}

bool GameLaw::promoteImmediate() const
{
    return m_promoteImmediate;
}

bool GameLaw::flyingKing() const
{
    return m_flyingKing;
}

bool GameLaw::changeLaw(const QString &name)
{
    for ( int i = 0; lawDefines[i].code; i++ ) {
        if (lawDefines[i].name == name) {
            m_name = lawDefines[i].name;
            m_code = lawDefines[i].code;
            m_boardHeight = lawDefines[i].boardHeight;
            m_boardWidth = lawDefines[i].boardWidth;
            m_piecesPerSide = lawDefines[i].piecesPerSide;
            m_startColor = lawDefines[i].startColor;
            m_manTakeBack = lawDefines[i].manTakeBack;
            m_takeMost = lawDefines[i].takeMost;
            m_promoteImmediate = lawDefines[i].promoteImmediate;
            m_flyingKing = lawDefines[i].flyingKing;
            m_manTakeKing = lawDefines[i].manTakeKing;

            emit(onChangeLaw());

            return true;
        }
    }
    return false;
}
