#include "connection.h"

Connection::Connection() {}

bool Connection::createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    bool test = false;
    db.setDatabaseName("projet");
    db.setUserName("bouazizi");
    db.setPassword("jihed");

    if (db.open())
        test = true;

    return test;
}

void Connection::closeConnection()
{
    db.close();
}
