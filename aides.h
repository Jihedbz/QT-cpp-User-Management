#ifndef AIDES_H
#define AIDES_H
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QGridLayout>
#include <QtCharts>
#include <QChartView>
#include <QSystemTrayIcon>
#include <QMainWindow>
#include "ui_mainwindow.h"



class Aides
{
    QString description_aide,date_aide,donneur,type_aide;
    int ID_aide,ID_res;
public:
    Aides(){}
    Aides (int,int,QString,QString,QString,QString);
    //getters
    QString getdescription(){return description_aide;}
    QString getdate (){return date_aide;}
    QString getdonneur (){return donneur;}
    QString gettype (){return type_aide;}


    int getID(){return ID_aide;}
    int getIDres(){return ID_res;}

    //setters
    void setdescription(QString d){description_aide=d;}
    void setdate (QString da){date_aide=da;}
    void setdonneur(QString dn){donneur=dn;}
    void settype(QString t){type_aide=t;}

    void setID (int id){this ->ID_aide=id;}
    void setID_res (int id_r){this ->ID_res=id_r;}
    void generateQRCode(QString code) ;

    void notification(Ui::MainWindow *ui);
    QSqlQueryModel *recherche(QString titre);


//    void notification(QString noti);



    // fonctionnalités de base
    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int);
    bool modifier(int i);

    QSqlQueryModel *trierASC();
    QSqlQueryModel *trierDESC();


    QSqlQueryModel *chercher(QString critere);
    QChartView *stat_type();

};

#endif // AIDES_H
