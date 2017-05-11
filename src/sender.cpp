#include "sender.h"
bool Sender::connected = false;

bool Sender::makeConnection()
{
    //qDebug() << "Connecting\r\n";
    mySocket = new QTcpSocket();
    mySocket->connectToHost(//"192.168.1.10",
                            "localhost",
                            1234);
    //so client can also read
    connect(mySocket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    return true;
}
void Sender::send(Sender::Tag t, QString id)
{
    QString coordsToSend = QString::number(t) + " " + QString::number(t) + " " + id + " ";
    send(coordsToSend);
}

void Sender::send(QPen &pen, QString id)
{
    //qDebug() << mySocket->localAddress().toString() << mySocket->localPort();
    //QByteArray ba;
    QColor col(pen.color());

    QString toSend = QString::number(Tag::color) + " " + col.name().remove(0,1) + " " + id + " ";
    send(toSend);
    toSend = QString::number(Tag::width) + " " + QString::number(pen.width()) + " " + id + " ";
    send(toSend);
}

bool Sender::send(QPoint toSend, QString id)
{
    QString coordsToSend = QString::number(toSend.x()) + " " + QString::number(toSend.y()) + " " + id + " ";
    send(coordsToSend);
    return true;
}
void Sender::send(Shape::Type shapeType, QString id)
{
     QString toSend = QString::number(Tag::shape) + " " + QString::number(shapeType) + " " + id + " ";
     send(toSend);
}

bool Sender::send(QString toSend)
{
    QByteArray bytesToSend = toSend.toLatin1();
    mySocket->write(bytesToSend.data());
    return true;
}

//read bytes, then make them into a poly
void Sender::readyRead()
{
    QByteArray readBytes = mySocket->readAll();
    qDebug() << "readyRead" << readBytes;

    QString dataStr = QString::fromLocal8Bit(readBytes);
    getPainterFriend()->stringToPoly(dataStr);
}

Sender & Sender::getInstance()
{
    static Sender instance; // Guaranteed to be destroyed.
                           // Instantiated on first use.
    if (!connected){
        connected = true;
        instance.makeConnection();
    }
    return instance;
}

void Sender::setPainterFriend(Painter * p)
{
    painterFriend = p;
}

Painter * Sender::getPainterFriend()
{
    return painterFriend;
}
