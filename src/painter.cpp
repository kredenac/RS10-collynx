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
QColor lineColor = QColor(10,177,200);

void Painter::mouseMoveEvent( QMouseEvent * event )
{

    //qDebug()<< isMousePressed;
    if (isMousePressed){
        QPoint newPoint = event->pos();
        //qDebug() << newPoint;
        QString coordsToSend = QString::number(newPoint.x()) + " " + QString::number(newPoint.y()) + " ";
        Sender::getInstance().send(coordsToSend);
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
        //send -1 -1 to others, so they 'relese the mouse' too
        QString coordsToSend = QString::number(-1) + " " + QString::number(-1) + " ";
        Sender::getInstance().send(coordsToSend);
    }
}

void Painter::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton){
        isMousePressed = true;
    }
    QColor blue = QColor(10,177,200);
    if (event->button() == Qt::RightButton){
        lineColor = lineColor == blue ? Qt::white : blue;
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

    //draw other users
    QPen otherPen(QColor(66, 15, 91));
    otherPen.setWidth(7);
    painter.setPen(otherPen);

    for (auto l : linesFromOthers){
        painter.drawPolyline(l);
    }
    painter.drawPolyline(otherUsersPoly);
}

void Painter::stringToPoly(QString str)
{
    QStringList listStr = str.split(' ', QString::SkipEmptyParts);
     qDebug() << "DEBUGGG"<<listStr.size();

    if (listStr.size() <= 1 || listStr.size() % 2){
        return;
    }
    for (int i = 0; i < listStr.size(); i+=2){
        qDebug() << "x je " << listStr[i] << ", y je " << listStr[i+1];
        QPoint newPoint(listStr[i].toInt(), listStr[i+1].toInt());
        //if -1 -1, then add to new poly
        if (newPoint.x() == -1 && newPoint.y() == -1 ){
            linesFromOthers.append(otherUsersPoly);
            otherUsersPoly.clear();
        }else{
            otherUsersPoly << newPoint;
        }
    }
    this->update();
}
