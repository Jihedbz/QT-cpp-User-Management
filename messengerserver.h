#ifndef MESSENGERSERVER_H
#define MESSENGERSERVER_H

#include <QTcpServer>

namespace chatbox
{

    class MessengerSocket;

    class MessengerServer : public QTcpServer
    {
    public:
        MessengerServer(QObject *parent = nullptr);
        bool startServer(quint16 port);

    private:
        QList<MessengerSocket *> mSockets;

    protected:
        void incomingConnection(qintptr handle);
    };
}
#endif // CHATSERVER_H
