#include "painter.h"
#include "ui_painter.h"

Painter::Painter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Painter), brushSize(7)
{
    ui->setupUi(this);
    isMousePressed = false;
    //da prepoznaje mouse movement
    setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);

    /*
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
    //setFixedSize(windowSize);
    setBaseSize(windowSize);
}

Painter::~Painter()
{
    delete ui;
}


void Painter::keyPressEvent(QKeyEvent * event)
{
    //qDebug() << "key = " << event->key();
    switch (event->key()) {
    case Qt::Key_Escape:
        QCoreApplication::quit();
        break;
    case Qt::Key_Plus:
        brushSize++;
        myLines.getLines().last().pen.setWidth(brushSize);
        break;
    case Qt::Key_Minus:
        brushSize > 1 ? brushSize-- : brushSize;
        myLines.getLines().last().pen.setWidth(brushSize);
        break;
    case Qt::Key_Z:
        myLines.undo();
        update();
    default:
        break;
    }

}

void Painter::mouseMoveEvent(QMouseEvent * event )
{
    //qDebug()<< isMousePressed;
    if (isMousePressed){
        QPoint newPoint = event->pos();
        //qDebug() << newPoint;
        //TODO dodaj u sender
        QString coordsToSend = QString::number(newPoint.x()) + " " + QString::number(newPoint.y()) + " ";
        Sender::getInstance().send(coordsToSend);
        myLines.addPoint(newPoint);
        update();
    }
}

void Painter::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        isMousePressed = false;
        myLines.newLine(myLines.getLines().last().pen, brushSize);
        update();
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
            QPen pointPen(color);
            pointPen.setWidth(brushSize);
            myLines.setPen(pointPen) ;
        }
    }
}

void Painter::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    //klasa pomocu koje crtamo
    QPainter painter(this);
    for (const auto l : myLines.getLines()){
        painter.setPen(l.pen);
        painter.drawPolyline(l.poly);
    }

    //draw other users
    QPen otherPen(QColor(66, 15, 91));
    otherPen.setWidth(7);
    painter.setPen(otherPen);
    for (const auto l : otherLines.getLines()){
        painter.setPen(l.pen);
        painter.drawPolyline(l.poly);
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
    update();
}

