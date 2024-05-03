#ifndef MESSENGERLANCLIENT_H
#define MESSENGERLANCLIENT_H

#include <QWidget>

namespace Ui
{
    class MessengerLANClient;
}

class QTcpSocket;
namespace chatbox
{
    class MessengerLANClient : public QWidget
    {
        Q_OBJECT

    public:
        explicit MessengerLANClient(QWidget *parent = nullptr);
        ~MessengerLANClient();

    private slots:
        void on_send_clicked();

        void on_bind_clicked();

    private:
        Ui::MessengerLANClient *ui;
        QTcpSocket *mSocket;
    };
}
#endif // MESSENGERLANCLIENT_H
