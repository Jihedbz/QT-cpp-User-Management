#include "messengersocket.h"

namespace chatbox
{

    MessengerSocket::MessengerSocket(qintptr handle, QObject *parent) : QTcpSocket(parent)
    {
        setSocketDescriptor(handle);
        connect(this, &MessengerSocket::readyRead, [&]()
                { emit DuReadyRead(this); });
        connect(this, &MessengerSocket::stateChanged, [&](int S)
                { emit DuStateChanged(this, S); });
    }
}
