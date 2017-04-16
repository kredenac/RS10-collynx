#ifndef SENDER_H
#define SENDER_H
#include "painter.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QString>

//singleton
class Sender : public QObject
{
    Q_OBJECT
public:
    bool send(QString toSend);
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
