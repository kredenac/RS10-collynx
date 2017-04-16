#include "sender.h"
bool Sender::connected = false;

bool Sender::makeConnection()
{
    qDebug() << "Connecting\r\n";
    mySocket = new QTcpSocket();
    mySocket->connectToHost("localhost", 1234);
    //qDebug() << "Tried to connect\r\n";
    //so client can also read
    connect(mySocket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    return true;
}

bool Sender::send(QString toSend)
{
    QByteArray bytesToSend = (toSend /*+ "\r\n"*/).toLatin1();
    mySocket->write(bytesToSend.data());

    //mySocket->write("ja sam klijent collynx");
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
