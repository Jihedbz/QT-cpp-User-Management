#include "messengerserver.h"
#include <QDebug>
#include "messengersocket.h"
#include <QTextStream>

namespace chatbox
{

    MessengerServer::MessengerServer(QObject *parent)
        : QTcpServer(parent)
    {
    }
    bool MessengerServer::startServer(quint16 port)
    {
        return listen(QHostAddress::Any, port);
    }
    void MessengerServer::incomingConnection(qintptr handle)
    {
        qDebug() << "User connected with handle:" << handle;
        auto socket = new MessengerSocket(handle, this);
        mSockets << socket;

        for (auto i : mSockets)
        {
            QTextStream T(i);
            T << "Server: Connected :" << handle;
            i->flush();
        }

        connect(socket, &MessengerSocket::DuReadyRead, [&](MessengerSocket *S)
                {qDebug()<<"DuReadyRead";
        QTextStream T(S);
        auto text=T.readAll();
        for (auto i:mSockets)
        {
            QTextStream K(i);
            K<<text ;
            i->flush();
        } });
        connect(socket, &MessengerSocket::DuStateChanged, [&](MessengerSocket *S, int ST)
                {
                    qDebug() << "DuStateChanged with handle " << S->socketDescriptor();
                    if (ST == QTcpSocket::UnconnectedState)
                    {
                        qDebug() << "Unconnected State with handle :" << S->socketDescriptor();
                        mSockets.removeOne(S);

                        for (auto i : mSockets)
                        {
                            QTextStream K(i);
                            K << "Server:The client" << S->socketDescriptor() << "disconnected..";
                            i->flush();
                        }
                    }
                });
    }
}
