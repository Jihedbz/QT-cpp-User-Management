#include "mainwindow.h"
#include "connection.h"
#include "messengerlanclient.h"
#include "messengerserver.h"
#include <QApplication>
#include <QMessageBox>
using namespace chatbox;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;
    bool test = c.createConnection();
    MainWindow w;
    //MessengerServer server;
    //MessengerLANClient m;
    // If the connection to the Database is successful
    if (test)
    {
        w.show();
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Database is not open"), QObject::tr("Connetion failed.\n"
                                                                                        "Click cancel to exit."),
                              QMessageBox::Cancel);
    }

    /*if (!server.startServer(3333))
    {
        qDebug() << "Error:" << server.errorString();
        return 1;
    }
    qDebug() << "Server Started....";
    m.show();*/
    return a.exec();
}
