#include "painter.h"
#include "sender.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Painter w;
    //w.show();
    Sender::getInstance().setPainterFriend(&w);
    QMainWindow *window = new QMainWindow();
    window->setAttribute( Qt::WA_TranslucentBackground );
    window->setWindowTitle(QString::fromUtf8("ColLynx"));
    window->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    //window->setWindowFlags();
    window->setCentralWidget(&w);
    window->show();
    //window->move(QPoint(0,0));
    return a.exec();
}
