#include "messengerlanclient.h"
#include "ui_messengerlanclient.h"
#include <QTcpSocket>
#include "messengerconnectiondialog.h"
#include <QTextStream>
namespace chatbox
{
    MessengerLANClient::MessengerLANClient(QWidget *parent) : QWidget(parent),
                                                              ui(new Ui::MessengerLANClient)
    {
        ui->setupUi(this);
        this->setWindowTitle("Messenger");
        this->setWindowIcon(QIcon(":/Icons/Icons/Images/chat.ico"));
        mSocket = new QTcpSocket(this);
        connect(mSocket, &QTcpSocket::readyRead, [&]()
                {
       QTextStream T(mSocket);
       auto text = T.readAll();
       ui->textEdit->append(text); });
    }

    MessengerLANClient::~MessengerLANClient()
    {
        delete ui;
    }

    void MessengerLANClient::on_send_clicked()
    {
        QTextStream T(mSocket);
        T << ui->nickname->text() << ":" << ui->message->text();
        mSocket->flush();
        ui->message->clear();
    }

    void MessengerLANClient::on_bind_clicked()
    {
        MessengerConnectionDialog D(this);
        if (D.exec() == QDialog::Rejected)
        {
            return;
        }
        mSocket->connectToHost(D.hostname(), D.port());
    }
}
