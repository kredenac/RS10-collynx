#include "painter.h"
#include "ui_painter.h"
Painter::Painter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Painter)
{
    ui->setupUi(this);

    //da prepoznaje mouse movement
    this->setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);

}

Painter::~Painter()
{
    delete ui;
}

bool isMousePressed = false;
QColor lineColor = Qt::blue;

void Painter::mouseMoveEvent( QMouseEvent * event )
{

    //qDebug()<< isMousePressed;
    if (isMousePressed){
        QPoint newPoint = event->pos();
        poly << newPoint;
        this->update();
    }


}

void Painter::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        isMousePressed = false;
        lines.append(poly);
        poly.clear();
        this->update();
    }
}

void Painter::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton){
        isMousePressed = true;
    }

    if (event->button() == Qt::RightButton){
        lineColor = lineColor == Qt::blue ? Qt::white : Qt::blue;
    }
}

void Painter::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    //klasa pomocu koje crtamo
    QPainter painter(this);
    QPen pointPen(lineColor);
    pointPen.setWidth(7);

    painter.setPen(pointPen);
    for (auto l : lines){
        painter.drawPolyline(l);
    }
    //da iscrta i trenutni, nedovrsen poligon
    painter.drawPolyline(poly);
}
