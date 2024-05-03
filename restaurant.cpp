#include "restaurant.h"
#include "ui_mainwindow.h"

Restaurant::Restaurant(int id, QString Nom, QString Website, QString Email, QString Adr, QString Tel)
{
    this->id_re = id;
    this->Nom = Nom;
    this->Website = Website;
    this->Email = Email;
    this->Adr = Adr;
    this->Tel = Tel;
}

bool Restaurant::ajouter_re()
{
    QSqlQuery query;
    query.prepare("insert into Restaurant (NOM, WEBSITE, ADRESSE, TELEPHONE_RES, EMAIL)"
                  "values (:Nom,:Web,:Adr,:Tel,:Email)");
    query.bindValue(":Nom", Nom);
    query.bindValue(":Web", Website);
    query.bindValue(":Adr", Adr);
    query.bindValue(":Tel", Tel);
    query.bindValue(":Email", Email);

    qDebug() << "Restaurant Added";
    return query.exec();
}

QSqlQueryModel *Restaurant::afficher_re()
{

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM Restaurant");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Website"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Address"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Phone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Email"));

    return model;
}

bool Restaurant::supprimer_re(int i)
{

    QSqlQuery query;

    query.prepare("DELETE from Restaurant where ID_RES=:id");
    query.bindValue(":id", i);
    return query.exec();
}

bool Restaurant::modifier_re(int i)
{

    QSqlQuery query;
    query.prepare("update Restaurant set NOM=:nom,WEBSITE=:website,ADRESSE=:adresse,TELEPHONE_RES=:tel,EMAIL=:email where ID_RES=:id");
    query.bindValue(":id", i);
    query.bindValue(":nom", Nom);
    query.bindValue(":website", Website);
    query.bindValue(":adresse", Adr);
    query.bindValue(":tel", Tel);
    query.bindValue(":email", Email);
    qDebug() << "Updated";
    return query.exec();
}

QSqlQueryModel *Restaurant::recherche_rest(QString titre)
{
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("select * from Restaurant WHERE LOWER(NOM) LIKE LOWER('%" + titre + "%')");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Website"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Address"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Phone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Email"));
    return model;
}

QSqlQueryModel *Restaurant::trierC()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM Restaurant ORDER BY NOM ASC ");

    return model;
}

QSqlQueryModel *Restaurant::trierD()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM Restaurant ORDER BY NOM DESC ");
    return model;
}

QChartView *Restaurant::stat_rest()
{
    int grandTunis = 0;
    int outside = 0;

    QSqlQuery query;
    query.prepare("SELECT * FROM Restaurant WHERE LOWER(adresse) LIKE '%ariana%' OR LOWER(adresse) LIKE '%benarous%' OR LOWER(adresse) LIKE '%manouba%'");
    query.exec();
    while (query.next())
        grandTunis++;

    query.prepare("SELECT * FROM Restaurant WHERE LOWER(adresse) NOT LIKE '%ariana%' OR LOWER(adresse) NOT LIKE '%benarous%' OR LOWER(adresse) NOT LIKE '%manouba%'");
    query.exec();
    while (query.next())
        outside++;

    QPieSeries *series = new QPieSeries();
    series->append("In Tunis", grandTunis);
    series->append("Outside Tunis The Capital", outside);

    int total = grandTunis + outside;
    // ajouter des étiquettes avec des pourcentages
    QString labelPublicite = QString("Outside The Capital (%1%)").arg((double)grandTunis / total * 100, 0, 'f', 1);
    QString labelSensibilisation = QString("In The Capital (%1%)").arg((double)outside / total * 100, 0, 'f', 1);
    series->slices().at(1)->setLabel(labelSensibilisation);
    series->slices().at(0)->setLabel(labelPublicite);

    QPieSlice *slice = series->slices().at(0);
    slice->setExploded(true);
    slice->setColor("#ffdaac");
    QPieSlice *slice2 = series->slices().at(1);
    slice2->setColor("#990000");

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

QAbstractItemModel *Restaurant::autofillTable(Restaurant r)
{

    QStandardItemModel *model = new QStandardItemModel(0, 5);

    // Set the header labels
    model->setHeaderData(0, Qt::Horizontal, "Title");
    model->setHeaderData(1, Qt::Horizontal, "Website");
    model->setHeaderData(2, Qt::Horizontal, "Email");
    model->setHeaderData(3, Qt::Horizontal, "Address");
    model->setHeaderData(4, Qt::Horizontal, "Phone");

    // Add new row
    QList<QStandardItem *> newRow;
    newRow.append(new QStandardItem(r.getNom()));
    newRow.append(new QStandardItem(r.getWebsite()));
    newRow.append(new QStandardItem(r.getEmail()));
    newRow.append(new QStandardItem(r.getAdr()));
    newRow.append(new QStandardItem(r.getTelephone()));
    model->appendRow(newRow);
    // Return the model
    return model;
}

QAbstractItemModel *Restaurant::ClearTable()
{

    QStandardItemModel *model = new QStandardItemModel(0, 5);
    model->clear();

    // Set the header labels
    model->setHeaderData(0, Qt::Horizontal, "Title");
    model->setHeaderData(1, Qt::Horizontal, "Website");
    model->setHeaderData(2, Qt::Horizontal, "Email");
    model->setHeaderData(3, Qt::Horizontal, "Address");
    model->setHeaderData(4, Qt::Horizontal, "Phone");

    // Return the model
    return model;
}
