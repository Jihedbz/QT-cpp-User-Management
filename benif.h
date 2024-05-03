#ifndef BENIF_H
#define BENIF_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>
#include <QPrinter>
#include <QPainter>
#include <QTableView>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QGridLayout>
#include <QtCharts>
#include <QChartView>

class Benif
{
private:
    int ID_benif;
    QString nom_ass;
    QString adresse_ass;
    QString nom_responsable;
    QString telephone_ass;
    QString email_ass;
    QString age;

public:
    // Constructors
    Benif(){};
    Benif(int ID_benif,
          QString nom_ass,
          QString adresse_ass,
          QString nom_responsable,
          QString telephone_ass,
          QString email_ass,
          QString age);

    // Getters
    int getId() { return ID_benif; }
    QString getNom() { return nom_ass; }
    QString getAdresse() { return adresse_ass; }
    QString getResponsable() { return nom_responsable; }
    QString getTelephone() { return telephone_ass; }
    QString getEmail() { return email_ass; }
    QString getAge() { return age; }

    // Setters
    void setID(int id) { ID_benif = id; }
    void setNom(QString n) { nom_ass = n; }
    void setAdresse(QString a) { adresse_ass = a; }
    void setResponsable(QString r) { nom_responsable = r; }
    void setTelephone(QString t) { telephone_ass = t; }
    void setEmail(QString e) { email_ass = e; }
    void setAge(QString ag) { age = ag; }
    // Functions

    bool ajouter_benif();
    QSqlQueryModel *afficher_benif();
    bool supprimer_benif(int i);
    bool modifier_benif(int i);
    void exportToPDF(QTableView *tableView);
    QSqlQueryModel *trierC();
    QSqlQueryModel *trierD();
    QSqlQueryModel *recherche_bn(QString titre);
    QChartView *stat_age();
    void generer();

};

#endif // EMPLOYEE_H
