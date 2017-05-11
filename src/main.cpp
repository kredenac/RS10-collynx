#include "painter.h"
#include "chooser.h"
#include "sender.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //w.show();

    QMainWindow *window = new QMainWindow();



    window->setAttribute( Qt::WA_TranslucentBackground );
    window->setWindowTitle(QString::fromUtf8("ColLynx"));
    window->setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);
    //window->setProperty("windowOpacity", 0.5);

    Painter w(window);
    Sender::getInstance().setPainterFriend(&w);

    //window->setWindowFlags();
    window->setCentralWidget(&w);
//    window->setCentralWidget(&c);
    window->show();

    //window->setStyleSheet("background-image : url(C:/Users/Dzoni/Documents/QtProjects/ColLynx/img_fjords.jpg);");
    //window->setStyleSheet("backgroud-style : cover");
//    window->setStyleSheet("background: yellow");
    //window->move(QPoint(0,0));
    return a.exec();
}
