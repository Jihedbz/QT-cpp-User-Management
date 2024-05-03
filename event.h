#ifndef EVENT_H
#define EVENT_H
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QGridLayout>
#include <QtCharts>
#include <QChartView>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QTableView>
#include <QFileDialog>
#include <QDebug>




class Event
{
private:
    int id_event;
    QString nom_event;
    QString date_event;
    QString horaire_event;
    QString type_event;
    int participant_number;
    int entry_price;
   // int id_em;

public:

    //Constructors
    Event(){};
    Event(int id_event, QString nom_event,  QString date_event, QString type_event, QString horaire_event, int participant_number,int entry_price); //int id_em

    //Getters
    int getid_event(){return id_event;}
    QString getnom_event(){return nom_event;}
    QString getdate_event(){return date_event;}
    QString gethoraire_event(){return horaire_event;}
    QString gettype_event(){return type_event;}
    int getparticipant_number(){return participant_number;}
    int getentry_price(){return entry_price;}
    //int getid_em(){return id_em;}



    //Setters
    void setID(int id){id_event=id;}
    void setnom_event(QString h){nom_event=h;}
    void setdate_event(QString pi){date_event=pi;}
    void sethoraire_event(QString he){ horaire_event=he;}
    void settype_event(QString te){ type_event=te;}
    void setparticipant_number(int pn ){participant_number=pn;}
    void setentry_price(int px){entry_price=px;}
  //void setid_em(int em){id_em=em;}

    //Functions

    bool ajouter_event();
    QSqlQueryModel *afficher_event();
    bool supprimer_event(int i);
    bool modifier_event(int i);

    QSqlQueryModel *trier_event(QString critere);
    QSqlQueryModel *chercher_event(QString critere);
    void exportToPDF();
  //  QChartView *stat_gender();
    //QChartView *stat_age();
    QSqlQueryModel *recherche_event(QString titre);
    QChartView *stat_type();


};

#endif // EMPLOYEE_H
