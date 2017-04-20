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
    setMinimumSize(windowSize);

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
    case Qt::Key_T:
    {
        QPen pen;
        Sender::getInstance().send(pen);
    }
    default:
        break;
    }
}

void Painter::mouseMoveEvent(QMouseEvent * event )
{
    //qDebug()<< isMousePressed;
    if (isMousePressed){
        QPoint newPoint = event->pos();

        Sender::getInstance().send(newPoint);
        myLines.addPoint(newPoint);
        update();
    }
}

void Painter::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        isMousePressed = false;
        //Transform::shift(150, 0, myLines.getLines().last());
        myLines.newLine(myLines.getLines().last().pen, brushSize);
        update();
        //Sender::getInstance().send(Sender::Endl)
        //send -1 -1 to others, so they 'relese the mouse' too
        QString coordsToSend = QString::number(-1) + " " + QString::number(-1) + " ";
        Sender::getInstance().send(coordsToSend);
    }
}

void Painter::mousePressEvent(QMouseEvent *event)
{

    switch (event->button()) {
    case Qt::LeftButton:{
        isMousePressed = true;
        //send pen info to others
        qDebug() << event->pos();
        QPoint p(event->pos());
        myLines.addPoint(p);
        //grozota ako hocemo da se vidi tacka cim se klikne
        /*int x =p.x() + 1;
        p.setX(x);
        myLines.addPoint(p);*/
        Sender::getInstance().send(myLines.getLines().last().pen);
        Sender::getInstance().send(event->pos());
        }
        break;
    case Qt::RightButton:{
       QColor color(selectColor(event->pos()) );
        if (color.isValid()){
            QPen pointPen(color);
            pointPen.setWidth(brushSize);
            myLines.setPen(pointPen) ;
            break;
        }
        }
        break;
    case Qt::MiddleButton:
        move(event->pos());
        break;
    default:
        break;
    }

}

QColor Painter::selectColor(QPoint pos)
{
    QColorDialog parent(this);
    parent.setStyleSheet("background : rgba(175, 175, 176, 1);");

    parent.move(pos.x(), pos.y());
    return QColorDialog::getColor(Qt::white, &parent, "Choose color");
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
    //qDebug() << "DEBUGGG"<< listStr.size();
    //maybe 0, not 1? if somethings goes wrong
    if (listStr.size() <= 1 || listStr.size() % 2){
        return;
    }
    //add a point to current line, or store the line in line vector
    for (int i = 0; i < listStr.size(); i+=2){
        qDebug() << "x je " << listStr[i] << ", y je " << listStr[i+1];
        QPoint newPoint(listStr[i].toInt(), listStr[i+1].toInt());
        //if -1 -1, then add to new poly
        if (newPoint.x() == -2){
            qDebug() << "STIGLA BOJA ^_^";

            qDebug() << "col: "<< listStr[i+1].toInt(Q_NULLPTR, 16);
            otherLines.setPenColor(listStr[i+1].toInt(Q_NULLPTR, 16));
        } else if (newPoint.x() == -3){
            qDebug() << "stigao W I D T H" << listStr[i+1].toInt();
            otherLines.setPenWidth(listStr[i+1].toInt());

        }
        else if (newPoint.x() == -1 && newPoint.y() == -1 ){
            otherLines.newLine();
        }else{
            otherLines.addPoint(newPoint);
        }
    }
    update();
}


Painter::~Painter()
{
    delete ui;
}
