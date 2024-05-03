#include "aides.h"
#include "mainwindow.h"
#include "connection.h"
#include "ui_mainwindow.h"
#include <QtCharts/QChartView>
#include "QDebug"
#include <QApplication>
#include <QUrl>
#include <QDesktopServices>

Aides::Aides(int ID_aide, int ID_res, QString description_aide, QString date_aide, QString type_aide, QString donneur)
{
    this->ID_aide = ID_aide;
    this->ID_res = ID_res;
    this->description_aide = description_aide;
    this->date_aide = date_aide;
    this->type_aide = type_aide;
    this->donneur = donneur;
}

bool Aides::ajouter()
{

    QSqlQuery query;

    query.prepare("INSERT INTO AIDE (ID_RES, DESCRIPTION_AIDE,DATE_AIDE,TYPE_AIDE, DONNEUR)"
                  "VALUES(:ID_RES,:DESCRIPTION_AIDE,:DATE_AIDE,:TYPE_AIDE,:DONNEUR)");

    query.bindValue(":ID_RES", ID_res);
    query.bindValue(":DESCRIPTION_AIDE", description_aide);
    query.bindValue(":DATE_AIDE", date_aide);
    query.bindValue(":TYPE_AIDE", type_aide);
    query.bindValue(":DONNEUR", donneur);
    return query.exec();
}

bool Aides::supprimer(int i)
{

    QSqlQuery query;

    query.prepare("DELETE from AIDE where ID_aide=:id");
    query.bindValue(":id", i);
    return query.exec();
}

QSqlQueryModel *Aides ::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM AIDE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID "));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Restaurant"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Donor"));

    return model;
}

bool Aides::modifier(int ID)
{
    QSqlQuery query;
    query.prepare("UPDATE AIDE SET DESCRIPTION_AIDE=:description,DATE_AIDE=:date,TYPE_AIDE=:type,DONNEUR=:donneur where ID_AIDE=:id");
    query.bindValue(":id", ID);
    query.bindValue(":description", description_aide);
    query.bindValue(":date", date_aide);
    query.bindValue(":donneur", donneur);
    query.bindValue(":type", type_aide);
    return query.exec();
}

QChartView *Aides::stat_type()
{
    int aliment = 0;
    int vetement = 0;
    int argent = 0;

    QSqlQuery query;
    query.prepare("select * from AIDE where TYPE_AIDE='aliment'");
    query.exec();
    while (query.next())
        aliment++;

    query.prepare("select * from AIDE where TYPE_AIDE='vetements'");
    query.exec();
    while (query.next())
        vetement++;

    query.prepare("select * from AIDE where TYPE_AIDE='argent'");
    query.exec();
    while (query.next())
        argent++;

    qDebug() << aliment << vetement << argent;

    QPieSeries *series = new QPieSeries();
    series->append("aliment", aliment);
    series->append("vetement", vetement);
    series->append("argent", argent);

    QPieSlice *slice = series->slices().at(0);
    slice->setExploded(true);
    slice->setColor("#ffc0cb");
    QPieSlice *slice2 = series->slices().at(1);
    slice2->setColor("#00FFFF");

    QPieSlice *slice3 = series->slices().at(2);
    slice3->setColor("#ffd700");

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("type statistics");

    series->setLabelsVisible();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->chart()->setAnimationOptions(QChart::AllAnimations);
    chartView->chart()->legend()->hide();
    return chartView;
}

QSqlQueryModel *Aides::chercher(QString critere)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM AIDE WHERE TYPE_AIDE LIKE '" + critere + "%'");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID "));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Restaurant"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Donor"));
    return model;
}

void generateQRCode(QString code)
{
    QString url = QString("https://chart.googleapis.com/chart?cht=qr&chs=300x300&chl=%1").arg(code);

    QDesktopServices::openUrl(QUrl(url));
}

void Aides::notification(Ui::MainWindow *ui)
{

    QLabel *notif = ui->stackedWidget_2->findChild<QLabel *>("notif_money");
    QPushButton *money_btn = ui->stackedWidget_2->findChild<QPushButton *>("money_btn");
    QLabel *notif1 = ui->stackedWidget_2->findChild<QLabel *>("notif_dress");
    QPushButton *vete_btn = ui->stackedWidget_2->findChild<QPushButton *>("dress_btn");
    QLabel *notif2 = ui->stackedWidget_2->findChild<QLabel *>("notif_food");
    QPushButton *alim_btn = ui->stackedWidget_2->findChild<QPushButton *>("food_btn");

    alim_btn->hide();
    vete_btn->hide();
    money_btn->hide();

    QSqlQuery query("SELECT COUNT(*) FROM AIDE where TYPE_AIDE = 'argent'");

    while (query.next())
    {
        int count = query.value(0).toInt();
        if (count < 2)
        {
            notif->show();
            money_btn->show();
        }

        QSqlQuery query("SELECT COUNT(*) FROM AIDE where TYPE_AIDE = 'vetements'");

        while (query.next())
        {
            int count = query.value(0).toInt();
            if (count < 2)
            {
                notif1->show();
                vete_btn->show();
            }

            QSqlQuery query("SELECT COUNT(*) FROM AIDE where TYPE_AIDE = 'aliment'");

            while (query.next())
            {
                int count = query.value(0).toInt();
                if (count < 2)
                    notif2->show();
                alim_btn->show();
                        }
        }
    }
}

QSqlQueryModel *Aides::trierASC()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM AIDE ORDER BY ID_AIDE ASC ");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID "));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Restaurant"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Donor"));
    return model;
}

QSqlQueryModel *Aides::trierDESC()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM AIDE ORDER BY ID_AIDE DESC ");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID "));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Restaurant"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Donor"));
    return model;
}

QSqlQueryModel *Aides::recherche(QString titre)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("select * from AIDE WHERE LOWER(DONNEUR) LIKE LOWER('%" + titre + "%')");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID "));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Restaurant"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Donor"));
    return model;
}
