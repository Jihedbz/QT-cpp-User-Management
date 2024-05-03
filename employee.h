#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QGridLayout>
#include <QtCharts>
#include <QChartView>

class Employee
{
private:
    int id_em;
    QString Nom;
    QString Prenom;
    QString Email;
    QString Sexe;
    QString Adresse;
    QString Fonction;
    QString Date;
    QString Telephone;
    int Salaire;

public:
    // Constructors
    Employee(){};
    Employee(int id, QString Nom, QString Prenom, QString Email, QString Sexe, QString Adresse, QString Fonction, QString Date, QString Telephone, int Salaire);

    // Getters
    int getId() { return id_em; }
    QString getNom() { return Nom; }
    QString getPrenom() { return Prenom; }
    QString getEmail() { return Email; }
    QString getSexe() { return Sexe; }
    QString getAdresse() { return Adresse; }
    QString getFonction() { return Fonction; }
    QString getDaten() { return Date; }
    QString getTelephone() { return Telephone; }
    int getSalaire() { return Salaire; }

    // Setters
    void setID(int id) { id_em = id; }
    void setNom(QString n) { Nom = n; }
    void setPrenom(QString p) { Prenom = p; }
    void setEmail(QString e) { Email = e; }
    void setSexe(QString s) { Sexe = s; }
    void setAdresse(QString a) { Adresse = a; }
    void setFonction(QString f) { Fonction = f; }
    void setDaten(QString d) { Date = d; }
    void setTelephone(QString t) { Telephone = t; }
    void setSalaire(int s) { Salaire = s; }

    // Functions
    bool ajouter_em();
    QSqlQueryModel *afficher_em();
    bool supprimer_em(int i);
    bool modifier_em(int i);

    QSqlQueryModel *trier_em(QString critere);
    QChartView *stat_gender();
    QChartView *stat_age();
    Employee retrievedata(int id);

    QSqlQueryModel *trierASC();
    QSqlQueryModel *trierDESC();
};

#endif // EMPLOYEE_H
