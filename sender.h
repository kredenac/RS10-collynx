#ifndef SENDER_H
#define SENDER_H
#include "painter.h"
#include "lines.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QString>
#include <QPen>
//singleton
class Sender : public QObject
{
    Q_OBJECT
public:
    enum Tag{
        endline = -6969, color = -6966, width = -6696, shape = -6999, undo = -9699
    };
    bool send(QString toSend);
    bool send(QPoint tosend);
    void send(QPen &pen);
    void send(Tag t);
    void send(Shape::Type shapeType);
    bool makeConnection();
    static Sender & getInstance();
    Sender(Sender const&)         = delete;
    void operator=(Sender const&) = delete;
    void setPainterFriend(Painter * p);
    Painter * getPainterFriend();
private:
    Sender(){}
    static bool connected;
    QTcpSocket * mySocket;
    Painter * painterFriend;

protected:

public slots:
    void readyRead();
};

#endif // SENDER_H
