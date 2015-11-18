#-------------------------------------------------
#
# Project created by QtCreator 2015-11-16T17:24:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Athena
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    boardview.cpp \
    hydra/board.cpp \
    hydra/engine.cpp \
    hydra/fentokenizer.cpp \
    hydra/gamelaw.cpp \
    hydra/move.cpp \
    hydra/movegen.cpp \
    hydra/movelist.cpp \
    hydra/position.cpp

HEADERS  += mainwindow.h \
    boardview.h \
    hydra/board.h \
    hydra/common.h \
    hydra/engine.h \
    hydra/fentokenizer.h \
    hydra/gamelaw.h \
    hydra/move.h \
    hydra/movegen.h \
    hydra/movelist.h \
    hydra/position.h

FORMS    += mainwindow.ui
