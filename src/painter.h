#ifndef PAINTER_H
#define PAINTER_H
class Painter;
#include "sender.h"
#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <math.h>
#include <QVector>


namespace Ui {
class Painter;
}

class Painter : public QMainWindow
{
    Q_OBJECT

public:
    explicit Painter(QWidget *parent = 0);
    void stringToPoly(QString str);
    //niz poligona
    QVector<QPolygon> lines;
    QVector<QPolygon> linesFromOthers;
    QPolygon otherUsersPoly;
    QPolygon poly;
    ~Painter();

private:
    Ui::Painter *ui;


//posto hocemo da crtamo, zovi tad ovu fju.
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event );
    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // PAINTER_H
