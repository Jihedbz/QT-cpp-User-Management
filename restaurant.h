#ifndef RESTAURANT_H
#define RESTAURANT_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QGridLayout>
#include <QtCharts>
#include <QChartView>

class Restaurant
{
private:
    int id_re;
    QString Nom;
    QString Website;
    QString Email;
    QString Adr;
    QString Tel;

public:
    // Constructors
    Restaurant(){};
    Restaurant(int id, QString Nom, QString Website, QString Email, QString Adr, QString Tel);

    // Getters
    int getId() { return id_re; }
    QString getNom() { return Nom; }
    QString getWebsite() { return Website; }
    QString getEmail() { return Email; }
    QString getAdr() { return Adr; }
    QString getTelephone() { return Tel; }

    // Setters
    void setID(int id) { id_re = id; }
    void setNom(QString n) { Nom = n; }
    void setWebsite(QString w) { Website = w; }
    void setEmail(QString mail) { Email = mail; }
    void setAdr(QString adr) { Adr = adr; }
    void setTel(QString t) { Tel = t; }

    // Functions

    bool ajouter_re();
    QSqlQueryModel *afficher_re();
    bool supprimer_re(int i);
    bool modifier_re(int i);
    QSqlQueryModel *trier_re(QString critere);
    QSqlQueryModel *recherche_rest(QString titre);
    QSqlQueryModel *trierC();
    QSqlQueryModel *trierD();
    QChartView *stat_rest();
    QAbstractItemModel *autofillTable(Restaurant r);
    QAbstractItemModel *ClearTable();
};

#endif // RESTAURANT_H
