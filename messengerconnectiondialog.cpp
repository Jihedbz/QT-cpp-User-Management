#include "messengerconnectiondialog.h"
#include "ui_messengerconnectiondialog.h"
namespace chatbox
{
    MessengerConnectionDialog::MessengerConnectionDialog(QWidget *parent) : QDialog(parent),
                                                                            ui(new Ui::MessengerConnectionDialog)
    {
        ui->setupUi(this);
    }

    MessengerConnectionDialog::~MessengerConnectionDialog()
    {
        delete ui;
    }

    void MessengerConnectionDialog::on_ok_clicked()
    {
        mHostname = ui->hostname->text();
        mPort = ui->port->value();
        accept();
    }

    void MessengerConnectionDialog::on_cancel_clicked()
    {
        reject();
    }
}
