#include "employee.h"
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

Employee::Employee(int id, QString Nom, QString Prenom, QString Email, QString Sexe, QString Adresse, QString Fonction, QString Date, QString Telephone, int Salaire)
{
    this->id_em = id;
    this->Nom = Nom;
    this->Prenom = Prenom;
    this->Email = Email;
    this->Sexe = Sexe;
    this->Adresse = Adresse;
    this->Fonction = Fonction;
    this->Date = Date;
    this->Telephone = Telephone;
    this->Salaire = Salaire;
}

bool Employee::ajouter_em()
{
    QSqlQuery query;
    query.prepare("insert into EMPLOYEE (NOM, PRENOM, EMAIL, SEXE, ADRESSE, FONCTION, DATE_DE_NAISSANCE, TELEPHONE, SALAIRE)"
                  "values (:Nom,:Prenom,:Email,:Sexe,:Adresse,:Fonction,:Date,:Telephone,:Salaire)");
    query.bindValue(":Nom", Nom);
    query.bindValue(":Prenom", Prenom);
    query.bindValue(":Email", Email);
    query.bindValue(":Sexe", Sexe);
    query.bindValue(":Adresse", Adresse);
    query.bindValue(":Fonction", Fonction);
    query.bindValue(":Date", Date);
    query.bindValue(":Telephone", Telephone);
    query.bindValue(":Salaire", Salaire);
    qDebug() << "Added";
    return query.exec();
}

QSqlQueryModel *Employee::afficher_em()
{

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM EMPLOYEE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("First Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Last Name"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Gender"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Function"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date of Birth"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Phone"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Salary"));

    return model;
}

bool Employee::supprimer_em(int i)
{

    QSqlQuery query;
    query.prepare("DELETE from EMPLOYEE where ID_EM=:id");
    query.bindValue(":id", i);
    return query.exec();
}

bool Employee::modifier_em(int i)
{

    QSqlQuery query;
    query.prepare("update EMPLOYEE set NOM=:nom,PRENOM=:prenom,EMAIL=:email,SEXE=:sexe,ADRESSE=:adresse,FONCTION=:fonction,DATE_DE_NAISSANCE=:date,TELEPHONE=:t,SALAIRE=:s where ID_EM=:id");
    query.bindValue(":id", i);
    query.bindValue(":nom", Nom);
    query.bindValue(":prenom", Prenom);
    query.bindValue(":email", Email);
    query.bindValue(":sexe", Sexe);
    query.bindValue(":adresse", Adresse);
    query.bindValue(":fonction", Fonction);
    query.bindValue(":date", Date);
    query.bindValue(":t", Telephone);
    query.bindValue(":s", Salaire);
    qDebug() << "Updated";
    return query.exec();
}

QSqlQueryModel *Employee::trier_em(QString critere)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYEE WHERE LOWER(NOM) LIKE LOWER('%" + critere + "%')");

    query.exec();
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("First Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Last Name"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Gender"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Function"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date of Birth"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Phone"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Salary"));
    return model;
}

QChartView *Employee::stat_gender()
{
    int male = 0;
    int female = 0;

    QSqlQuery query;
    query.prepare("select * from EMPLOYEE where sexe='Male'");
    query.exec();
    while (query.next())
        male++;

    query.prepare("select * from EMPLOYEE where sexe='Female'");
    query.exec();
    while (query.next())
        female++;

    qDebug() << male << female;

    QPieSeries *series = new QPieSeries();
    series->append("Male", male);
    series->append("Female", female);

    QPieSlice *slice = series->slices().at(0);
    slice->setExploded(true);
    slice->setColor("#f55d11");
    QPieSlice *slice2 = series->slices().at(1);
    slice2->setColor("#ff8b52");

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Gender statistics");

    series->setLabelsVisible();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->chart()->setAnimationOptions(QChart::AllAnimations);
    chartView->chart()->legend()->hide();
    return chartView;
}

QChartView *Employee::stat_age()
{

    QSqlQuery query;
    int g1 = 0;
    int g2 = 0;
    int g3 = 0;
    query.prepare("SELECT DATE_DE_NAISSANCE FROM EMPLOYEE");

    if (query.exec())
    {
        while (query.next())
        {
            QString dates = query.value(0).toString();
            QDate date = QDate::fromString(dates, "ddd MMM d yyyy");
            if (date.isValid())
            {
                int year = date.year();
                int age = 2023 - year;
                if (age >= 20 && age <= 30)
                    g1++;
                else if (age > 30 && age <= 40)
                    g2++;
                else if (age > 40 && age <= 60)
                    g3++;
            }
            else
            {
                // Handle invalid date format
                qDebug() << "Invalid date format: " << dates;
            }
        }
    }
    else
    {
        // Handle query error
        qDebug() << "Query failed ";
    }

    qDebug() << "Number of employees aged 20-30: " << g1;
    qDebug() << "Number of employees aged 31-40: " << g2;
    qDebug() << "Number of employees aged 41-60: " << g3;

    QPieSeries *series = new QPieSeries();
    series->append("20-30 year", g1);
    series->append("30-40 year", g2);
    series->append("40-60 year", g3);

    QPieSlice *slice = series->slices().at(0);
    slice->setExploded(true);
    slice->setColor("#092cdb");
    QPieSlice *slice2 = series->slices().at(1);
    slice2->setColor("#4360f0");
    QPieSlice *slice3 = series->slices().at(2);
    slice3->setColor("#303a6e");

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Age Statistics");
    series->setLabelsVisible();
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->chart()->setAnimationOptions(QChart::AllAnimations);
    chartView->chart()->legend()->hide();
    return chartView;
}

Employee Employee::retrievedata(int id)
{
    QString nom, prenom, tel, addresse, email, fonction, date, sexe;
    int ide, salaire;

    QSqlQuery qry;
    qry.prepare("select * from employee where id_em=:id");
    qry.bindValue(":id", id);
    if (qry.exec())
    {
        while (qry.next())
        {
            // id
            ide = qry.value(0).toInt();
            // nom
            nom = qry.value(1).toString();
            // prenom
            prenom = qry.value(2).toString();
            // email
            email = qry.value(3).toString();
            // date
            sexe = qry.value(4).toString();
            // ad
            addresse = qry.value(5).toString();
            // tel
            fonction = qry.value(6).toString();

            date = qry.value(7).toString();

            tel = qry.value(8).toString();

            salaire = qry.value(9).toInt();
        }
    }

    Employee em(ide, nom, prenom, email, sexe, addresse, fonction, date, tel, salaire);
    return em;
}

QSqlQueryModel *Employee::trierASC()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM EMPLOYEE ORDER BY ID_EM ASC ");

    return model;
}
QSqlQueryModel *Employee::trierDESC()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM EMPLOYEE ORDER BY ID_EM DESC ");

    return model;
}
