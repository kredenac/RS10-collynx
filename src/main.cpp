#include "painter.h"
#include "sender.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Painter w;
    w.show();
    Sender::getInstance().setPainterFriend(&w);
    Sender::getInstance().send("ColLynx");
    return a.exec();
}
