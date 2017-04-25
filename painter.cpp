#include "painter.h"
#include "ui_painter.h"

Painter::Painter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Painter), brushSize(7), nowDrawing(Shape::Type::line)
{
    ui->setupUi(this);

    isMousePressed = false;
    setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);
    /*
    //setStyleSheet("background-image : url(C:/Users/Dzoni/Documents/QtProjects/ColLynx/img_fjords.jpg);");
    //setStyleSheet("backgroud-style : cover");
    */
    setStyleSheet("background : rgba(5,0,20,0.2);");
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFocusPolicy(Qt::StrongFocus);

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
    /**********/

}
/**TMP test****/
QPixmap * testScreenPtr = NULL;
/*************/
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
    case Qt::Key_Z:{
        myLines.undo();
        Sender::getInstance().send(Sender::Tag::undo);
        update();
    }
        break;
    case Qt::Key_T:{
        static bool test = false;
        stayOnTop(test);
        test = ! test;
        /*test*/
        /*postavlja ss za background*/
        static QPixmap testScreen;
        QScreen *screen = QGuiApplication::primaryScreen();
        testScreen = screen->grabWindow(0);
        testScreenPtr = &testScreen;
        qDebug() << testScreen.toImage().byteCount();
    }
        break;
    case Qt::Key_1:
        nowDrawing = Shape::Type::line;
        myLines.changeLastType(nowDrawing);
        break;
    case Qt::Key_2:
        nowDrawing = Shape::Type::ellipse;
        myLines.changeLastType(nowDrawing);
        break;
    case Qt::Key_3:
        nowDrawing = Shape::Type::rectangle;
        myLines.changeLastType(nowDrawing);
        break;
    default:
        break;
    }
}

void Painter::stayOnTop(bool setTop)
{
    if (setTop){
        this->parentWidget()->setWindowFlags(
                    this->parentWidget()->windowFlags() | Qt::WindowStaysOnTopHint);
        this->parentWidget()->show();
    } else {
        this->parentWidget()->setWindowFlags(
                    this->parentWidget()->windowFlags() & ~Qt::WindowStaysOnTopHint);
        this->parentWidget()->show();
    }
}

void Painter::mouseMoveEvent(QMouseEvent * event )
{
    //qDebug()<< isMousePressed;
    if (isMousePressed){
        QPoint newPoint = event->pos();

        Sender::getInstance().send(newPoint);
        myLines.addPoint(newPoint, nowDrawing);
        update();
    }
}

void Painter::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        isMousePressed = false;
        myLines.newLine(myLines.getLines().last().pen, brushSize, nowDrawing);
        Sender::getInstance().send(Sender::Tag::endline);
    }
}

void Painter::mousePressEvent(QMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        isMousePressed = true;
        beginNewDrawable(event->pos());
        update();
        break;
    case Qt::RightButton:
        selectColor(event->pos());
        break;
    case Qt::MiddleButton:
        moveWidgetCenter(event->globalPos());
        break;
    default:
        break;
    }
}

void Painter::beginNewDrawable(const QPoint & pos)
{
    //send pen info to others
    //qDebug() << event->pos();
    QPoint p(pos);
    myLines.addPoint(p, nowDrawing);
    Sender::getInstance().send(nowDrawing);
    Sender::getInstance().send(myLines.getLines().last().pen);
    Sender::getInstance().send(pos);
}

void Painter::moveWidgetCenter(const QPoint& globalPos)
{
    QPoint offset(width()/2, height()/2);
    parentWidget()->move(globalPos - offset);
}


QColor Painter::selectColor(QPoint pos)
{
    QColorDialog parent(this);
    parent.setStyleSheet("background : rgba(175, 175, 176, 1);");

    parent.move(pos.x(), pos.y());
    QColor color( QColorDialog::getColor(Qt::white, &parent, "Choose color") );
    if (color.isValid()){
        QPen pointPen(color);
        pointPen.setWidth(brushSize);
        myLines.setPen(pointPen);
    }
    return color;
}

void Painter::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    if (testScreenPtr){
        painter.drawPixmap(QPoint(0,0),*testScreenPtr);
    }
    myLines.drawAll(painter);
    otherLines.drawAll(painter);
}

void Painter::stringToPoly(QString str)
{
    QStringList listStr = str.split(' ', QString::SkipEmptyParts);
    //maybe 0, not 1? if somethings goes wrong

    if (listStr.size() <= 1 || listStr.size() % 2){
        return;
    }

    if (str.length() > 1000 ){
        qDebug() << "primih sliku";
    }
    //parse incoming string according to first tag
    for (int i = 0; i < listStr.size(); i+=2){
        //qDebug() << "x je " << listStr[i] << ", y je " << listStr[i+1];
        QPoint newPoint(listStr[i].toInt(), listStr[i+1].toInt());

        switch (newPoint.x()) {
        case Sender::Tag::color:
            otherLines.setPenColor(listStr[i+1].toInt(Q_NULLPTR, 16));
            break;
        case Sender::Tag::width:
            otherLines.setPenWidth(listStr[i+1].toInt());
            break;
        case Sender::Tag::shape:
            otherDrawing = static_cast<Shape::Type>(listStr[i+1].toInt());
            otherLines.changeLastType(otherDrawing);
            break;
        case Sender::Tag::undo:
            otherLines.undo();
            break;
        case Sender::Tag::endline:
            if (newPoint.y() == Sender::Tag::endline) {
                otherLines.newLine(otherDrawing);
                break;
            }
        default:
            otherLines.addPoint(newPoint, otherDrawing);
            break;
        }
    }
    update();
}


Painter::~Painter()
{
    delete ui;
}
