#include "benif.h"
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPrinter>
#include <QPainter>
#include <QTableView>
#include <QPrintDialog>
#include <sstream>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
#include <QtCharts/QChartView>
#include <QAxObject>
#include <QtNetwork>
#include <QUrlQuery>
#include <QByteArray>

Benif::Benif(int ID_benif,
             QString nom_ass,
             QString adresse_ass,
             QString nom_responsable,
             QString telephone_ass,
             QString email_ass,
             QString age)
{
    this->ID_benif = ID_benif;
    this->nom_ass = nom_ass;
    this->adresse_ass = adresse_ass;
    this->nom_responsable = nom_responsable;
    this->telephone_ass = telephone_ass;
    this->email_ass = email_ass;
    this->age = age;
}

bool Benif::ajouter_benif()
{
    QSqlQuery query;
    query.prepare("insert into BENIFICIAIRE (NOM_ASS, ADRESSE_ASS, NOM_RESPONSABLE, TELEPHONE_ASS, EMAIL_ASS,AGE)"
                  "values (:nom_ass,:adresse_ass,:nom_responsable,:telephone_ass,:email_ass,:age)");
    query.bindValue(":nom_ass", nom_ass);
    query.bindValue(":adresse_ass", adresse_ass);
    query.bindValue(":nom_responsable", nom_responsable);
    query.bindValue(":telephone_ass", telephone_ass);
    query.bindValue(":email_ass", email_ass);
    query.bindValue(":age", age);

    qDebug() << "Added";
    return query.exec();
}

QSqlQueryModel *Benif::afficher_benif()
{

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID_BENIF,NOM_ASS,NOM_RESPONSABLE,AGE,ADRESSE_ASS,EMAIL_ASS,TELEPHONE_ASS FROM BENIFICIAIRE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Last name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("First Name "));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Age"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Adress"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Phone"));
    return model;
}

bool Benif::supprimer_benif(int i)
{

    QSqlQuery query;

    query.prepare("DELETE from BENIFICIAIRE where ID_BENIF=:id");
    query.bindValue(":id", i);
    return query.exec();
}

bool Benif::modifier_benif(int i)
{

    QSqlQuery query;
    query.prepare("update BENIFICIAIRE set NOM_ASS=:nom_ass,ADRESSE_ASS=:adresse_ass,NOM_RESPONSABLE=:nom_responsable,TELEPHONE_ASS=:telephone_ass,EMAIL_ASS=:email_ass,AGE=:age where ID_BENIF=:id");
    query.bindValue(":id", i);
    query.bindValue(":nom_ass", nom_ass);
    query.bindValue(":adresse_ass", adresse_ass);
    query.bindValue(":nom_responsable", nom_responsable);
    query.bindValue(":telephone_ass", telephone_ass);
    query.bindValue(":email_ass", email_ass);
    query.bindValue(":age", age);
    qDebug() << "Updated";
    return query.exec();
}

QSqlQueryModel *Benif::trierC()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID_BENIF,NOM_ASS,NOM_RESPONSABLE,AGE,ADRESSE_ASS,EMAIL_ASS,TELEPHONE_ASS FROM BENIFICIAIRE ORDER BY AGE ASC ");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Last name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("First Name "));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Age"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Adress"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Phone"));
    return model;
}

QSqlQueryModel *Benif::trierD()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID_BENIF,NOM_ASS,NOM_RESPONSABLE,AGE,ADRESSE_ASS,EMAIL_ASS,TELEPHONE_ASS FROM BENIFICIAIRE ORDER BY AGE DESC ");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Last name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("First Name "));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Age"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Adress"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Phone"));
    return model;
}

QSqlQueryModel *Benif::recherche_bn(QString titre)
{
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("SELECT ID_BENIF,NOM_ASS,NOM_RESPONSABLE,AGE,ADRESSE_ASS,EMAIL_ASS,TELEPHONE_ASS FROM BENIFICIAIRE WHERE LOWER(NOM_ASS) LIKE LOWER('%" + titre + "%')");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Last name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("First Name "));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Age"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Adress"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Phone"));
    return model;
}

void exportToPDF(QTableView *tableView)
{

    QPrinter printer(QPrinter::HighResolution);

    printer.setPrinterName("Printer Name");

    QPrintDialog dialog(&printer, nullptr);
    if (dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    QPainter painter(&printer);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    double xscale = printer.pageRect().width() / double(tableView->width());
    painter.scale(xscale, xscale);

    double ypos = (printer.pageRect().height() / xscale - tableView->height()) / 1500;

    painter.translate(0, ypos);

    tableView->render(&painter);

    painter.end();
}

QChartView *Benif::stat_age()
{
    int pub = 0;
    int Sensib = 0;

    QSqlQuery query;
    query.prepare("select * from BENIFICIAIRE where AGE>='30'");
    query.exec();
    while (query.next())
        pub++;

    query.prepare("select * from BENIFICIAIRE where AGE<'30'");
    query.exec();
    while (query.next())
        Sensib++;

    qDebug() << pub << Sensib;

    QPieSeries *series = new QPieSeries();
    series->append("Age >= 30", pub);
    series->append("Age < 30", Sensib);

    int total = pub + Sensib;
    QString labelPublicite = QString("Age over 30 "
                                     "(%1%)")
                                 .arg((double)pub / total * 100, 0, 'f', 1);
    QString labelSensibilisation = QString("Age below 30 "
                                           "(%1%)")
                                       .arg((double)Sensib / total * 100, 0, 'f', 1);
    series->slices().at(1)->setLabel(labelSensibilisation);
    series->slices().at(0)->setLabel(labelPublicite);

    QPieSlice *slice = series->slices().at(0);
    slice->setExploded(true);
    slice->setColor("#ffdaac");
    QPieSlice *slice2 = series->slices().at(1);
    slice2->setColor("#990000");

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Age statistics");

    series->setLabelsVisible();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->chart()->setAnimationOptions(QChart::AllAnimations);
    chartView->chart()->legend()->hide();
    return chartView;
}

void Benif::generer()
{

    try
    {

        QAxObject *excel = new QAxObject("Excel.Application");

        excel->setProperty("Visible", true);

        QAxObject *workbook = excel->querySubObject("Workbooks")->querySubObject("Add");

        QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);

        worksheet->querySubObject("Cells(int,int)", 1, 1)->setProperty("Value", "First name");
        worksheet->querySubObject("Cells(int,int)", 1, 2)->setProperty("Value", "Last name");
        worksheet->querySubObject("Cells(int,int)", 1, 3)->setProperty("Value", "Adress");
        worksheet->querySubObject("Cells(int,int)", 1, 4)->setProperty("Value", "Email");
        worksheet->querySubObject("Cells(int,int)", 1, 5)->setProperty("Value", "Phone");
        worksheet->querySubObject("Cells(int,int)", 1, 6)->setProperty("Value", "Description of assistance");
        worksheet->querySubObject("Cells(int,int)", 1, 7)->setProperty("Value", "Date of assistance");
        worksheet->querySubObject("Cells(int,int)", 1, 8)->setProperty("Value", "Restaurant Name");

        QSqlQuery qry;
        qry.prepare("SELECT NOM_ASS,NOM_RESPONSABLE,ADRESSE_ASS,EMAIL_ASS,TELEPHONE_ASS,"
                    "AIDE.DESCRIPTION_AIDE,AIDE.DATE_AIDE,RESTAURANT.NOM "
                    "FROM BENIFICIAIRE "
                    "INNER JOIN AIDE ON BENIFICIAIRE.ID_BENIF=AIDE.ID_BENIF "
                    "INNER JOIN RESTAURANT ON RESTAURANT.ID_RES=AIDE.ID_RES");
        if (qry.exec())
        {
            int row = 2; // Start at row 2 (the first row is for column headers)
            while (qry.next())
            {
                worksheet->querySubObject("Cells(int,int)", row, 1)->setProperty("Value", qry.value(0).toString());
                worksheet->querySubObject("Cells(int,int)", row, 2)->setProperty("Value", qry.value(1).toString());
                worksheet->querySubObject("Cells(int,int)", row, 3)->setProperty("Value", qry.value(2).toString());
                worksheet->querySubObject("Cells(int,int)", row, 4)->setProperty("Value", qry.value(3).toString());
                worksheet->querySubObject("Cells(int,int)", row, 5)->setProperty("Value", qry.value(4).toString());
                worksheet->querySubObject("Cells(int,int)", row, 6)->setProperty("Value", qry.value(5).toString());
                worksheet->querySubObject("Cells(int,int)", row, 7)->setProperty("Value", qry.value(6).toString());
                worksheet->querySubObject("Cells(int,int)", row, 8)->setProperty("Value", qry.value(7).toString());
                row++;
            }

            QAxObject *usedRange = worksheet->querySubObject("UsedRange");
            QAxObject *columns = usedRange->querySubObject("Columns");

            columns->dynamicCall("AutoFit");

            QString filePath = "C:/Users/youssef/Desktop/Benif.xlsx";
            QVariant result = workbook->dynamicCall("SaveAs(const QString&)", filePath);
            if (result.isNull())
            {
                QString errorMessage = "Error: " + workbook->property("LastError").toString();
            }

            workbook->dynamicCall("Close()");
            excel->dynamicCall("Quit()");
        }
    }
    catch (const std::exception &e)
    {
        // Handle the exception
        QString errorMessage = "Error: " + QString(e.what());
        QMessageBox::critical(nullptr, "Error", errorMessage);
    }
}
