#include "arduino.h"
#include <QString>
#include "connection.h"
#include <QDate>
#include <QDateTime>

Arduino::Arduino()
{
    data="";
    arduino_port_name="";
    arduino_is_available=false;
    serial=new QSerialPort;
}

QString Arduino::getarduino_port_name()
{
    return arduino_port_name;
}

QSerialPort *Arduino::getserial()
{
   return serial;
}
int Arduino::connect_arduino()
{   // recherche du port sur lequel la carte arduino identifée par  arduino_uno_vendor_id
    // est connectée
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()){
           if(serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()){
               if(serial_port_info.vendorIdentifier() == arduino_uno_vendor_id && serial_port_info.productIdentifier()
                       == arduino_uno_producy_id) {
                   arduino_is_available = true;
                   arduino_port_name=serial_port_info.portName();
               } } }
        qDebug() << "arduino_port_name is :" << arduino_port_name;
        if(arduino_is_available){ // configuration de la communication ( débit...)
            serial->setPortName(arduino_port_name);
            if(serial->open(QSerialPort::ReadWrite)){
                serial->setBaudRate(QSerialPort::Baud9600); // débit : 9600 bits/s
                serial->setDataBits(QSerialPort::Data8); //Longueur des données : 8 bits,
                serial->setParity(QSerialPort::NoParity); //1 bit de parité optionnel
                serial->setStopBits(QSerialPort::OneStop); //Nombre de bits de stop : 1
                serial->setFlowControl(QSerialPort::NoFlowControl);
                return 0;
            }
            return 1;
        }
        return -1;
}

int Arduino::close_arduino()

{

    if(serial->isOpen()){
            serial->close();
            return 0;
        }
    return 1;


}



QByteArray Arduino::read_from_arduino()
{
    QByteArray data;
    while (serial->waitForReadyRead()) {
        data.append(serial->read(1));
        if (data.endsWith('\n')) {
            return data.trimmed();
        }
    }
    return QByteArray();
}


int Arduino::write_to_arduino( QByteArray d)
{

    if(serial->isWritable()){
        serial->write(d);  // envoyer des donnés vers Arduino
    }else{
        qDebug() << "Couldn't write to serial!";
    }


}


QSerialPort serial;

void Arduino::add_temp(QString temp)
{
    QSqlQuery query;

    QString SqlQuery = "insert into TEMPERATURE VALUES (:temp,:time)";
    query.prepare(SqlQuery);

    query.bindValue(":temp",temp);



    // Get the current system date and time
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // Print the current date and time in the format "yyyy-MM-dd hh:mm:ss"
    QString dateTimeString = currentDateTime.toString(Qt::ISODate);
    qDebug() << "Current date and time:" << dateTimeString;




    query.bindValue(":time",dateTimeString);

    query.exec();
    qDebug() << "Element added";
}


QSqlQueryModel *Arduino :: afficher_temperature()
{
    QSqlQueryModel * model=new QSqlQueryModel();

    model->setQuery("SELECT * FROM TEMPERATURE");
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("TEMP "));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("TIME"));

    return model;
}
