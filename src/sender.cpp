#include "sender.h"
bool Sender::connected = false;

bool Sender::makeConnection()
{
    //qDebug() << "Connecting\r\n";
    mySocket = new QTcpSocket();
    mySocket->connectToHost("localhost", 1234);
    //qDebug() << "Tried to connect\r\n";
    //so client can also read
    connect(mySocket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);

    return true;
}

void Sender::send(QPen &pen)
{
    QByteArray ba;
    //pen.color().HexArgb();
    QColor col(pen.color());
    //pen.setColor(col);
    //col.name();
    QString toSend ="-2 " + col.name().remove(0,1) + " ";
    //ba = toSend.toLatin1();
    send(toSend);
    //-3 je za width pena
    toSend = "-3 " + QString::number(pen.width()) + " ";
    send(toSend);
    //qDebug()<< ba.data();
    //-2 znaci da se salje boja
    //QString back = QString::fromLocal8Bit(ba) ;
    //qDebug() << back.toInt(Q_NULLPTR, 16);
}

bool Sender::send(QPoint toSend)
{
    QString coordsToSend = QString::number(toSend.x()) + " " + QString::number(toSend.y()) + " ";
    send(coordsToSend);
    return true;
}

bool Sender::send(QString toSend)
{
    //TODO optimize with ints, not strings
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
