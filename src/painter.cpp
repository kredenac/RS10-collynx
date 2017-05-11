#include "painter.h"
#include "ui_painter.h"

Painter::Painter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Painter), brushSize(7), nowDrawing(Shape::Type::line), c(parent), alwaysOnTop(false)
{
    _id = "";
    for(int i=1; i<=c.getButtonNum(); i++){
        QObject::connect(c.getButton(i), SIGNAL(clicked()), this, SLOT(clickedButton()));
    }

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
//FIXME iz nekog razloga mora da se klikne da bi se video window kad ulazi u focus
void Painter::focusInEvent(QFocusEvent *event)
{
    qDebug() << "always on top " << alwaysOnTop;
    if (!alwaysOnTop) return;
    static int i = 0;
    if (event->gotFocus()){
        qDebug() << "got focus" << i++;
        //da registruje input

        this->parentWidget()->setWindowFlags(
                    this->parentWidget()->windowFlags() & ~Qt::WindowTransparentForInput);
        this->parentWidget()->show();


        //this->parentWidget()->update();
        //this->show();
        //this->setFocus();

        //moveWidgetCenter(this->parentWidget()->pos() + QPoint(-5,-5) );
    }
}

void Painter::focusOutEvent(QFocusEvent *event)
{
    if (!alwaysOnTop) return;
    static int i=0;
    if(event->lostFocus()){
        qDebug() << "lost focus" << i++;
        //da ignorise input i prosledjuje onome sto je iza
        this->parentWidget()->setWindowFlags(
                    this->parentWidget()->windowFlags() | Qt::WindowTransparentForInput);
        this->parentWidget()->show();
        //this->show();

    }
}



/**TMP test****/
QPixmap * testScreenPtr = NULL;
void Painter::snapshot(){
    static QPixmap testScreen;
    QScreen *screen = QGuiApplication::primaryScreen();

    testScreen = screen->grabWindow(0)/*.scaledToHeight(720,Qt::FastTransformation);*/;
    //testScreenPtr = &testScreen;
    QImage img = testScreen.toImage().convertToFormat(QImage::Format_Indexed8, Qt::AutoColor);
    testScreen.fromImage(img, Qt::AutoColor);
    testScreenPtr = &testScreen;
    show();
    qDebug() << img.byteCount() << " " << img.format();/*testScreen.toImage().height() << " " << testScreen.toImage().width();*/
}

/*************/
void Painter::keyPressEvent(QKeyEvent * event)
{
    //qDebug() << "key = " << event->key();
    if(isMousePressed)
        return;

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
        Sender::getInstance().send(Sender::Tag::undo, id());
        update();
    }
        break;
    case Qt::Key_T:
        alwaysOnTop = !alwaysOnTop;
        stayOnTop(alwaysOnTop);
        break;
    case Qt::Key_S:
        hide();
        QTimer::singleShot(500, this, SLOT(snapshot()));
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
    case Qt::Key_4:
        nowDrawing = Shape::Type::circle;
        myLines.changeLastType(nowDrawing);
        break;
    case Qt::Key_5:
        nowDrawing = Shape::Type::square;
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

        Sender::getInstance().send(newPoint, id());
        myLines.addPoint(newPoint, nowDrawing);
        update();
    }
}

void Painter::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        isMousePressed = false;
        myLines.newLine(myLines.getLines().last().pen, brushSize, nowDrawing);
        Sender::getInstance().send(Sender::Tag::endline, id());
    }
}

void Painter::mousePressEvent(QMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        isMousePressed = true;
        beginNewDrawable(event->pos(), id());
        c.hide();
        update();
        break;
    case Qt::RightButton:
        c.cshow(event->pos(),width(),height(),myLines.getLines().last().pen.color());
        break;
    case Qt::MiddleButton:
        moveWidgetCenter(event->globalPos());
        break;
    default:
        break;
    }
}

void Painter::beginNewDrawable(const QPoint & pos, QString id)
{
    //send pen info to others
    //qDebug() << event->pos();
    QPoint p(pos);
    myLines.addPoint(p, nowDrawing);
    Sender::getInstance().send(nowDrawing, id);
    Sender::getInstance().send(myLines.getLines().last().pen, id);
    Sender::getInstance().send(pos, id);
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

    QHash<QString, Lines>::const_iterator i = otherLines.constBegin();
    while (i != otherLines.constEnd()) {
        i.value().drawAll(painter);
        ++i;
    }

//    QHashIterator<QString, int> i(otherLines);
//    while (i.hasNext()) {
//        i.next();
//        i.value().drawAll(painter);
//    }
}

void Painter::stringToPoly(QString str)
{
    QStringList listStr = str.split(' ', QString::SkipEmptyParts);
    //maybe 0, not 1? if somethings goes wrong

    if (listStr.size() <= 2 || listStr.size() % 3){
        return;
    }

    if (str.length() > 1000 ){
        qDebug() << "primih sliku";
    }
    //parse incoming string according to first tag
    for (int i = 0; i < listStr.size(); i+=3){
        //qDebug() << "x je " << listStr[i] << ", y je " << listStr[i+1];
        QPoint newPoint(listStr[i].toInt(), listStr[i+1].toInt());
        qDebug() << newPoint.x() << " newpoint.x";
        switch (newPoint.x()) {
        case Sender::Tag::color:
            otherLines[listStr[i+2]].setPenColor(listStr[i+1].toInt(Q_NULLPTR, 16));
            break;
        case Sender::Tag::width:
            otherLines[listStr[i+2]].setPenWidth(listStr[i+1].toInt());
            break;
        case Sender::Tag::shape:
            otherDrawing = static_cast<Shape::Type>(listStr[i+1].toInt());
            otherLines[listStr[i+2]].changeLastType(otherDrawing);
            break;
        case Sender::Tag::undo:
            otherLines[listStr[i+2]].undo();
            break;
        case Sender::Tag::endline:
            if (newPoint.y() == Sender::Tag::endline) {
                otherLines[listStr[i+2]].newLine(otherDrawing);
                break;
            }
        case Sender::Tag::id:{
            QStringList ids = listStr[i+1].split('.', QString::SkipEmptyParts);
            if(id() == ""){
                setId(ids[ids.size()-1]);
                auto i = ids.constBegin();
                for(;i!=ids.constEnd()-1;i++)
                    otherLines.insert(*i, Lines());
            } else {
                otherLines.insert(ids[ids.size()-1], Lines());
            }
            //otherLines.insert(listStr[i+1], Lines());
            qDebug() << listStr[i+1] << "<--------------";
            break;}
        default:
            otherLines[listStr[i+2]].addPoint(newPoint, otherDrawing);
            break;
        }
    }
    update();
}

void Painter::setId(QString id){
    _id = id;
}

QString Painter::id(){
    return _id;
}

void Painter::clickedButton()
{
    QString name = ((QPushButton*)sender())->objectName();

    // HACK (unicode)
    char index = name[name.size()-1].unicode();
    switch(index){
        case '1':
            nowDrawing = Shape::Type::line;
            myLines.changeLastType(nowDrawing);
            break;
        case '2':
            nowDrawing = Shape::Type::ellipse;
            myLines.changeLastType(nowDrawing);
            break;
        case '3':
            nowDrawing = Shape::Type::rectangle;
            myLines.changeLastType(nowDrawing);
            break;
        case '4':
            nowDrawing = Shape::Type::circle;
            myLines.changeLastType(nowDrawing);
            break;
        case '5':
            nowDrawing = Shape::Type::square;
            myLines.changeLastType(nowDrawing);
            break;
        case '6':
            // TODO fix position, and change dialog in function
            selectColor(QPoint(100,100));
            break;
    }
    c.hide();
}

Painter::~Painter()
{
    delete ui;
}
