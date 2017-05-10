#ifndef PAINTER_H
#define PAINTER_H
class Painter;
#include "sender.h"
#include "lines.h"
#include "transform.h"
#include "chooser.h"
#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <math.h>
#include <QVector>
#include <QColorDialog>
#include <QPushButton>

namespace Ui {
class Painter;
}

class Painter : public QMainWindow
{
    Q_OBJECT

public:
    explicit Painter(QWidget *parent = 0);
    void stringToPoly(QString str);
    Lines myLines;
    Lines otherLines;
    ~Painter();

public slots:
    void clickedButton();
    void snapshot();
private:
    Ui::Painter *ui;
    int brushSize;
    QColor selectColor(QPoint pos);
    Shape::Type nowDrawing;
    Shape::Type otherDrawing;
    void moveWidgetCenter(const QPoint &globalPos);
    void beginNewDrawable(const QPoint &pos);
    void stayOnTop(bool setTop);
    chooser c;
    bool alwaysOnTop;
protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event );
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    bool isMousePressed;
};

#endif // PAINTER_H
