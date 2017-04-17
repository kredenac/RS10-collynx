#include "painter.h"
#include "ui_painter.h"
#include <QRect>
#include <QApplication>
Painter::Painter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Painter)
{

    ui->setupUi(this);
    isMousePressed = false;
    //da prepoznaje mouse movement
    setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);
    /*
    //
    //setStyleSheet("background-image : url(C:/Users/Dzoni/Documents/QtProjects/ColLynx/img_fjords.jpg);");
    //setStyleSheet("backgroud-style : cover");
    //setStyleSheet("opacity: 100);");
    */
    setStyleSheet("background : rgba(5,0,20,0.2);");
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::WindowSystemMenuHint);
    setWindowFlags(Qt::FramelessWindowHint);
    // strech window to fit screen
    QScreen * screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    //qDebug() << height << " " << width;
    //QSize windowSize(width,height);
    QSize windowSize(width/2,height/2);
    this->setFixedSize(windowSize);
}

Painter::~Painter()
{
    delete ui;
}

//bool isMousePressed = false;
QColor lineColor = QColor(10,177,200);

void Painter::mouseMoveEvent( QMouseEvent * event )
{
    //qDebug()<< isMousePressed;
    if (isMousePressed){
        QPoint newPoint = event->pos();
        //qDebug() << newPoint;
        //TODO dodaj u sender
        QString coordsToSend = QString::number(newPoint.x()) + " " + QString::number(newPoint.y()) + " ";
        Sender::getInstance().send(coordsToSend);
        //poly << newPoint;
        myLines.addPoint(newPoint);
        this->update();
    }
}

void Painter::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        isMousePressed = false;
        myLines.newLine();
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
    if (event->button() == Qt::RightButton){
        QColor color = QColorDialog::getColor(Qt::white, this, "Izaberite boju");
        if (color.isValid()){
            lineColor = color;
        }
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
    for (const auto l : myLines.getLines()){
        painter.drawPolyline(l);
    }

    //draw other users
    QPen otherPen(QColor(66, 15, 91));
    otherPen.setWidth(7);
    painter.setPen(otherPen);
    for (const auto l : otherLines.getLines()){

        painter.drawPolyline(l);
    }
}

void Painter::stringToPoly(QString str)
{
    QStringList listStr = str.split(' ', QString::SkipEmptyParts);
    qDebug() << "DEBUGGG"<< listStr.size();
    //maybe 0, not 1? if somethings goes wrong
    if (listStr.size() <= 1 || listStr.size() % 2){
        return;
    }
    //add a point to current line, or store the line in line vector
    for (int i = 0; i < listStr.size(); i+=2){
        qDebug() << "x je " << listStr[i] << ", y je " << listStr[i+1];
        QPoint newPoint(listStr[i].toInt(), listStr[i+1].toInt());
        //if -1 -1, then add to new poly
        if (newPoint.x() == -1 && newPoint.y() == -1 ){
            otherLines.newLine();
        }else{
            otherLines.addPoint(newPoint);
        }
    }
    this->update();
}

