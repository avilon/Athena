#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QWidget>

class Engine;

class BoardView : public QWidget
{
    Q_OBJECT
public:
    explicit BoardView(QWidget *parent = 0);

signals:

public slots:
};

#endif // BOARDVIEW_H
