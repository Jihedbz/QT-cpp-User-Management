#include "event.h"


Event::Event(int id_event, QString nom_event,  QString date_event, QString horaire_event, QString type_event,int participant_number, int entry_price)
{
    this->id_event=id_event;
    this->nom_event=nom_event;
    this->date_event=date_event;
    this->horaire_event=horaire_event;
    this->type_event=type_event;
    this->participant_number=participant_number;
    this->entry_price=entry_price;
   // this->id_em=id_em; // try to get it from the data base
}


bool Event::ajouter_event(){
    QSqlQuery query;
    query.prepare("insert into EVENEMENT (NOM_EVENT, DATE_EVENT, HORAIRE_EVENT, TYPE_EVENT, PRICE , PARTICIPANT_NUMBER)" "values (:nom_event,:date_event,:horaire_event,:type_event,:entry_price,:participant_number)");
    query.bindValue(":nom_event", nom_event);
    query.bindValue(":date_event", date_event);
    query.bindValue(":horaire_event", horaire_event);
    query.bindValue(":type_event", type_event);
    query.bindValue(":participant_number", participant_number);
    query.bindValue(":entry_price", entry_price);


    qDebug() << "Added";
    return query.exec();
}

QSqlQueryModel *Event::afficher_event(){

    QSqlQueryModel *model=new QSqlQueryModel();
    model->setQuery("SELECT ID_EVENT,NOM_EVENT,DATE_EVENT,HORAIRE_EVENT,TYPE_EVENT,PRICE,PARTICIPANT_NUMBER FROM EVENEMENT ");
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("ID"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("EVENT NAME"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("EVENT DATE"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("HORAIRE_EVENT"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("EVENT TYPE"));
    model->setHeaderData(5,Qt::Horizontal,QObject::tr("PRICE"));
    model->setHeaderData(6,Qt::Horizontal,QObject::tr("PARTICIPANT_NUMBER"));
    return model;
}

bool Event::supprimer_event(int i){

    QSqlQuery query;
    //QString res=QString::number(i);

    query.prepare("DELETE from EVENEMENT where ID_EVENT=:id");
    query.bindValue(":id", i);
    return query.exec();
}

bool Event::modifier_event(int i){

    QSqlQuery query;
    query.prepare("update EVENEMENT set NOM_EVENT=:nom_event,DATE_EVENT=:date_event,HORAIRE_EVENT=:horaire_event,TYPE_EVENT=:type_event,PRICE=:px,PARTICIPANT_NUMBER=:pn where ID_EVENT=:id");
    query.bindValue(":id", i);
    query.bindValue(":nom_event",nom_event);
    query.bindValue(":date_event",date_event);
    query.bindValue(":horaire_event",horaire_event);
    query.bindValue(":type_event",type_event);
    query.bindValue(":pn",participant_number);
    query.bindValue(":px",entry_price);
    qDebug() << "Updated";
    return query.exec();

}

QSqlQueryModel *Event::trier_event(QString critere)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM EVENEMENT WHERE LOWER(NOM_EVENT) = LOWER(:nom_event)");
    query.bindValue(":nom_event", critere);
    query.exec();
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);

    model->setHeaderData(0,Qt::Horizontal,QObject::tr("ID"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("EVENT NAME"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("EVENT DATE"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("EVENT TIME"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("EVENT TYPE"));
      model->setHeaderData(5,Qt::Horizontal,QObject::tr("PRICE"));
     model->setHeaderData(6,Qt::Horizontal,QObject::tr("PARTICIPANT_NUMBER"));


    return model;
}

QSqlQueryModel *Event::chercher_event(QString critere)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM EVENEMENT WHERE LOWER(NOM_EVENT) LIKE LOWER('%" + critere + "%')");

    query.exec();
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("ID"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("EVENT NAME"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("EVENT DATE"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("EVENT TIME"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("EVENT TYPE"));
    model->setHeaderData(5,Qt::Horizontal,QObject::tr("PRICE"));
    model->setHeaderData(6,Qt::Horizontal,QObject::tr("PARTICIPANT_NUMBER"));

    return model;
}



void Event::exportToPDF()
{
    QSqlDatabase db;
    QTableView tableView;
    QSqlQueryModel * Modal=new  QSqlQueryModel();
    QSqlQuery qry;
    qry.prepare("SELECT* FROM EVENEMENT");
    qry.exec();
    Modal->setQuery(qry);
    tableView.setModel(Modal);
    db.close();
    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = tableView.model()->rowCount();
    const int columnCount =  tableView.model()->columnCount();

    const QString strTitle ="ListedesEvenement";
    out <<  "<html>\n"
          /*  "<img src='C:/Users/youssef/Downloads/regerg.png' height='108' width='250'/>"*/
            "<head>\n"
            "<meta Content=\"Text/html; charset=Windows-1251\">\n"
        <<  QString("<title>%1</title>\n").arg(strTitle)
        <<  "</head>\n"
            "<body bgcolor=#ffffff link=#5000A0>\n"
        << QString("<h3 style=\" font-size: 50px; font-family: Arial, Helvetica, sans-serif; color: #e80e32; font-weight: lighter; text-align: center;\">%1</h3>\n").arg("LIST OF EVENEMENT")
        <<  "<br>"

        <<  "<table border=1 cellspacing=0 cellpadding=2 width=\"100%\">\n";
    out << "<thead><tr bgcolor=#f0f0f0>";
    for (int column = 0; column < columnCount; column++)
         if (!tableView.isColumnHidden(column))
             out << QString("<th>%1</th>").arg(tableView.model()->headerData(column, Qt::Horizontal).toString());
         out << "</tr></thead>\n";

         for (int row = 0; row < rowCount; row++)
         {
           out << "<tr>";
           for (int column = 0; column < columnCount; column++)
           {
                if (!tableView.isColumnHidden(column))
                {
                    QString data = tableView.model()->data(tableView.model()->index(row, column)).toString().simplified();
                    out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                }
           }
           out << "</tr>\n";
         }
         out <<  "</table>\n"
                 "<br><br>"
             <<"<br>"
             <<"<table border=1 cellspacing=0 cellpadding=2>\n";
         out << "<thead><tr bgcolor=#f0f0f0>";
         out <<  "</table>\n"
                 "</body>\n"
                 "</html>\n";
     QTextDocument *ListeDesevenement = new QTextDocument();
     ListeDesevenement->setHtml(strStream);
     QPrinter printer;
     QPrintDialog *dialog = new QPrintDialog(&printer, NULL);
     if (dialog->exec() == QDialog::Accepted)
     {
         ListeDesevenement->print(&printer);
     }
     printer.setOutputFormat(QPrinter::PdfFormat);
     printer.setPaperSize(QPrinter::A4);
     printer.setOutputFileName("/tmp/event.pdf");
     printer.setPageMargins(QMarginsF(15, 15, 15, 15));
     delete ListeDesevenement;
}



QSqlQueryModel *Event::recherche_event(QString titre)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("select * from EVENEMENT WHERE LOWER(NOM_EVENT) LIKE LOWER('%" + titre + "%')");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Website"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Address"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Phone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Email"));
    return model;
}



QChartView *Event::stat_type()
{
    int pub = 0;
    int Sensib = 0;

    QSqlQuery query;
    query.prepare("select * from EVENEMENT where lower(TYPE_EVENT)='charity'");
    query.exec();
    while (query.next())
        pub++;

    query.prepare("select * from EVENEMENT where lower(TYPE_EVENT)='advertisement'");
    query.exec();
    while (query.next())
        Sensib++;

    qDebug() << pub << Sensib;

    QPieSeries *series = new QPieSeries();
    series->append("Charity Event", pub);
    series->append("Advertisement Event", Sensib);

    int total = pub + Sensib;
    QString labelPublicite = QString(" Charity Event "
                                     "(%1%)")
                                 .arg((double)pub / total * 100, 0, 'f', 1);
    QString labelSensibilisation = QString(" Advertisement Event "
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
    chart->setTitle("Event Type statistics");

    series->setLabelsVisible();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->chart()->setAnimationOptions(QChart::AllAnimations);
    chartView->chart()->legend()->hide();
    return chartView;
}
