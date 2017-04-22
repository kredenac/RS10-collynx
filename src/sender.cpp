#include "sender.h"
bool Sender::connected = false;

bool Sender::makeConnection()
{
    //qDebug() << "Connecting\r\n";
    mySocket = new QTcpSocket();
    mySocket->connectToHost("localhost", 1234);
    //so client can also read
    connect(mySocket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    return true;
}

void Sender::send(QPen &pen)
{
    //QByteArray ba;
    QColor col(pen.color());

    QString toSend = QString::number(Tag::color) + " " + col.name().remove(0,1) + " ";
    send(toSend);
    toSend = QString::number(Tag::width) + " " + QString::number(pen.width()) + " ";
    send(toSend);
}

bool Sender::send(QPoint toSend)
{
    QString coordsToSend = QString::number(toSend.x()) + " " + QString::number(toSend.y()) + " ";
    send(coordsToSend);
    return true;
}
void Sender::send(Shape::Type shapeType)
{
     QString toSend = QString::number(Tag::shape) + " " + QString::number(shapeType) + " ";
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
