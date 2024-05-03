#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employee.h"
#include "aides.h"
#include "event.h"
#include "restaurant.h"
#include "user.h"
#include "arduino.h"
#include <QMovie>
#include <QDateEdit>
#include <QMessageBox>
#include <QTableView>
#include <QLabel>
#include <QComboBox>
#include <QDebug>
#include <QPrinter>
#include <QPainter>
#include <QStandardPaths>
#include <QPixmap>
#include <QPrintDialog>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QSsl>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>
#include "qrcodegen.hpp"
#include <QAxBase>
#include <QAxObject>
#include <mshtml.h>
#include <QtWidgets>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QtNetwork>
#include <QSslSocket>
#include "messengerlanclient.h"
#include "messengerserver.h"
#include "calculator.h"
using namespace chatbox;

#define file_rx "[A-z0-9]+(\\.(jpg|png|gif|jpeg|jfif))"


Event ev;
MainWindow::MainWindow(QWidget * parent): QMainWindow(parent), ui(new Ui::MainWindow) {

  ui -> setupUi(this);

  int ret = A.connect_arduino(); // lancer la connexion à arduino
    switch (ret) {
    case (0):
      qDebug() << "arduino is available and connected to : " << A.getarduino_port_name();
      break;
    case (1):
      qDebug() << "arduino is available but not connected to :" << A.getarduino_port_name();
      break;
    case (-1):
      qDebug() << "arduino is not available";
    }

    QObject::connect(A.getserial(), SIGNAL(readyRead()), this, SLOT(update_label())); // permet de lancer

    //le slot update_label suite à la reception du signal readyRead (reception des données

    qDebug() << "=============" + A.getarduino_port_name();

    QObject::connect(A.getserial(), SIGNAL(readyRead()), this, SLOT(update_label())); // permet de lancer
    QSerialPort * serialPtr = A.getserial();

    if (!serialPtr) {
      qDebug() << "Error: Unable to get a pointer to QSerialPort object";
      return;
    }
    if (!serialPtr -> isOpen()) {

      serialPtr -> setPortName(A.getarduino_port_name()); // replace with the name of your serial port
      serialPtr -> setBaudRate(QSerialPort::Baud9600);
      serialPtr -> setDataBits(QSerialPort::Data8);
      serialPtr -> setParity(QSerialPort::NoParity);
      serialPtr -> setStopBits(QSerialPort::OneStop);
      serialPtr -> setFlowControl(QSerialPort::NoFlowControl);

      if (serialPtr -> open(QIODevice::ReadWrite)) {
        qDebug() << "Serial port opened successfully!";
      } else {
        qDebug() << "Failed to open serial port!";
        return;
      }
    }

//*************************************
  QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_em");
  Tableviewem -> setModel(Etmp.afficher_em());

  QTableView * Tableviewads = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableViewads");
  Tableviewads -> setModel(ads.afficher());

  QTableView *tableView_temp = ui->stackedWidget_2->findChild<QTableView*>("tableView_temp");
  tableView_temp->setModel(A.afficher_temperature());

  setFixedSize(1280, 720);
  this -> setWindowIcon(QIcon(":/Icons/Icons/Images/icon2.ico"));
  this -> setWindowTitle("Main Attentives");
  ui->stackedWidget->setCurrentIndex(0);
  ui->stackedWidget_2->setCurrentIndex(2);

  //**************************

  // Gif generators
  QMovie * gif_log = new QMovie(":/Icons/Icons/Images/resetpwd.gif");
  QMovie * gif_profile = new QMovie(":/Icons/Icons/Images/profile.gif");

  QLabel * gifform = ui -> stackedWidget_2 -> findChild < QLabel * > ("label_22");
  QMovie * gif_form = new QMovie(":/Icons/Icons/Images/form.gif");

  ui -> label_3 -> setMovie(gif_log);
  ui -> profilepic -> setMovie(gif_profile);
  gifform -> setMovie(gif_form);

  gif_form -> start();
  gif_log -> start();
  gif_profile -> start();
  //**************************

  QLabel * error = ui -> stackedWidget -> findChild < QLabel * > ("label_60");
  error -> hide();

  QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
  ui -> axWidget -> dynamicCall("setProperty(const QString&, const QVariant&)", "Silent", true);



  QThread * serialThread = new QThread();
   serialPtr -> moveToThread(serialThread);
  QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label()));

                   //  QSerialPort *serialPtr = A.getserial();
                     if (!serialPtr) {
                         qDebug() << "Error: Unable to get a pointer to QSerialPort object";
                         return;
                     }

                     if (!serialPtr->isOpen()) {
                         qDebug() << "Error: Serial port is not open";
                         return;
                     }

                     QObject::connect(serialPtr, &QSerialPort::readyRead, [=]() {
                         QByteArray data = serialPtr->readAll();
                         QString temp = QString::fromLatin1(data).trimmed();
                         if (!temp.isEmpty()) {

                             if(temp.length() == 2){
                             qDebug() << "Temperature:" << temp;
                             Arduino A;
                             QThread::msleep(4000);
                             A.add_temp(temp);
                             }
                         }
                     });
                     serialThread -> start();

}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_pushButton_clicked() {
  ui -> stackedWidget -> setCurrentIndex(0);
}

// Login and session management
void MainWindow::on_Login_btn_clicked() {
  QLabel * icon_em = ui -> stackedWidget -> findChild < QLabel * > ("icon_employee");
  QPushButton * button_em = ui -> stackedWidget -> findChild < QPushButton * > ("button_employee");

  QLabel * icon_aide = ui -> stackedWidget -> findChild < QLabel * > ("icon_donations");
  QPushButton * button_aide = ui -> stackedWidget -> findChild < QPushButton * > ("button_donations");

  QLabel * icon_events = ui -> stackedWidget -> findChild < QLabel * > ("icon_events");
  QPushButton * button_events = ui -> stackedWidget -> findChild < QPushButton * > ("button_events");

  QLabel * icon_benif = ui -> stackedWidget -> findChild < QLabel * > ("icon_benif");
  QPushButton * button_benif = ui -> stackedWidget -> findChild < QPushButton * > ("button_benif");

  QLabel * icon_restaurants = ui -> stackedWidget -> findChild < QLabel * > ("icon_restaurants");
  QPushButton * button_restaurants = ui -> stackedWidget -> findChild < QPushButton * > ("button_restaurants");

  QLineEdit * lineEdit_email = ui -> stackedWidget -> findChild < QLineEdit * > ("email_login");
  QString Email = lineEdit_email -> text();

  QLineEdit * lineEdit_Password = ui -> stackedWidget -> findChild < QLineEdit * > ("password_2");
  QString Password = lineEdit_Password -> text();
  Employee e;
  user u;
  bool valid = u.Checkuser(Email, Password);
  if (valid) {
    u = u.Login(Email, Password);
    e = u.getEm();
    QString Role = u.getrole();
    QLabel * error = ui -> stackedWidget -> findChild < QLabel * > ("label_60");
    error -> hide();
    QString name = e.getNom() + " " + e.getPrenom();
    //QString role = e.getFonction();

    QLabel * nameholder = ui -> stackedWidget -> findChild < QLabel * > ("label_7");
    QLabel * roleholder = ui -> stackedWidget -> findChild < QLabel * > ("label_8");
    nameholder -> setText(name);
    roleholder -> setText(Role);

    icon_em -> hide();
    button_em -> hide();
    icon_aide -> hide();
    button_aide -> hide();
    icon_events -> hide();
    button_events -> hide();
    icon_benif -> hide();
    button_benif -> hide();
    icon_restaurants -> hide();
    button_restaurants -> hide();

    if (Role == "Admin") {
      // instructions for admin role
        ui -> stackedWidget -> setCurrentIndex(2);
        ui -> stackedWidget_2 -> setCurrentIndex(1);
        icon_em -> show();
        button_em -> show();
        icon_aide -> show();
        button_aide -> show();
        icon_events -> show();
        button_events -> show();
        icon_benif -> show();
        button_benif -> show();
        icon_restaurants -> show();
        button_restaurants -> show();

    } else if (Role == "Event Coordinator") {
      // instructions for event coordinator role
        ui -> stackedWidget -> setCurrentIndex(2);
        ui -> stackedWidget_2 -> setCurrentIndex(23);
        button_events -> show();
        icon_events -> show();

        button_events -> move(9,190);
        icon_events -> move(20,200);

    } else if (Role == "Human Resources") {
      // instructions for human resources role

      ui -> stackedWidget -> setCurrentIndex(2);
      ui -> stackedWidget_2 -> setCurrentIndex(1);
      icon_em -> show();
      button_em -> show();
      button_em -> move(9,190);
      icon_em -> move(20,200);

    } else if (Role == "Partner Relationship") {
      // instructions for partner relationship role

      ui -> stackedWidget -> setCurrentIndex(2);
      ui -> stackedWidget_2 -> setCurrentIndex(10);
      button_restaurants -> show();
      icon_restaurants -> show();
      button_restaurants -> move(9,190);
      icon_restaurants -> move(20,200);

    } else if (Role == "Fundraising Agent") {
      // instructions for fundraising agent role

      ui -> stackedWidget -> setCurrentIndex(2);
      ui -> stackedWidget_2 -> setCurrentIndex(5);
      icon_aide -> show();
      button_aide -> show();
      button_aide -> move(9,190);
      icon_aide -> move(20,200);

    } else if (Role == "Charitable giving Agent") {
      // instructions for charitable giving agent role
        ui -> stackedWidget -> setCurrentIndex(2);
        ui -> stackedWidget_2 -> setCurrentIndex(17);
        icon_benif -> show();
        button_benif -> show();
        icon_benif -> move(20,200);
        button_benif -> move(9,190);

    } else {

      qDebug() << "Role not found.";
    }
  } else {
    QLabel * error = ui -> stackedWidget -> findChild < QLabel * > ("label_60");
    error -> show();
  }
}
//*********************

void MainWindow::on_forget_btn_clicked() {
  ui -> stackedWidget -> setCurrentIndex(1);
}
//**************************************
void MainWindow::on_forget_btn_2_clicked() {
  ui -> stackedWidget -> setCurrentIndex(0);
  QLabel * email_check = ui -> stackedWidget -> findChild < QLabel * > ("email_check");
  email_check -> hide();
  ui -> forget_email -> clear();
}

// Add Employee
void MainWindow::on_pushButton_8_clicked() {
  if (control(1)) {
    QLineEdit * lineEdit_nom = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_Nom");
    QString Nom = lineEdit_nom -> text();

    QLineEdit * lineEdit_Prenom = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_Prenom");
    QString Prenom = lineEdit_Prenom -> text();

    QLineEdit * lineEdit_Email = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_Email");
    QString Email = lineEdit_Email -> text();

    QLineEdit * lineEdit_Adresse = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_Adresse");
    QString Adresse = lineEdit_Adresse -> text();

    QLineEdit * lineEdit_Salaire = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_Salaire");
    int Salaire = lineEdit_Salaire -> text().toInt();

    QLineEdit * lineEdit_Telephone = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_Telephone");
    QString Telephone = lineEdit_Telephone -> text();

    QDateEdit * dateEdit = ui -> stackedWidget_2 -> findChild < QDateEdit * > ("dateEdit_date");
    QString Date_nai = dateEdit -> date().toString();

    QCheckBox * checkBox_M = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_H");
    QCheckBox * checkBox_F = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_F");

    QCheckBox * checkBox_Event_Coordinator = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_event");
    QCheckBox * checkBox_Human_Resources = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_human");
    QCheckBox * checkBox_Partner_Relationship = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_part");
    QCheckBox * checkBox_Fundraising_Agent = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_fund");
    QCheckBox * checkBox_Charitable_giving_Agent = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_charity");

    QString Role = "";
    if (checkBox_Event_Coordinator -> isChecked()) {
      Role = "Event Coordinator";
    } else if (checkBox_Human_Resources -> isChecked()) {
      Role = "Human Resources";
    } else if (checkBox_Partner_Relationship -> isChecked()) {
      Role = "Partner Relationship";
    } else if (checkBox_Fundraising_Agent -> isChecked()) {
      Role = "Fundraising Agent";
    } else if (checkBox_Charitable_giving_Agent -> isChecked()) {
      Role = "Charitable giving Agent";
    } else {
      // handle unknown role
      qDebug() << "Role not found.";
    }
    qDebug() << Role;

    QString Sexe = "";
    if (checkBox_M -> isChecked()) {
      Sexe = "Male";
    } else if (checkBox_F -> isChecked()) {
      Sexe = "Female";
    }

    Employee e(0, Nom, Prenom, Email, Sexe, Adresse, Role, Date_nai, Telephone, Salaire);

    bool test = e.ajouter_em();

    if (test) {
      QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_em");
      Tableviewem -> setModel(Etmp.afficher_em());
    } else {
      QMessageBox::critical(nullptr, QObject::tr("Error"),
        QObject::tr("The element is refused \n"
          "Click ok to exit."),
        QMessageBox::Ok);
    }
  }
}
//**************************

// Hide all the error handlers in the add employee page
void MainWindow::on_back1_btn_3_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(1);
  QLabel * error_name = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_nom");
  QLabel * error_prenom = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_prenom");
  QLabel * error_email = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_email");
  QLabel * error_salaire = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_salaire");
  QLabel * error_telephone = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_telephone");
  QLabel * error_fonction = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_fonction");
  QLabel * error_adresse = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_adresse");
  QLabel * error_sexe = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_sexe");

  error_name -> hide();
  error_prenom -> hide();
  error_email -> hide();
  error_salaire -> hide();
  error_telephone -> hide();
  error_fonction -> hide();
  error_adresse -> hide();
  error_sexe -> hide();
}
//******************************

void MainWindow::on_back1_btn_2_clicked() {
  QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_em");
  Tableviewem -> setModel(Etmp.afficher_em());
  ui -> stackedWidget_2 -> setCurrentIndex(1);
}

void MainWindow::on_pushButton_10_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(0);
}

void MainWindow::on_pushButton_11_clicked() {
  QLineEdit * rechercher_em = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_rechercheem");
  rechercher_em -> setPlaceholderText("Function");
  ui -> stackedWidget_2 -> setCurrentIndex(2);

  QComboBox * Combodelete = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox");
  Combodelete -> clear();

  QSqlQuery query;
  query.prepare("SELECT ID_EM FROM EMPLOYEE");
  if (query.exec()) {
    while (query.next()) {
      int i = query.value(0).toInt();
      QString intString = QString::number(i);
      Combodelete -> addItem(intString);
    }
  }
}

//**************************

// Updating the comboBox and all the lineEdits in the Update employee page
void MainWindow::on_pushButton_14_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(3);

  QComboBox * Comboupdate = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox_2");
  Comboupdate -> clear();
  Comboupdate -> addItem("Select");
  QSqlQuery query, query1;
  query.prepare("SELECT ID_EM FROM EMPLOYEE");
  if (query.exec()) {
    while (query.next()) {
      int i = query.value(0).toInt();
      QString intString = QString::number(i);
      Comboupdate -> addItem(intString);
    }
  }

  connect(Comboupdate, QOverload < int > ::of( & QComboBox::currentIndexChanged), this, [ = ](int index) {

    QCheckBox * checkBox_Event_Coordinator = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_event_2");
    QCheckBox * checkBox_Human_Resources = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_human_2");
    QCheckBox * checkBox_Partner_Relationship = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_part_2");
    QCheckBox * checkBox_Fundraising_Agent = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_fund_2");
    QCheckBox * checkBox_Charitable_giving_Agent = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_charity_2");

    checkBox_Event_Coordinator -> setChecked(false);
    checkBox_Human_Resources -> setChecked(false);
    checkBox_Partner_Relationship -> setChecked(false);
    checkBox_Fundraising_Agent -> setChecked(false);
    checkBox_Charitable_giving_Agent -> setChecked(false);

    QString selectedItemText = Comboupdate -> currentText();
    int ID = selectedItemText.toInt();

    QSqlQuery query1;

    query1.prepare("SELECT NOM, PRENOM, EMAIL, SEXE, ADRESSE, FONCTION, DATE_DE_NAISSANCE, TELEPHONE, SALAIRE FROM EMPLOYEE WHERE ID_EM = :id");
    query1.bindValue(":id", ID);
    qDebug() << "preparation completed ";
    if (query1.exec() && query1.next()) {

      QString firstName = query1.value("NOM").toString();
      QString lastName = query1.value("PRENOM").toString();
      QString email = query1.value("EMAIL").toString();
      QString sexe = query1.value("SEXE").toString();
      QString adresse = query1.value("ADRESSE").toString();
      QString date_nai = query1.value("DATE_DE_NAISSANCE").toString();
      QString telephone = query1.value("TELEPHONE").toString();
      QString fonction = query1.value("FONCTION").toString();
      QString salaire = query1.value("SALAIRE").toString();
      qDebug() << "retrieving completed";

      QLineEdit * lineEdit_firstname = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_updatefirstname");
      lineEdit_firstname -> setText(firstName);

      QLineEdit * lineEdit_lastname = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_updatelastname");
      lineEdit_lastname -> setText(lastName);

      QLineEdit * lineEdit_email = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_updateemail");
      lineEdit_email -> setText(email);

      QLineEdit * lineEdit_adresse = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_updateadresse");
      lineEdit_adresse -> setText(adresse);

      QLineEdit * lineEdit_salaire = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_updatesalaire");
      lineEdit_salaire -> setText(salaire);

      QLineEdit * lineEdit_telephone = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_updatephone");
      lineEdit_telephone -> setText(telephone);

      QDateEdit * lineEdit_date = ui -> stackedWidget_2 -> findChild < QDateEdit * > ("dateEdit_lineEdit_updatedate");
      QDate birthdate = QDate::fromString(date_nai, "ddd MMM d yyyy");
      lineEdit_date -> setDate(birthdate);

      if (fonction == "Event Coordinator") {
        checkBox_Event_Coordinator -> setChecked(true);
      } else if (fonction == "Human Resources") {
        checkBox_Human_Resources -> setChecked(true);
      } else if (fonction == "Partner Relationship") {
        checkBox_Partner_Relationship -> setChecked(true);
      } else if (fonction == "Fundraising Agent") {
        checkBox_Fundraising_Agent -> setChecked(true);
      } else if (fonction == "Charitable giving Agent") {
        checkBox_Charitable_giving_Agent -> setChecked(true);
      } else {
        qDebug() << "Unknown role";
      }

      QCheckBox * checkBox_M = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_H_2");
      QCheckBox * checkBox_F = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_F_2");

      if (sexe == "Male") {
        checkBox_M -> setChecked(true);
      } else if (sexe == "Female") {
        checkBox_F -> setChecked(true);
      }
    }
  });
}
//**************************

// Clearing the update page after leaving it
void MainWindow::on_back1_btn_7_clicked() {
  QStringList lineEditNames = {
    "lineEdit_updatefirstname",
    "lineEdit_updatelastname",
    "lineEdit_updateemail",
    "lineEdit_updateadresse",
    "lineEdit_updatefunction",
    "lineEdit_updatesalaire",
    "lineEdit_updatephone"
  };

  for (const QString & name: lineEditNames) {
    QLineEdit * lineEdit = ui -> stackedWidget_2 -> findChild < QLineEdit * > (name);
    if (lineEdit) {
      lineEdit -> clear();
    }
  }

  QDateEdit * dateEdit = ui -> stackedWidget_2 -> findChild < QDateEdit * > ("dateEdit_lineEdit_updatedate");
  if (dateEdit) {
    dateEdit -> clear();
  }

  QCheckBox * checkBox_M = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_H_2");
  QCheckBox * checkBox_F = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_F_2");
  if (checkBox_M && checkBox_F) {
    checkBox_M -> setChecked(false);
    checkBox_F -> setChecked(false);
  }

  ui -> stackedWidget_2 -> setCurrentIndex(1);
}
//**************************

// Update Employee
void MainWindow::on_update_em_button_clicked() {

  QLineEdit * lineEdit_firstname = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_updatefirstname");
  QString firstname = lineEdit_firstname -> text();

  QLineEdit * lineEdit_lastname = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_updatelastname");
  QString lastname = lineEdit_lastname -> text();

  QLineEdit * lineEdit_email = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_updateemail");
  QString email = lineEdit_email -> text();

  QLineEdit * lineEdit_adresse = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_updateadresse");
  QString adresse = lineEdit_adresse -> text();

  QLineEdit * lineEdit_salaire = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_updatesalaire");
  int Salaire = lineEdit_salaire -> text().toInt();

  QLineEdit * lineEdit_telephone = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_updatephone");
  QString telephone = lineEdit_telephone -> text();

  QDateEdit * lineEdit_date = ui -> stackedWidget_2 -> findChild < QDateEdit * > ("dateEdit_lineEdit_updatedate");
  QString Date_nai = lineEdit_date -> date().toString();

  QCheckBox * checkBox_M = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_H_2");
  QCheckBox * checkBox_F = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_F_2");

  QCheckBox * checkBox_Event_Coordinator = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_event_2");
  QCheckBox * checkBox_Human_Resources = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_human_2");
  QCheckBox * checkBox_Partner_Relationship = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_part_2");
  QCheckBox * checkBox_Fundraising_Agent = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_fund_2");
  QCheckBox * checkBox_Charitable_giving_Agent = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_charity_2");

  QString Role = "";
  if (checkBox_Event_Coordinator -> isChecked()) {
    Role = "Event Coordinator";
  } else if (checkBox_Human_Resources -> isChecked()) {
    Role = "Human Resources";
  } else if (checkBox_Partner_Relationship -> isChecked()) {
    Role = "Partner Relationship";
  } else if (checkBox_Fundraising_Agent -> isChecked()) {
    Role = "Fundraising Agent";
  } else if (checkBox_Charitable_giving_Agent -> isChecked()) {
    Role = "Charitable giving Agent";
  } else {
    // handle unknown role
    qDebug() << "Role not found.";
  }

  QString Sexe = "";
  if (checkBox_M -> isChecked()) {
    Sexe = "Male";
  } else if (checkBox_F -> isChecked()) {
    Sexe = "Female";
  }

  QComboBox * Comboupdate = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox_2");
  QString selectedItemText = Comboupdate -> currentText();
  int ID = selectedItemText.toInt();
  Employee e(ID, firstname, lastname, email, Sexe, adresse, Role, Date_nai, telephone, Salaire);

  bool test1 = e.modifier_em(ID);

  if (test1) {
    QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_em");
    Tableviewem -> setModel(Etmp.afficher_em());
    qDebug() << "Updated";
  } else {
    QMessageBox::critical(nullptr, QObject::tr("Error"),
      QObject::tr("The element is refused \n"
        "Click ok to exit."),
      QMessageBox::Ok);
  }
}
//**************************

void MainWindow::on_pushButton_17_clicked() {
  QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_em");
  Tableviewem -> setModel(Etmp.afficher_em());
  QLineEdit * rechercher_em = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_rechercheem");
  rechercher_em -> clear();
  rechercher_em -> setPlaceholderText("Function");
}

void MainWindow::on_pushButton_12_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(4);
  Employee e;
  QGridLayout * StatsLayout1 = ui -> stackedWidget_2 -> findChild < QGridLayout * > ("gridLayout1");
  StatsLayout1 -> addWidget(e.stat_gender(), 0, 0);
  QGridLayout * StatsLayout2 = ui -> stackedWidget_2 -> findChild < QGridLayout * > ("gridLayout2");
  StatsLayout2 -> addWidget(e.stat_age(), 0, 1);
}

void MainWindow::on_back1_btn_4_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(1);
}

bool MainWindow::control(int n) {
  QLineEdit * lineEdit_nom = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_Nom");
  QLineEdit * lineEdit_Prenom = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_Prenom");
  QLineEdit * lineEdit_Email = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_Email");
  QLineEdit * lineEdit_Adresse = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_Adresse");
  QLineEdit * lineEdit_Salaire = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_Salaire");
  QLineEdit * lineEdit_Telephone = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_Telephone");
  QCheckBox * checkBox_M = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_H");
  QCheckBox * checkBox_F = ui -> stackedWidget_2 -> findChild < QCheckBox * > ("checkBox_F");

  QLabel * error_name = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_nom");
  QLabel * error_prenom = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_prenom");
  QLabel * error_email = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_email");
  QLabel * error_salaire = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_salaire");
  QLabel * error_telephone = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_telephone");
  QLabel * error_adresse = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_adresse");
  QLabel * error_sexe = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_sexe");

  if (n == 1) {
    if ((lineEdit_nom -> text() == "") || (lineEdit_Prenom -> text() == "") || (lineEdit_Email -> text() == "") || (lineEdit_Adresse -> text() == "") || (lineEdit_Salaire -> text() == "") || (lineEdit_Telephone -> text() == "") || (!(checkBox_M -> isChecked()) && (!(checkBox_F -> isChecked())))) {
      if (lineEdit_nom -> text() == "") {
        error_name -> show();
        error_name -> setText("Champs Obligatoire !");
      }

      if (lineEdit_Prenom -> text() == "") {
        error_prenom -> show();
        error_prenom -> setText("Champs Obligatoire !");
      }

      if (lineEdit_Email -> text() == "") {
        error_email -> show();
        error_email -> setText("Champs Obligatoire !");
      }

      if (lineEdit_Adresse -> text() == "") {
        error_adresse -> show();
        error_adresse -> setText("Champs Obligatoire !");
      }

      if (lineEdit_Salaire -> text() == "") {
        error_salaire -> show();
        error_salaire -> setText("Champs Obligatoire !");
      }

      if (lineEdit_Telephone -> text() == "") {
        error_telephone -> show();
        error_telephone -> setText("Champs Obligatoire !");
      }

      if (!(checkBox_F -> isChecked()) && (!(checkBox_M -> isChecked()))) {
        error_sexe -> show();
        error_sexe -> setText("Choisier un Sexe!");
      }

      qDebug() << "suii1";

      return 0;
    } else if ((lineEdit_Telephone -> text().length() < 8) || !(lineEdit_Email -> text().contains("@")) || !(lineEdit_Email -> text().contains(".")) || (lineEdit_nom -> text().length() < 3) || (lineEdit_Prenom -> text().length() < 2) || (lineEdit_Adresse -> text().length() < 10)) {
      if (lineEdit_Telephone -> text().length() < 8) {
        error_telephone -> show();
        error_telephone -> setText("Doit Contenir 8 chiffres !");
      }

      if (lineEdit_Adresse -> text().length() < 10) {
        error_adresse -> show();
        error_adresse -> setText("Doit Contenir 10 chiffres !");
      }

      if (lineEdit_nom -> text().length() < 3) {
        error_name -> show();
        error_name -> setText("Doit Contenir 3 chiffres !");
      }

      if (lineEdit_Prenom -> text().length() < 3) {
        error_prenom -> show();
        error_prenom -> setText("Doit Contenir 3 chiffres !");
      }

      if (!(lineEdit_Email -> text().contains("@")) || !(lineEdit_Email -> text().contains("."))) {
        error_email -> show();
        error_email -> setText("Forme Invalide ! (manque @ ou . )");
      }
      qDebug() << "suii2";

      return 0;
    } else {
      qDebug() << "suii3";

      return 1;
    }
  }
  qDebug() << "suii4";
  return 1;
}

void MainWindow::on_pdf_employee_clicked() {

  QSqlDatabase db;

  QTableView tableView;
  QSqlQueryModel * Modal = new QSqlQueryModel();
  QSqlQuery qry;

  QString titre = ui -> lineEdit_rechercheem -> text().toLower();
  qry.prepare("select * from EMPLOYEE WHERE LOWER(NOM) LIKE LOWER('%" + titre + "%')");
  qry.exec();
  Modal -> setQuery(qry);
  tableView.setModel(Modal);
  db.close();
  QString strStream;
  QTextStream out( & strStream);

  const int rowCount = tableView.model() -> rowCount();
  const int columnCount = tableView.model() -> columnCount();

  const QString strTitle = "ListedesEmployees";
  out << "<html>\n"
  "<img src=':/Icons/Icons/Images/logo.png' height='108' width='250'/>"
  "<head>\n"
  "<meta Content=\"Text/html; charset=Windows-1251\">\n" <<
  QString("<title>%1</title>\n").arg(strTitle) <<
    "</head>\n"
  "<body bgcolor=#ffffff link=#5000A0>\n" <<
  QString("<h3 style=\" font-size: 50px; font-family: Arial, Helvetica, sans-serif; color: #e80e32; font-weight: lighter; text-align: center;\">%1</h3>\n").arg("LISTE DES EMPLOYEES") <<
    "<br>"

    <<
    "<table border=1 cellspacing=0 cellpadding=2 width=\"100%\">\n";
  out << "<thead><tr bgcolor=#f0f0f0>";
  for (int column = 0; column < columnCount; column++)
    if (!tableView.isColumnHidden(column))
      out << QString("<th>%1</th>").arg(tableView.model() -> headerData(column, Qt::Horizontal).toString());
  out << "</tr></thead>\n";

  for (int row = 0; row < rowCount; row++) {
    out << "<tr>";
    for (int column = 0; column < columnCount; column++) {
      if (!tableView.isColumnHidden(column)) {
        QString data = tableView.model() -> data(tableView.model() -> index(row, column)).toString().simplified();
        out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
      }
    }
    out << "</tr>\n";
  }
  out << "</table>\n"
  "<br><br>" <<
  "<br>" <<
  "<table border=1 cellspacing=0 cellpadding=2>\n";
  out << "<thead><tr bgcolor=#f0f0f0>";
  out << "</table>\n"
  "</body>\n"
  "</html>\n";
  QTextDocument * ListeDesemployee = new QTextDocument();
  ListeDesemployee -> setHtml(strStream);
  QPrinter printer;
  QPrintDialog * dialog = new QPrintDialog( & printer, NULL);
  if (dialog -> exec() == QDialog::Accepted) {
    ListeDesemployee -> print( & printer);
  }
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setPaperSize(QPrinter::A4);
  printer.setOutputFileName("/tmp/Employee.pdf");
  printer.setPageMargins(QMarginsF(15, 15, 15, 15));
  delete ListeDesemployee;
}

// mailing recovery
void MainWindow::on_Recover_btn_clicked() {
  user u;
  QLabel * email_check = ui -> stackedWidget -> findChild < QLabel * > ("email_check");

  email_check -> hide();

  QString to = ui -> forget_email -> text();

  bool valid = u.Checkemail(to);
  if (valid) {
    QString password = u.Getpassword(to);
    QString url = "https://matrix.vintage619.tn/send-email.php?";
    QString from = "From=mainsattentive@vintage619.tn";
    QString Subject = "Password%20Reset";
    QString Body = "Here's%20your%20password Your Password:" + password;
    QString FinalUrl = url + from + "&To=" + to + "&Subject=" + Subject + "&Body=" + Body + "&smtpServer=smtp.vintage619.tn&Username=med@vintage619.tn&Password=Medmed1&port=25";

    QNetworkAccessManager manager;
    QNetworkRequest request(FinalUrl);
    QNetworkReply * reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), & loop, SLOT(quit()));
    loop.exec();

    if (reply -> error() == QNetworkReply::NoError) {
      QString html = reply -> readAll();
      qDebug() << "HTML Content: " << html;
      email_check -> setText(html);
      email_check -> show();
    } else {
      qDebug() << "Error loading page: " << reply -> errorString();
      email_check -> setText("Server Down!");
      email_check -> show();
    }

    reply -> deleteLater();
  } else {
    email_check -> setText("Check your E-mail!");
    email_check -> show();
  }
}

//********************************************
void MainWindow::on_Login_btn_2_clicked() {

  ui -> stackedWidget -> setCurrentIndex(3);
  QLabel * error = ui -> stackedWidget -> findChild < QLabel * > ("label_59");
  error -> hide();

  QComboBox * Combo = ui -> stackedWidget -> findChild < QComboBox * > ("comboBox_3");
  Combo -> clear();

  QSqlQuery query;
  query.prepare("SELECT e.ID_EM FROM EMPLOYEE e LEFT JOIN COMPTE c ON e.ID_EM = c.ID_EM WHERE c.ID_EM IS NULL");

  if (query.exec()) {
    while (query.next()) {
      int i = query.value(0).toInt();
      QString intString = QString::number(i);
      Combo -> addItem(intString);
    }
  }
}

bool Checkexistence(int i) {
  int exists = 0;
  QSqlQuery query;
  query.prepare("SELECT COUNT(*) FROM COMPTE WHERE ID_EM= :id");

  query.bindValue(":id", i);

  if (query.exec() && query.next()) {
    exists = query.value(0).toInt();
  }
  return exists;
}

void MainWindow::on_Recover_btn_2_clicked() {
  QLabel * error = ui -> stackedWidget -> findChild < QLabel * > ("label_59");
  QLineEdit * lineEdit_pwd = ui -> stackedWidget -> findChild < QLineEdit * > ("lineEdit_pwd");
  QString pwd = lineEdit_pwd -> text();
  QLineEdit * lineEdit_pwd1 = ui -> stackedWidget -> findChild < QLineEdit * > ("lineEdit_2");
  QString pwd1 = lineEdit_pwd1 -> text();
  QComboBox * Combo = ui -> stackedWidget -> findChild < QComboBox * > ("comboBox_3");
  int id = Combo -> currentText().toInt();

  if (pwd != pwd1) {
    error -> show();
  } else {
    Employee e;
    e = e.retrievedata(id);
    QString email = e.getEmail();
    QString role = e.getFonction();
    user u(e, email, role, pwd);

    bool valid1 = u.Ajouter_user(id);
    if (valid1) {
      QMessageBox::information(nullptr, QObject::tr("Created!"),
        QObject::tr("The Account is created \n"
          "Click ok to exit."),
        QMessageBox::Ok);
    } else {
      QMessageBox::critical(nullptr, QObject::tr("Error"),
        QObject::tr("The Account is refused \n"
          "Click ok to exit."),
        QMessageBox::Ok);
    }
  }
}
//******************************************

void MainWindow::on_forget_btn_3_clicked() {
  ui -> stackedWidget -> setCurrentIndex(0);
}

void MainWindow::on_radioButton_clicked() {
  QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_em");
  Tableviewem -> setModel(Etmp.trierASC());
}

void MainWindow::on_radioButton_2_clicked() {
  QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_em");
  Tableviewem -> setModel(Etmp.trierDESC());
}

void MainWindow::on_pushButton_19_clicked() {

  QComboBox * Combodelete = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox");
  QString selectedItemText = Combodelete -> currentText();
  int ID = selectedItemText.toInt();

  bool test1 = Etmp.supprimer_em(ID);
  if (test1) {
    QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_em");
    Tableviewem -> setModel(Etmp.afficher_em());
  } else {
    QMessageBox::critical(nullptr, QObject::tr("Error"),
      QObject::tr("The element is refused \n"
        "Click ok to exit."),
      QMessageBox::Ok);
  }
}

//********************Aide********************************

void MainWindow::on_pushButton_3_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(6);

  QComboBox * Combo = ui -> stackedWidget -> findChild < QComboBox * > ("comboBox_5");
  Combo -> clear();

  QSqlQuery query;
  query.prepare("SELECT ID_RES FROM RESTAURANT");

  if (query.exec()) {
    while (query.next()) {
      int i = query.value(0).toInt();

      QString intString = QString::number(i);

      Combo -> addItem(intString);
    }
  }
}

void MainWindow::on_pushButton_6_clicked() {
  if (control(6)) {

    QLineEdit * lineEdit_donneur = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_donneur");
    QString donneur = lineEdit_donneur -> text();

    QLineEdit * lineEdit_description = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_description");
    QString description_aide = lineEdit_description -> text();

    QDateEdit * dateEdit2 = ui -> stackedWidget_2 -> findChild < QDateEdit * > ("date_donation");
    QString date_aide = dateEdit2 -> date().toString();

    QComboBox * comboBox3 = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox_5");
    int ID_res = comboBox3 -> currentText().toInt();

    QComboBox * comboBox2 = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox_4");
    QString type_aide = comboBox2 -> currentText();

    Aides A(0, ID_res, description_aide, date_aide, type_aide, donneur);

    bool test = A.ajouter();

    if (test) {

      qDebug() << "Added";
    } else {
      qDebug() << "Refused";
    }
  }
}

void MainWindow::on_pushButton_2_clicked() {

  ui -> stackedWidget_2 -> setCurrentIndex(7);

  QComboBox * ComboBoxdelaide = ui -> stackedWidget -> findChild < QComboBox * > ("comboBox_6");
  ComboBoxdelaide -> clear();

  QSqlQuery query;
  query.prepare("SELECT ID_AIDE FROM AIDE");
  if (query.exec()) {
    while (query.next()) {
      int i = query.value(0).toInt();
      QString intString = QString::number(i);
      ComboBoxdelaide -> addItem(intString);
    }
  }

  QTableView * Tableviewads = ui -> stackedWidget -> findChild < QTableView * > ("tableViewads");
  Tableviewads -> setModel(ads.afficher());

  QLabel * notif = ui -> stackedWidget_2 -> findChild < QLabel * > ("notif_money");
  QLabel * notif1 = ui -> stackedWidget_2 -> findChild < QLabel * > ("notif_dress");
  QLabel * notif2 = ui -> stackedWidget_2 -> findChild < QLabel * > ("notif_food");

  notif -> hide();
  notif1 -> hide();
  notif2 -> hide();

  ads.notification(ui);
}

void MainWindow::on_back_aid_dash_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(5);
}

void MainWindow::on_food_btn_clicked() {
  QMessageBox::information(nullptr, QObject::tr("Insufficient Resources"),
    QObject::tr("There is only two elements or less of type "
      "FOOD"
      " \n"
      "click cancel to exit."),
    QMessageBox::Cancel);
}

void MainWindow::on_money_btn_clicked() {
  QMessageBox::information(nullptr, QObject::tr("Insufficient Resources"),
    QObject::tr("Insufficient amount of money \n"
      "click cancel to exit."),
    QMessageBox::Cancel);
}

void MainWindow::on_dress_btn_clicked() {
  QMessageBox::information(nullptr, QObject::tr("Insufficient Resources"),
    QObject::tr("There is only two elements or less of type "
      "Clothes"
      " \n"
      "click cancel to exit."),
    QMessageBox::Cancel);
}

void MainWindow::on_pushButton_13_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(5);
}

void MainWindow::on_Delete_aid_clicked() {
  QComboBox * ComboBoxdelaide = ui -> stackedWidget -> findChild < QComboBox * > ("comboBox_6");
  QString selectedItemText = ComboBoxdelaide -> currentText();
  int ID = selectedItemText.toInt();

  bool test1 = ads.supprimer(ID);
  if (test1) {
    QTableView * Tableviewem = ui -> stackedWidget -> findChild < QTableView * > ("tableViewads");
    Tableviewem -> setModel(ads.afficher());
    QMessageBox::information(nullptr, QObject::tr("Deleted!"),
      QObject::tr("The element is deleted \n"
        "Click ok to exit."),
      QMessageBox::Ok);
  } else {
    QMessageBox::critical(nullptr, QObject::tr("Error"),
      QObject::tr("The element is refused \n"
        "Click ok to exit."),
      QMessageBox::Ok);
  }
}

void MainWindow::on_radioButton_3_clicked() {
  QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableViewads");
  Tableviewem -> setModel(ads.trierASC());
}

void MainWindow::on_radioButton_4_clicked() {
  QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableViewads");
  Tableviewem -> setModel(ads.trierDESC());
}

void MainWindow::on_pdf_aid_clicked() {
  QSqlDatabase db;

  QTableView tableView_rest;
  QSqlQueryModel * Modal = new QSqlQueryModel();
  QSqlQuery qry;

  QString titre = ui -> aid_recherche -> text().toLower();
  qry.prepare("select * from AIDE WHERE LOWER(DONNEUR) LIKE LOWER('%" + titre + "%')");

  qry.exec();
  Modal -> setQuery(qry);
  tableView_rest.setModel(Modal);
  db.close();
  QString strStream;
  QTextStream out( & strStream);

  const int rowCount = tableView_rest.model() -> rowCount();
  const int columnCount = tableView_rest.model() -> columnCount();

  const QString strTitle = "List of aids";
  out << "<html>\n"
  "<img src=':/Icons/Icons/Images/logo.png' height='108' width='250'/>"
  "<head>\n"
  "<meta Content=\"Text/html; charset=Windows-1251\">\n" <<
  QString("<title>%1</title>\n").arg(strTitle) <<
    "</head>\n"
  "<body bgcolor=#ffffff link=#5000A0>\n" <<
  QString("<h3 style=\" font-size: 50px; font-family: Arial, Helvetica, sans-serif; color: #e80e32; font-weight: lighter; text-align: center;\">%1</h3>\n").arg("LIST OF DONATIONS") <<
    "<br>"

    <<
    "<table border=1 cellspacing=0 cellpadding=2 width=\"100%\">\n";
  out << "<thead><tr bgcolor=#f0f0f0>";
  for (int column = 0; column < columnCount; column++)
    if (!tableView_rest.isColumnHidden(column))
      out << QString("<th>%1</th>").arg(tableView_rest.model() -> headerData(column, Qt::Horizontal).toString());
  out << "</tr></thead>\n";

  for (int row = 0; row < rowCount; row++) {
    out << "<tr>";
    for (int column = 0; column < columnCount; column++) {
      if (!tableView_rest.isColumnHidden(column)) {
        QString data = tableView_rest.model() -> data(tableView_rest.model() -> index(row, column)).toString().simplified();
        out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
      }
    }
    out << "</tr>\n";
  }
  out << "</table>\n"
  "<br><br>" <<
  "<br>" <<
  "<table border=1 cellspacing=0 cellpadding=2>\n";
  out << "<thead><tr bgcolor=#f0f0f0>";
  out << "</table>\n"
  "</body>\n"
  "</html>\n";
  QTextDocument * ListeDesRest = new QTextDocument();
  ListeDesRest -> setHtml(strStream);
  QPrinter printer;
  QPrintDialog * dialog = new QPrintDialog( & printer, NULL);
  if (dialog -> exec() == QDialog::Accepted) {
    ListeDesRest -> print( & printer);
  }
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setPaperSize(QPrinter::A4);
  printer.setOutputFileName("/tmp/Restaurants.pdf");
  printer.setPageMargins(QMarginsF(15, 15, 15, 15));
  delete ListeDesRest;
}

void MainWindow::on_aid_recherche_textChanged(const QString & arg1) {
  ui -> tableViewads -> setModel(ads.recherche(arg1));
}

void MainWindow::on_pushButton_16_clicked() {
  QModelIndexList indexList = ui->tableViewads -> selectionModel() -> selectedIndexes();

  QStringList rowData;
  foreach(QModelIndex index, indexList) {
    rowData << ui -> tableViewads -> model() -> data(index).toString();
  }

  QString url = QString("https://chart.googleapis.com/chart?cht=qr&chs=300x300&chl=%1").arg(rowData.join("\n ,"));
  QDesktopServices::openUrl(QUrl(url));
}

void MainWindow::on_pushButton_5_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(8);
  Aides ads;
  QGridLayout * StatsLayout1 = ui -> stackedWidget_2 -> findChild < QGridLayout * > ("gridLayout_2");
  StatsLayout1 -> addWidget(ads.stat_type(), 0, 0, 0);
}

void MainWindow::on_pushButton_4_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(9);

  QComboBox * comboBox_2update = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox_update_aid");
  comboBox_2update -> clear();
  comboBox_2update -> addItem("Select");
  QSqlQuery query, query1;
  query.prepare("SELECT ID_AIDE FROM AIDE");

  if (query.exec()) {
    while (query.next()) {
      int i = query.value(0).toInt();
      QString intString = QString::number(i);
      comboBox_2update -> addItem(intString);
    }
  }

  connect(comboBox_2update, QOverload < int > ::of( & QComboBox::currentIndexChanged), this, [ = ](int index) {

    QString selectedItemText = comboBox_2update -> currentText();
    int ID = selectedItemText.toInt();

    QSqlQuery query1;
    query1.prepare("SELECT ID_RES, DESCRIPTION_AIDE, DATE_AIDE , DONNEUR,  TYPE_AIDE FROM AIDE WHERE ID_AIDE = :id");
    query1.bindValue(":id", ID);
    qDebug() << "preparation completed ";
    if (query1.exec() && query1.next()) {

      QString description_aide = query1.value("DESCRIPTION_AIDE").toString();
      QString date_aide = query1.value("DATE_AIDE").toString();
      QString donneur = query1.value("DONNEUR").toString();
      QString type_aide = query1.value("TYPE_AIDE").toString();
      QString id_res = query1.value("ID_RES").toString();
      qDebug() << "retrieving completed";

      QLineEdit * lineEdit_descaide = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_descaide");
      lineEdit_descaide -> setText(description_aide);

      QDateEdit * dateEditaide = ui -> stackedWidget_2 -> findChild < QDateEdit * > ("dateEditaide");
      QDate date = QDate::fromString(date_aide, "ddd MMM d yyyy");
      dateEditaide -> setDate(date);

      QLineEdit * lineEdit_donnaide = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_donnaide");
      lineEdit_donnaide -> setText(donneur);

      QComboBox * comboBox_3type = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox_3type");
      comboBox_3type -> setCurrentText(type_aide);

      QLineEdit * lineEdi1000 = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdi1000");
      lineEdi1000 -> setText(id_res);
    }
  });
}

void MainWindow::on_btn_update_aid_clicked() {
  QLineEdit * lineEdit_descaide = ui -> stackedWidget -> findChild < QLineEdit * > ("lineEdit_descaide");
  QString description_aide = lineEdit_descaide -> text();

  QDateEdit * dateEditaide = ui -> stackedWidget -> findChild < QDateEdit * > ("dateEditaide");
  QString date_aide = dateEditaide -> date().toString();

  QLineEdit * lineEdit_donnaide = ui -> stackedWidget -> findChild < QLineEdit * > ("lineEdit_donnaide");
  QString donneur = lineEdit_donnaide -> text();

  QComboBox * comboBox_3type = ui -> stackedWidget -> findChild < QComboBox * > ("comboBox_3type");
  QString type_aide = comboBox_3type -> currentText();

  QLineEdit * lineEdi1000 = ui -> stackedWidget -> findChild < QLineEdit * > ("lineEdi1000");
  QString selectedItemText1 = lineEdi1000 -> text();
  int id_res = selectedItemText1.toInt();

  QComboBox * comboBox_2update = ui -> stackedWidget -> findChild < QComboBox * > ("comboBox_update_aid");
  QString selectedItemText = comboBox_2update -> currentText();
  int ID = selectedItemText.toInt();

  Aides a(ID, id_res, description_aide, date_aide, type_aide, donneur);

  bool test1 = a.modifier(ID);

  if (test1) {
    QTableView * Tableviewem = ui -> stackedWidget -> findChild < QTableView * > ("tableViewads");
    Tableviewem -> setModel(ads.afficher());
    QMessageBox::information(nullptr, QObject::tr("Updated!"),
      QObject::tr("The element is updated \n"
        "Click ok to exit."),
      QMessageBox::Ok);
  } else {
    QMessageBox::critical(nullptr, QObject::tr("Error"),
      QObject::tr("The element is refused \n"
        "Click ok to exit."),
      QMessageBox::Ok);
  }
}

//*************************Restaurant**********************************

void MainWindow::on_pushButton_23_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(11);
  QComboBox * Combonom = ui -> stackedWidget_2 -> findChild < QComboBox * > ("combo_r_nom");
  Combonom -> clear();
  Combonom -> addItem("Select a Restaurant");
  QSqlQuery query;
  query.prepare("SELECT NOM FROM RESTAURANT");
  if (query.exec()) {
    while (query.next()) {
      QString nom = query.value("NOM").toString();
      Combonom -> addItem(nom);
    }
  }
}

void MainWindow::on_combo_r_nom_currentIndexChanged(const QString & arg1) {
  QComboBox * Combonom = ui -> stackedWidget_2 -> findChild < QComboBox * > ("combo_r_nom");
  QString location = Combonom -> currentText();

  if (location == "Select a Restaurant") {
    ui -> axWidget -> dynamicCall("Navigate(const QString&)", "file:///C:/Users/Jihed/Downloads/2a2-2a2-share-and-care-center-mohamed-chehida/2a2-2a2-share-and-care-center-mohamed-chehida/website/index.html");
  } else {
    ui -> axWidget -> dynamicCall("Navigate(const QString&)", "https://www.google.com/maps?q=" + location);
  }
}

void MainWindow::on_pushButton_24_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(12);

  ui -> axWidget_2 -> dynamicCall("Navigate(const QString&)", "https://www.google.com/maps?q=Restaurants In Tunisia");
  ui -> Api_dataview -> setModel(nullptr);
  ui -> Api_dataview -> horizontalHeader() -> setStretchLastSection(true);
  ui -> Api_dataview -> setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui -> Api_dataview -> setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  QStandardItemModel * model = new QStandardItemModel(1, 5);

  model -> setHeaderData(0, Qt::Horizontal, "Title");
  model -> setHeaderData(1, Qt::Horizontal, "Website");
  model -> setHeaderData(2, Qt::Horizontal, "Email");
  model -> setHeaderData(3, Qt::Horizontal, "Address");
  model -> setHeaderData(4, Qt::Horizontal, "Phone");

  ui -> Api_dataview -> setModel(model);
}

void MainWindow::on_pushButton_25_clicked() {
  QVariant currentURL = ui -> axWidget_2 -> dynamicCall("LocationURL()");
  QString currentURLString = currentURL.toString();

  QString api_key = "113d6a718688c8a06ef67088e308dffe46cd0389383de9829a0daa5adc1a5c12";

  int nameStartIndex = currentURLString.indexOf("/place/") + 7;
  int nameEndIndex = currentURLString.indexOf("/", nameStartIndex);
  QString LocationName = currentURLString.mid(nameStartIndex, nameEndIndex - nameStartIndex).replace("+", " ");

  int cordsStartIndex = currentURLString.indexOf("/@") + 2;
  int cordsEndIndex = currentURLString.indexOf("/data");
  QString LocationCords = currentURLString.mid(cordsStartIndex, cordsEndIndex - cordsStartIndex);

  qDebug() << currentURLString;
  qDebug() << LocationName;
  qDebug() << LocationCords;

  QString url = "https://serpapi.com/search.json?engine=google_maps&q=" + LocationName + "&ll=@" + LocationCords + "&type=search&api_key=" + api_key;
  qDebug() << url;

  QNetworkAccessManager manager;
  QNetworkRequest request(url);

  QNetworkReply * reply = manager.get(request);

  QEventLoop loop;
  QObject::connect(reply, SIGNAL(finished()), & loop, SLOT(quit()));
  loop.exec();

  QJsonDocument response = QJsonDocument::fromJson(reply -> readAll());
  QJsonObject root = response.object();
  qDebug() << "===========================";

  qDebug() << (root.contains("local_results"));
  qDebug() << "===========================";

  qDebug() << (root.contains("place_results"));
  qDebug() << "===========================";

  if (root.contains("local_results")) {
    QJsonArray localResults = root["local_results"].toArray();
    QJsonObject result = localResults[0].toObject();

    QString title = result["title"].toString();
    QString address = result["address"].toString();

    QString phone;
    if (result.contains("phone")) {
      phone = result["phone"].toString();
    } else {
      phone = "";
    }

    QString website;
    if (result.contains("website")) {
      website = result["website"].toString();
    } else {
      website = "";
    }

    qDebug() << "Title: " << title;
    qDebug() << "Website: " << website;

    qDebug() << "Address: " << address;
    qDebug() << "Phone: " << phone;

    Restaurant r(0, title, website, NULL, address, phone);
    ui -> Api_dataview -> setModel(r.autofillTable(r));
  } else {
    qDebug() << "Case 2";

    QJsonObject placeResults = root["place_results"].toObject();

    qDebug() << placeResults.size();

    QString title = placeResults["title"].toString();
    QString address = placeResults["address"].toString();

    QString phone;
    if (placeResults.contains("phone")) {
      phone = placeResults["phone"].toString();
    } else {
      phone = "";
    }

    QString website;
    if (placeResults.contains("website")) {
      website = placeResults["website"].toString();
    } else {
      website = "";
    }

    qDebug() << "Title: " << title;
    qDebug() << "Website: " << website;

    qDebug() << "Address: " << address;
    qDebug() << "Phone: " << phone;

    Restaurant r(0, title, website, NULL, address, phone);

    ui -> Api_dataview -> setModel(r.autofillTable(r));
  }

  reply -> deleteLater();
}

void MainWindow::on_ConfirmApiData_clicked() {
  QStandardItemModel * model = qobject_cast < QStandardItemModel * > (ui -> Api_dataview -> model());

  QString name = model -> index(0, 0).data().toString();
  QString website = model -> index(0, 1).data().toString();
  QString email = model -> index(0, 2).data().toString();
  QString address = model -> index(0, 3).data().toString();
  QString phone = model -> index(0, 4).data().toString();

  qDebug() << "Name: " << name << ", Website: " << website << ", Email: " << email << ", Address: " << address << ", Phone: " << phone;

  Restaurant r(0, name, website, NULL, address, phone);
  bool test = r.ajouter_re();

  if (test) {
    QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_em");
    Tableviewem -> setModel(Etmp.afficher_em());
    QMessageBox::information(nullptr, QObject::tr("Accepted!"),
      QObject::tr("The element is added \n"
        "Click ok to exit."),
      QMessageBox::Ok);
    ui -> Api_dataview -> setModel(r.ClearTable());
  } else {
    QMessageBox::critical(nullptr, QObject::tr("Error"),
      QObject::tr("The element is refused \n"
        "Click ok to exit."),
      QMessageBox::Ok);
    ui -> Api_dataview -> setModel(r.ClearTable());
  }
}

void MainWindow::on_clear_api_clicked() {
  Restaurant r;
  ui -> Api_dataview -> setModel(r.ClearTable());
  ui -> axWidget_2 -> dynamicCall("Navigate(const QString&)", "https://www.google.com/maps?q=Restaurants In Tunisia");
}

void MainWindow::on_pushButton_26_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(10);
}

void MainWindow::on_pushButton_27_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(10);
}

void MainWindow::on_pushButton_18_clicked() {
  Restaurant r;
  QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_rest");
  Tableviewem -> setModel(r.afficher_re());

  ui -> stackedWidget_2 -> setCurrentIndex(13);

  QComboBox * Combodelete = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox_rest");
  Combodelete -> clear();

  QSqlQuery query;
  query.prepare("SELECT ID_RES FROM RESTAURANT");
  if (query.exec()) {
    while (query.next()) {
      int i = query.value(0).toInt();
      QString intString = QString::number(i);
      Combodelete -> addItem(intString);
    }
  }
}

void exportToPDFrest(QTableView * tableView) {

  QPrinter printer(QPrinter::HighResolution);

  printer.setPrinterName("Printer Name");

  QPrintDialog dialog( & printer, nullptr);
  if (dialog.exec() != QDialog::Accepted) {
    return;
  }

  QPainter painter( & printer);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
  double xscale = printer.pageRect().width() / double(tableView -> width());
  painter.scale(xscale, xscale);

  double ypos = (printer.pageRect().height() / xscale - tableView -> height()) / 1500;

  painter.translate(0, ypos);

  tableView -> render( & painter);

  painter.end();
}

void MainWindow::on_pdf_r_clicked() {
  QSqlDatabase db;

  QTableView tableView_rest;
  QSqlQueryModel * Modal = new QSqlQueryModel();
  QSqlQuery qry;

  QString titre = ui -> lineEdit_rechercheem_2 -> text().toLower();
  qry.prepare("select * from RESTAURANT WHERE LOWER(NOM) LIKE LOWER('%" + titre + "%')");

  qry.exec();
  Modal -> setQuery(qry);
  tableView_rest.setModel(Modal);
  db.close();
  QString strStream;
  QTextStream out( & strStream);

  const int rowCount = tableView_rest.model() -> rowCount();
  const int columnCount = tableView_rest.model() -> columnCount();

  const QString strTitle = "List of Restaurants";
  out << "<html>\n"
  "<img src=':/Icons/Icons/Images/logo.png' height='108' width='250'/>"
  "<head>\n"
  "<meta Content=\"Text/html; charset=Windows-1251\">\n" <<
  QString("<title>%1</title>\n").arg(strTitle) <<
    "</head>\n"
  "<body bgcolor=#ffffff link=#5000A0>\n" <<
  QString("<h3 style=\" font-size: 50px; font-family: Arial, Helvetica, sans-serif; color: #e80e32; font-weight: lighter; text-align: center;\">%1</h3>\n").arg("List of Restaurants") <<
    "<br>"

    <<
    "<table border=1 cellspacing=0 cellpadding=2 width=\"100%\">\n";
  out << "<thead><tr bgcolor=#f0f0f0>";
  for (int column = 0; column < columnCount; column++)
    if (!tableView_rest.isColumnHidden(column))
      out << QString("<th>%1</th>").arg(tableView_rest.model() -> headerData(column, Qt::Horizontal).toString());
  out << "</tr></thead>\n";

  for (int row = 0; row < rowCount; row++) {
    out << "<tr>";
    for (int column = 0; column < columnCount; column++) {
      if (!tableView_rest.isColumnHidden(column)) {
        QString data = tableView_rest.model() -> data(tableView_rest.model() -> index(row, column)).toString().simplified();
        out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
      }
    }
    out << "</tr>\n";
  }
  out << "</table>\n"
  "<br><br>" <<
  "<br>" <<
  "<table border=1 cellspacing=0 cellpadding=2>\n";
  out << "<thead><tr bgcolor=#f0f0f0>";
  out << "</table>\n"
  "</body>\n"
  "</html>\n";
  QTextDocument * ListeDesRest = new QTextDocument();
  ListeDesRest -> setHtml(strStream);
  QPrinter printer;
  QPrintDialog * dialog = new QPrintDialog( & printer, NULL);
  if (dialog -> exec() == QDialog::Accepted) {
    ListeDesRest -> print( & printer);
  }
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setPaperSize(QPrinter::A4);
  printer.setOutputFileName("/tmp/Restaurants.pdf");
  printer.setPageMargins(QMarginsF(15, 15, 15, 15));
  delete ListeDesRest;
}

void MainWindow::on_lineEdit_rechercheem_2_textChanged(const QString & arg1) {
  ui -> tableView_rest -> setModel(Rtmp.recherche_rest(arg1));
}

void MainWindow::on_del_rest_clicked() {
  QComboBox * Combodelete = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox_rest");
  QString selectedItemText = Combodelete -> currentText();
  int ID = selectedItemText.toInt();

  bool test1 = Rtmp.supprimer_re(ID);
  if (test1) {
    QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_rest");
    Tableviewem -> setModel(Rtmp.afficher_re());
    QMessageBox::information(nullptr, QObject::tr("Deleted!"),
      QObject::tr("The element is deleted \n"
        "Click ok to exit."),
      QMessageBox::Ok);

    QComboBox * Combodelete2 = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox_rest");
    Combodelete2 -> clear();

    QSqlQuery query;
    query.prepare("SELECT ID_RES FROM RESTAURANT");
    if (query.exec()) {
      while (query.next()) {
        int i = query.value(0).toInt();
        QString intString = QString::number(i);
        Combodelete2 -> addItem(intString);
      }
    }
  } else {
    QMessageBox::critical(nullptr, QObject::tr("Error"),
      QObject::tr("The element is refused \n"
        "Click ok to exit."),
      QMessageBox::Ok);
  }
}

void MainWindow::on_asc_clicked() {
  ui -> tableView_rest -> setModel(Rtmp.trierC());
}

void MainWindow::on_dec_clicked() {
  ui -> tableView_rest -> setModel(Rtmp.trierD());
}

void MainWindow::on_pushButton_28_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(10);
}

void MainWindow::on_pushButton_21_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(15);

  Restaurant r;
  QGridLayout * StatsLayout1 = ui -> stackedWidget_2 -> findChild < QGridLayout * > ("gridLayout_rest");
  StatsLayout1 -> addWidget(r.stat_rest(), 0, 0);
}

void MainWindow::on_back1_btn_9_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(10);
}

bool MainWindow::control_rest(int n) {

  QLineEdit * lineEdit_nom = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_nom_2");
  QLineEdit * lineEdit_web = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_web_2");
  QLineEdit * lineEdit_adr = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_adr_2");
  QLineEdit * lineEdit_tel = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_tel_2");
  QLineEdit * lineEdit_email = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_email_2");

  QLabel * error_rest_nom = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_rest_nom");
  QLabel * error_rest_web = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_rest_web");
  QLabel * error_rest_adr = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_rest_adr");
  QLabel * error_rest_tel = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_rest_tel");
  QLabel * error_rest_email = ui -> stackedWidget_2 -> findChild < QLabel * > ("error_rest_email");

  error_rest_nom -> clear();
  error_rest_nom -> hide();
  error_rest_web -> clear();
  error_rest_web -> hide();
  error_rest_adr -> clear();
  error_rest_adr -> hide();
  error_rest_tel -> clear();
  error_rest_tel -> hide();
  error_rest_email -> clear();
  error_rest_email -> hide();

  if (n == 1) {

    if ((lineEdit_nom -> text() == "") || (lineEdit_web -> text() == "") || (lineEdit_adr -> text() == "") || (lineEdit_tel -> text() == "") || (lineEdit_email -> text() == "")) {

      if (lineEdit_nom -> text() == "") {
        error_rest_nom -> show();
        error_rest_nom -> setText("Champs Obligatoire !");
      }

      if (lineEdit_web -> text() == "") {
        error_rest_web -> show();
        error_rest_web -> setText("Champs Obligatoire !");
      }

      if (lineEdit_adr -> text() == "") {
        error_rest_adr -> show();
        error_rest_adr -> setText("Champs Obligatoire !");
      }

      if (lineEdit_tel -> text() == "") {
        error_rest_tel -> show();
        error_rest_tel -> setText("Champs Obligatoire !");
      }

      if (lineEdit_email -> text() == "") {
        error_rest_email -> show();
        error_rest_email -> setText("Champs Obligatoire !");
      }

      return 0;
    } else

    if ((lineEdit_nom -> text().length() < 3) || !(lineEdit_web -> text().contains(".")) || (lineEdit_adr -> text().length() < 5) || !(lineEdit_email -> text().contains("@")) || !(lineEdit_email -> text().contains(".")) || (lineEdit_tel -> text().length() < 8)) {

      if (lineEdit_tel -> text().length() < 8) {
        error_rest_tel -> show();
        error_rest_tel -> setText("Doit Contenir 8 chiffres !");
      }

      if (lineEdit_adr -> text().length() < 10) {
        error_rest_adr -> show();
        error_rest_adr -> setText("Doit Contenir 5 chiffres !");
      }

      if (lineEdit_nom -> text().length() < 3) {
        error_rest_nom -> show();
        error_rest_nom -> setText("Doit Contenir 3 chiffres !");
      }

      if (!(lineEdit_email -> text().contains("@")) || !(lineEdit_email -> text().contains("."))) {
        error_rest_email -> show();
        error_rest_email -> setText("Forme Invalide ! (manque @ ou . )");
      }

      return 0;
    } else {

      return 1;
    }
  }
  return 1;
}
void MainWindow::on_add_rest_2_clicked() {
  QLineEdit * rest_nom_2 = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_nom_2");
  QString Nom = rest_nom_2 -> text();

  QLineEdit * rest_web_2 = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_web_2");
  QString Web = rest_web_2 -> text();

  QLineEdit * rest_adr_2 = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_adr_2");
  QString Adr = rest_adr_2 -> text();

  QLineEdit * rest_tel_2 = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_tel_2");
  QString Tel = rest_tel_2 -> text();

  QLineEdit * rest_email_2 = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_email_2");
  QString Mail = rest_email_2 -> text();

  Restaurant r(0, Nom, Web, Mail, Adr, Tel);

  if (control_rest(1)) {

    bool test = r.ajouter_re();
    if (test) {
      QMessageBox::information(nullptr, QObject::tr("Accepted!"),
        QObject::tr("The element is added \n"
          "Click ok to exit."),
        QMessageBox::Ok);
      QTableView * TableviewR = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_rest");
      TableviewR -> setModel(r.afficher_re());
      rest_nom_2 -> clear();
      rest_web_2 -> clear();
      rest_adr_2 -> clear();
      rest_tel_2 -> clear();
      rest_email_2 -> clear();
    } else {
      QMessageBox::critical(nullptr, QObject::tr("Error"),
        QObject::tr("The element is refused \n"
          "Click ok to exit."),
        QMessageBox::Ok);
    }
  }
}

void MainWindow::on_pushButton_20_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(16);
}

void MainWindow::on_back_re_2_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(10);
}

//*************List benif*******************
void MainWindow::on_pushButton_55_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(18);
  QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_em_34");
  Tableviewem -> verticalHeader() -> setDefaultSectionSize(40);
  Tableviewem -> horizontalHeader() -> setDefaultSectionSize(160);
  Tableviewem -> setModel(bn.afficher_benif());
  QComboBox * Combodelete = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox_53");
  Combodelete -> clear();
  Combodelete -> addItem("Select");
  QSqlQuery query;
  query.prepare("SELECT ID_BENIF FROM BENIFICIAIRE");
  if (query.exec()) {
    while (query.next()) {
      int i = query.value(0).toInt();
      QString intString = QString::number(i);
      Combodelete -> addItem(intString);
    }
  }
}

void MainWindow::on_pushButton_54_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(19);
}

//**control benif****************

bool MainWindow::control_benif(int n) {

  QLineEdit * lineEdit_nom_ass_16 = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_nom_ass_16");
  QLineEdit * lineEdit_nom_responsable_16 = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_nom_responsable_16");
  QLineEdit * lineEdit_adresse_ass_16 = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_adresse_ass_16");
  QLineEdit * lineEdit_email_ass_16 = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_email_ass_16");
  QLineEdit * lineEdit_telephone_ass_16 = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_telephone_ass_16");
  QLineEdit * lineEdit_agee = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_agee");

  QLabel * label_1 = ui -> stackedWidget_2 -> findChild < QLabel * > ("label_133");
  QLabel * label_2 = ui -> stackedWidget_2 -> findChild < QLabel * > ("label_132");
  QLabel * label_3 = ui -> stackedWidget_2 -> findChild < QLabel * > ("label_135");
  QLabel * label_4 = ui -> stackedWidget_2 -> findChild < QLabel * > ("label_131");
  QLabel * label_5 = ui -> stackedWidget_2 -> findChild < QLabel * > ("label_130");
  QLabel * label_6 = ui -> stackedWidget_2 -> findChild < QLabel * > ("label_134");

  if (n == 1) {
    if ((lineEdit_nom_ass_16 -> text() == "") || (lineEdit_nom_responsable_16 -> text() == "") || (lineEdit_adresse_ass_16 -> text() == "") || (lineEdit_email_ass_16 -> text() == "") || (lineEdit_telephone_ass_16 -> text() == "") || (lineEdit_agee -> text() == "")) {
      if (lineEdit_nom_ass_16 -> text() == "") {
        label_1 -> show();
        label_1 -> setText("Required field !");
      }

      if (lineEdit_nom_responsable_16 -> text() == "") {
        label_2 -> show();
        label_2 -> setText("Required field !");
      }

      if (lineEdit_adresse_ass_16 -> text() == "") {
        label_3 -> show();
        label_3 -> setText("Required field !");
      }

      if (lineEdit_email_ass_16 -> text() == "") {
        label_4 -> show();
        label_4 -> setText("Required field !");
      }

      if (lineEdit_telephone_ass_16 -> text() == "") {
        label_5 -> show();
        label_5 -> setText("Required field !");
      }

      if (lineEdit_agee -> text() == "") {
        label_5 -> show();
        label_6 -> setText("Required field !");
      }

      qDebug() << "suii1";

      return 0;
    } else if ((lineEdit_telephone_ass_16 -> text().length() < 12) || (lineEdit_telephone_ass_16 -> text().length() > 12) || !(lineEdit_telephone_ass_16 -> text().startsWith("+216")) || !(lineEdit_email_ass_16 -> text().contains("@")) || !(lineEdit_email_ass_16 -> text().contains(".")) || (lineEdit_nom_responsable_16 -> text().length() < 3) || (lineEdit_agee -> text().length() < 2) || (lineEdit_agee -> text().length() > 2) || (lineEdit_nom_ass_16 -> text().length() < 3)) {

      if (lineEdit_telephone_ass_16 -> text().length() < 12 || !(lineEdit_telephone_ass_16 -> text().startsWith("+216"))) {
        label_6 -> show();
        label_6 -> setText("Must contain 12 digits and starts with +216 !");
      }

      if (lineEdit_nom_ass_16 -> text().length() < 3) {
        label_1 -> show();
        label_1 -> setText("Must contain 3 caracters !");
      }

      if (lineEdit_nom_responsable_16 -> text().length() < 3) {
        label_2 -> show();
        label_2 -> setText("Must contain 3 caracters !");
      }

      if (lineEdit_agee -> text().length() < 2 || (lineEdit_agee -> text().length() > 2)) {
        label_3 -> show();
        label_3 -> setText("Must contain 2 digits !");
      }

      if (!(lineEdit_email_ass_16 -> text().contains("@")) || !(lineEdit_email_ass_16 -> text().contains("."))) {
        label_5 -> show();
        label_5 -> setText("Forme Invalide ! (manque @ ou . )");
      }
      qDebug() << "suii2";

      return 0;
    } else {
      qDebug() << "suii3";

      return 1;
    }
  }
  qDebug() << "suii4";
  return 1;
}

void MainWindow::on_pushButton_29_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(22);
  Benif b;
  QGridLayout * StatsLayout1 = ui -> stackedWidget_2 -> findChild < QGridLayout * > ("gridLayout");
  StatsLayout1 -> addWidget(b.stat_age(), 0, 0);
}

void MainWindow::on_pushButton_56_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(21);
  QComboBox * Comboupdate = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox_19");
  Comboupdate -> clear();
  Comboupdate -> addItem("Select");
  QSqlQuery query, query1;
  query.prepare("SELECT ID_BENIF FROM BENIFICIAIRE");

  if (query.exec()) {
    while (query.next()) {
      int i = query.value(0).toInt();
      QString intString = QString::number(i);
      Comboupdate -> addItem(intString);
    }
  }

  connect(Comboupdate, QOverload < int > ::of( & QComboBox::currentIndexChanged), this, [ = ](int index) {

    QString selectedItemText = Comboupdate -> currentText();
    int ID = selectedItemText.toInt();

    QSqlQuery query1;

    query1.prepare("SELECT NOM_ASS,NOM_RESPONSABLE,TELEPHONE_ASS FROM BENIFICIAIRE WHERE ID_BENIF = :id");
    query1.bindValue(":id", ID);
    qDebug() << "preparation completed ";
    if (query1.exec() && query1.next()) {

      QString nom_ass = query1.value("nom_ass").toString();
      QString nom_responsable = query1.value("nom_responsable").toString();
      QString telephone_ass = query1.value("telephone_ass").toString();

      qDebug() << "retrieving completed";

      QLineEdit * lineEdit_nom_ass = ui -> stackedWidget -> findChild < QLineEdit * > ("lineEdit_update_nom_ass_2");
      lineEdit_nom_ass -> setText(nom_ass);
      lineEdit_nom_ass -> update();

      QLineEdit * lineEdit_nom_responsable = ui -> stackedWidget -> findChild < QLineEdit * > ("lineEdit_nom_responsable_18");
      lineEdit_nom_responsable -> setText(nom_responsable);
      lineEdit_nom_responsable -> update();

      QLineEdit * lineEdit_telephone_ass = ui -> stackedWidget -> findChild < QLineEdit * > ("lineEdit_phone_21");
      lineEdit_telephone_ass -> setText(telephone_ass);
      lineEdit_telephone_ass -> update();

    }
  });
}

void MainWindow::on_back1_btn_157_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(17);
}

void MainWindow::on_radioButton_5_clicked() {
  ui -> tableView_em_34 -> setModel(bn.trierC());
}

void MainWindow::on_radioButton_6_clicked() {
  ui -> tableView_em_34 -> setModel(bn.trierD());
}

void MainWindow::on_back_update_aiddash_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(5);
}

void MainWindow::on_back_list_benifdash_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(17);
}

void MainWindow::on_pushButton_22_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(14);

  QComboBox * Comboupdate = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox_7");
  Comboupdate -> clear();
  Comboupdate -> addItem("Select");
  QSqlQuery query, query1;
  query.prepare("SELECT ID_RES FROM RESTAURANT");

  if (query.exec()) {
    while (query.next()) {
      int i = query.value(0).toInt();
      QString intString = QString::number(i);
      Comboupdate -> addItem(intString);
    }
  }

  connect(Comboupdate, QOverload < int > ::of( & QComboBox::currentIndexChanged), this, [ = ](int index) {

    QString selectedItemText = Comboupdate -> currentText();
    int ID = selectedItemText.toInt();

    QSqlQuery query1;

    query1.prepare("SELECT * FROM RESTAURANT WHERE ID_RES = :id");
    query1.bindValue(":id", ID);
    qDebug() << "preparation completed ";
    if (query1.exec() && query1.next()) {

      QString res_nom = query1.value("NOM").toString();
      QString res_web = query1.value("WEBSITE").toString();
      QString res_adr = query1.value("ADRESSE").toString();
      QString res_tel = query1.value("TELEPHONE_RES").toString();
      QString res_email = query1.value("EMAIL").toString();

      qDebug() << "retrieving completed";

      QLineEdit * lineEdit_nomr = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_nom_3");
      lineEdit_nomr -> setText(res_nom);
      lineEdit_nomr -> update();

      QLineEdit * lineEdit_webr = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_web_3");
      lineEdit_webr -> setText(res_web);
      lineEdit_webr -> update();

      QLineEdit * lineEdit_adresse = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_adr_3");
      lineEdit_adresse -> setText(res_adr);

      QLineEdit * lineEdit_tel = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_tel_3");
      lineEdit_tel -> setText(res_tel);

      QLineEdit * lineEdit_email = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_email_3");
      lineEdit_email -> setText(res_email);

    }
  });
}

void MainWindow::on_update_res_clicked() {
  QLineEdit * lineEdit_nomr = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_nom_3");
  QString nomr = lineEdit_nomr -> text();

  QLineEdit * lineEdit_webr = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_web_3");
  QString webr = lineEdit_webr -> text();

  QLineEdit * lineEdit_adresse = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_adr_3");
  QString adr_rest = lineEdit_adresse -> text();

  QLineEdit * lineEdit_tel = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_tel_3");
  QString telR = lineEdit_tel -> text();

  QLineEdit * lineEdit_email = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("rest_email_3");
  QString emailR = lineEdit_email -> text();

  QComboBox * Comboupdate = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox_7");
  QString selectedItemText = Comboupdate -> currentText();
  int ID = selectedItemText.toInt();

  Restaurant r(ID, nomr, webr, emailR, adr_rest, telR);

  bool test1 = r.modifier_re(ID);

  if (test1) {
    QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_rest");
    Tableviewem -> setModel(Rtmp.afficher_re());
    QMessageBox::information(nullptr, QObject::tr("Updated!"),
      QObject::tr("The element is updated \n"
        "Click ok to exit."),
      QMessageBox::Ok);
  } else {
    QMessageBox::critical(nullptr, QObject::tr("Error"),
      QObject::tr("The element is refused \n"
        "Click ok to exit."),
      QMessageBox::Ok);
  }
}

void MainWindow::on_back1_btn_8_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(10);
  QStringList lineEditNames = {
    "rest_adr_3",
    "rest_email_3",
    "rest_nom_3",
    "rest_tel_3",
    "rest_web_3"
  };

  for (const QString & name: lineEditNames) {
    QLineEdit * lineEdit = ui -> stackedWidget_2 -> findChild < QLineEdit * > (name);
    if (lineEdit) {
      lineEdit -> clear();
    }
  }
}

void MainWindow::on_pushButton_349_clicked() {
  if (control_benif(1)) {
    QLineEdit * lineEdit_nom_ass = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_nom_ass_16");
    QString nom_ass = lineEdit_nom_ass -> text();

    QLineEdit * lineEdit_nom_responsable = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_nom_responsable_16");
    QString nom_responsable = lineEdit_nom_responsable -> text();

    QLineEdit * lineEdit_adresse_ass = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_adresse_ass_16");
    QString adresse_ass = lineEdit_adresse_ass -> text();

    QLineEdit * lineEdit_telephone_ass = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_telephone_ass_16");
    QString telephone_ass = lineEdit_telephone_ass -> text();

    QLineEdit * lineEdit_email_ass = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_email_ass_16");
    QString email_ass = lineEdit_email_ass -> text();

    QLineEdit * lineEdit_agee = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_agee");
    QString age = lineEdit_agee -> text();

    Benif b(0, nom_ass, adresse_ass, nom_responsable, telephone_ass, email_ass, age);

    bool test = b.ajouter_benif();

    if (test) {
      QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_em_34");
      Tableviewem -> setModel(b.afficher_benif());
      QMessageBox::information(nullptr, QObject::tr("Accepted!"),
        QObject::tr("The element is added \n"
          "Click ok to exit."),
        QMessageBox::Ok);
    } else {
      QMessageBox::critical(nullptr, QObject::tr("Error"),
        QObject::tr("The element is refused \n"
          "Click ok to exit."),
        QMessageBox::Ok);
    }
  }
}

// delete benif
void MainWindow::on_pushButton_304_clicked() {
  QComboBox * Combodelete = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox_53");
  Combodelete -> addItem("Select");

  QString selectedItemText = Combodelete -> currentText();
  int ID = selectedItemText.toInt();

  bool test1 = bn.supprimer_benif(ID);
  if (test1) {
    QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_em_34");
    Tableviewem -> setModel(bn.afficher_benif());
    QMessageBox::information(nullptr, QObject::tr("Deleted!"),
      QObject::tr("The element is deleted \n"
        "Click ok to exit."),
      QMessageBox::Ok);
  } else {
    QMessageBox::critical(nullptr, QObject::tr("Error"),
      QObject::tr("The element is refused \n"
        "Click ok to exit."),
      QMessageBox::Ok);
  }
}
// benif pdf
void MainWindow::on_pushButton_364_clicked() {
  QSqlDatabase db;

  QTableView tableView_rest;
  QSqlQueryModel * Modal = new QSqlQueryModel();
  QSqlQuery qry;

  QLineEdit * aid_recherche = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_recherchebn");
  QString titre = aid_recherche -> text().toLower();

  qry.prepare("select * from BENIFICIAIRE WHERE LOWER(NOM_ASS) LIKE LOWER('%" + titre + "%')");

  qry.exec();
  Modal -> setQuery(qry);
  tableView_rest.setModel(Modal);
  db.close();
  QString strStream;
  QTextStream out( & strStream);

  const int rowCount = tableView_rest.model() -> rowCount();
  const int columnCount = tableView_rest.model() -> columnCount();

  const QString strTitle = "List of aids";
  out << "<html>\n"
  "<img src=':/Icons/Icons/Images/logo.png' height='108' width='250'/>"
  "<head>\n"
  "<meta Content=\"Text/html; charset=Windows-1251\">\n" <<
  QString("<title>%1</title>\n").arg(strTitle) <<
    "</head>\n"
  "<body bgcolor=#ffffff link=#5000A0>\n" <<
  QString("<h3 style=\" font-size: 50px; font-family: Arial, Helvetica, sans-serif; color: #e80e32; font-weight: lighter; text-align: center;\">%1</h3>\n").arg("Beneficiary List") <<
    "<br>"

    <<
    "<table border=1 cellspacing=0 cellpadding=2 width=\"100%\">\n";
  out << "<thead><tr bgcolor=#f0f0f0>";
  for (int column = 0; column < columnCount; column++)
    if (!tableView_rest.isColumnHidden(column))
      out << QString("<th>%1</th>").arg(tableView_rest.model() -> headerData(column, Qt::Horizontal).toString());
  out << "</tr></thead>\n";

  for (int row = 0; row < rowCount; row++) {
    out << "<tr>";
    for (int column = 0; column < columnCount; column++) {
      if (!tableView_rest.isColumnHidden(column)) {
        QString data = tableView_rest.model() -> data(tableView_rest.model() -> index(row, column)).toString().simplified();
        out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
      }
    }
    out << "</tr>\n";
  }
  out << "</table>\n"
  "<br><br>" <<
  "<br>" <<
  "<table border=1 cellspacing=0 cellpadding=2>\n";
  out << "<thead><tr bgcolor=#f0f0f0>";
  out << "</table>\n"
  "</body>\n"
  "</html>\n";
  QTextDocument * ListeDesRest = new QTextDocument();
  ListeDesRest -> setHtml(strStream);
  QPrinter printer;
  QPrintDialog * dialog = new QPrintDialog( & printer, NULL);
  if (dialog -> exec() == QDialog::Accepted) {
    ListeDesRest -> print( & printer);
  }
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setPaperSize(QPrinter::A4);
  printer.setOutputFileName("/tmp/Restaurants.pdf");
  printer.setPageMargins(QMarginsF(15, 15, 15, 15));
  delete ListeDesRest;
}
// benif search
void MainWindow::on_lineEdit_recherchebn_textChanged(const QString & arg1) {
  ui -> tableView_em_34 -> setModel(bn.recherche_bn(arg1));
}
// update benif
void MainWindow::on_pushButton_58_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(20);

  QComboBox * Comboupdate = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox_18");
  Comboupdate -> clear();
  Comboupdate -> addItem("Select");
  QSqlQuery query, query1;
  query.prepare("SELECT ID_BENIF FROM BENIFICIAIRE");

  if (query.exec()) {
    while (query.next()) {
      int i = query.value(0).toInt();
      QString intString = QString::number(i);
      Comboupdate -> addItem(intString);
    }
  }

  connect(Comboupdate, QOverload < int > ::of( & QComboBox::currentIndexChanged), this, [ = ](int index) {

    QString selectedItemText = Comboupdate -> currentText();
    int ID = selectedItemText.toInt();

    QSqlQuery query1;

    query1.prepare("SELECT NOM_ASS, ADRESSE_ASS, NOM_RESPONSABLE, TELEPHONE_ASS, EMAIL_ASS, AGE FROM BENIFICIAIRE WHERE ID_BENIF = :id");
    query1.bindValue(":id", ID);
    qDebug() << "preparation completed ";
    if (query1.exec() && query1.next()) {

      QString nom_ass = query1.value("nom_ass").toString();
      QString adresse_ass = query1.value("adresse_ass").toString();
      QString nom_responsable = query1.value("nom_responsable").toString();
      QString telephone_ass = query1.value("telephone_ass").toString();
      QString email_ass = query1.value("email_ass").toString();
      QString age = query1.value("age").toString();

      qDebug() << "retrieving completed";

      QLineEdit * lineEdit_nom_ass = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_update_nom_ass");
      lineEdit_nom_ass -> setText(nom_ass);
      lineEdit_nom_ass -> update();

      QLineEdit * lineEdit_adresse_ass = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_update_adresse_ass");
      lineEdit_adresse_ass -> setText(adresse_ass);
      lineEdit_adresse_ass -> update();

      QLineEdit * lineEdit_nom_responsable = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_update_nom_responsable");
      lineEdit_nom_responsable -> setText(nom_responsable);
      lineEdit_nom_responsable -> update();

      QLineEdit * lineEdit_telephone_ass = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_update_telephone_ass");
      lineEdit_telephone_ass -> setText(telephone_ass);
      lineEdit_telephone_ass -> update();
      QLineEdit * lineEdit_email_ass = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_update_email_ass");
      lineEdit_email_ass -> setText(email_ass);

      lineEdit_email_ass -> update();

      QLineEdit * lineEdit_agee = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_update_agee");
      lineEdit_agee -> setText(age);

      lineEdit_agee -> update();

    }
  });
}

void MainWindow::on_back1_btn_140_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(17);
}

void MainWindow::on_pushButton_30_clicked() {
  QLineEdit * lineEdit_nom_ass = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_update_nom_ass");
  QString nom_ass = lineEdit_nom_ass -> text();

  QLineEdit * lineEdit_adresse_ass = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_update_adresse_ass");
  QString adresse_ass = lineEdit_adresse_ass -> text();

  QLineEdit * lineEdit_nom_responsable = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_update_nom_responsable");
  QString nom_responsable = lineEdit_nom_responsable -> text();

  QLineEdit * lineEdit_telephone_ass = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_update_telephone_ass");
  QString telephone_ass = lineEdit_telephone_ass -> text();

  QLineEdit * lineEdit_email_ass = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_update_email_ass");
  QString email_ass = lineEdit_email_ass -> text();

  QLineEdit * lineEdit_agee = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_update_agee");
  QString age = lineEdit_agee -> text();

  QComboBox * Comboupdate = ui -> stackedWidget_2 -> findChild < QComboBox * > ("comboBox_18");
  QString selectedItemText = Comboupdate -> currentText();
  int ID = selectedItemText.toInt();
  Benif b(ID, nom_ass, adresse_ass, nom_responsable, telephone_ass, email_ass, age);

  bool test4 = b.modifier_benif(ID);

  if (test4) {
    QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_em_34");
    Tableviewem -> setModel(b.afficher_benif());
    QMessageBox::information(nullptr, QObject::tr("Updated!"),
      QObject::tr("The element is updated \n"
        "Click ok to exit."),
      QMessageBox::Ok);
  } else {
    QMessageBox::critical(nullptr, QObject::tr("Error"),
      QObject::tr("The element is refused \n"
        "Click ok to exit."),
      QMessageBox::Ok);
  }
}

void MainWindow::on_backzzzz_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(17);
}

void MainWindow::on_pushButton_57_clicked() {
  bn.generer();
}

void MainWindow::on_pushButton_305_clicked() {
  QLineEdit * phone = ui -> stackedWidget -> findChild < QLineEdit * > ("lineEdit_phone_21");
  QString tel = phone -> text();
  qDebug() << tel;

  QComboBox * comboo = ui -> stackedWidget -> findChild < QComboBox * > ("comboBox_24");
  int index = comboo -> currentIndex();

  switch (index) {
  case 0:
    QMessageBox::information(this, "Please select", "Please select an option from the list.");
    break;
  case 1:
    postrequest("Hello,\Your package is on its way", tel);
    break;
  case 2:
    postrequest("Hello,\Your package is delivered", tel);
    break;
  default:
    break;
  }
}

bool MainWindow::postrequest(QString smsmsg, QString phonenumber) {
  bool tried = false;
  QEventLoop eventLoop;

  QNetworkAccessManager mgr;
  QObject::connect( & mgr, SIGNAL(finished(QNetworkReply * )), & eventLoop, SLOT(quit()));

  QNetworkRequest req(QUrl(QString("https://api.orange.com/smsmessaging/v1/outbound/tel%3A%2B21651416440/requests")));

  req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  QString api = "Bearer " + apikey;
  req.setRawHeader("Authorization", api.toUtf8());
  QJsonObject msg;
  msg["message"] = smsmsg;
  QJsonObject obj;

  obj["address"] = "tel:" + phonenumber;
  obj["senderAddress"] = "tel:+21651416440";

  obj["outboundSMSTextMessage"] = msg;
  QJsonObject body;
  body["outboundSMSMessageRequest"] = obj;
  QJsonDocument doc(body);
  QByteArray data = doc.toJson();
  QNetworkReply * reply = mgr.post(req, data);
  eventLoop.exec();

  if (reply -> error() == QNetworkReply::NoError) {
    qDebug() << "Success";
    delete reply;
    return true;
  } else {
    qDebug() << "Failure" << reply -> error();
    if (reply -> errorString() == "Host requires authentication") {
      if (!tried) {
        apikey = QInputDialog::getText(0, "API Key dialog",
          "API Key expired, Set a new one :", QLineEdit::Normal,
          "", nullptr);
        if (postrequest(smsmsg, phonenumber)) {
          return true;
        }
      }
      tried = true;
    }
    delete reply;
    return false;
  }
}

void MainWindow::on_back1_btn_135_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(17);
}

void MainWindow::on_pushButton_31_clicked() {
  ui -> stackedWidget_2 -> setCurrentIndex(29);
}

void MainWindow::on_lineEdit_rechercheem_textChanged(const QString & arg1) {
  Employee e;
  QTableView * Tableviewem = ui -> stackedWidget_2 -> findChild < QTableView * > ("tableView_em");
  Tableviewem -> setModel(e.trier_em(arg1));
}

void MainWindow::on_button_employee_clicked()
{
    ui -> stackedWidget_2 -> setCurrentIndex(1);

}

void MainWindow::on_button_restaurants_clicked()
{
    ui -> stackedWidget_2 -> setCurrentIndex(10);

}

void MainWindow::on_button_benif_clicked()
{
    ui -> stackedWidget_2 -> setCurrentIndex(17);

}

void MainWindow::on_button_donations_clicked()
{
    ui -> stackedWidget_2 -> setCurrentIndex(5);

}

void MainWindow::on_button_events_clicked()
{
    ui -> stackedWidget_2 -> setCurrentIndex(23);

}



//**********************************Events***************************
void MainWindow::on_lineEdit_rechercheev_textChanged(const QString &arg1)
{
    ui -> tableView_ev -> setModel(ev.recherche_event(arg1));

}

void MainWindow::on_pushButton_32_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(25);
    QTableView *Tableviewev = ui->stackedWidget_2->findChild<QTableView*>("tableView_ev");
    Tableviewev->setModel(ev.afficher_event());





    QComboBox *Combodelete = ui->stackedWidget_2->findChild<QComboBox*>("comboBox_ev");
    Combodelete->clear();

    QSqlQuery query;
    query.prepare("SELECT ID_EVENT FROM EVENEMENT");
    // Executing the query and fetching the ID_EVENT from the result set
    if (query.exec()) {
        while (query.next()) {
            int i=query.value(0).toInt();
            // Convert the ID_EVENT to a string format
            QString intString = QString::number(i);
            // Add the string representation of the ID_EVENT to the combobox
            Combodelete->addItem(intString);
        }
    }
}

void MainWindow::on_pushButton_15_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(23);
}

void MainWindow::on_delete_btn_ev_clicked()
{
    //Retrieving the date from the comboBox and putting in in the ID
    QComboBox *Combodelete = ui->stackedWidget_2->findChild<QComboBox*>("comboBox_ev");
    QString selectedItemText = Combodelete->currentText();
    int ID = selectedItemText.toInt();

    bool test1 = ev.supprimer_event(ID);
    if(test1){
        QTableView *Tableviewem = ui->stackedWidget_2->findChild<QTableView*>("tableView_ev");
        Tableviewem->setModel(ev.afficher_event());
        QMessageBox::information(nullptr, QObject::tr("Deleted!"),
                                 QObject::tr("The element is deleted \n"
                                            "Click ok to exit."), QMessageBox::Ok);
    }
    else{
        QMessageBox::critical(nullptr, QObject::tr("Error"),
                                 QObject::tr("The element is refused \n"
                                            "Click ok to exit."), QMessageBox::Ok);
    }

}


//PDF EVENT
void MainWindow::on_pushButton_36_clicked()
{
    QSqlDatabase db;

    QTableView tableView_rest;
    QSqlQueryModel * Modal = new QSqlQueryModel();
    QSqlQuery qry;

    QLineEdit * aid_recherche = ui -> stackedWidget_2 -> findChild < QLineEdit * > ("lineEdit_rechercheev");
    QString titre = aid_recherche -> text().toLower();

    qry.prepare("select * from EVENEMENT WHERE LOWER(NOM_EVENT) LIKE LOWER('%" + titre + "%')");

    qry.exec();
    Modal -> setQuery(qry);
    tableView_rest.setModel(Modal);
    db.close();
    QString strStream;
    QTextStream out( & strStream);

    const int rowCount = tableView_rest.model() -> rowCount();
    const int columnCount = tableView_rest.model() -> columnCount();

    const QString strTitle = "List of Events";
    out << "<html>\n"
    "<img src=':/Icons/Icons/Images/logo.png' height='108' width='250'/>"
    "<head>\n"
    "<meta Content=\"Text/html; charset=Windows-1251\">\n" <<
    QString("<title>%1</title>\n").arg(strTitle) <<
      "</head>\n"
    "<body bgcolor=#ffffff link=#5000A0>\n" <<
    QString("<h3 style=\" font-size: 50px; font-family: Arial, Helvetica, sans-serif; color: #e80e32; font-weight: lighter; text-align: center;\">%1</h3>\n").arg("Beneficiary List") <<
      "<br>"

      <<
      "<table border=1 cellspacing=0 cellpadding=2 width=\"100%\">\n";
    out << "<thead><tr bgcolor=#f0f0f0>";
    for (int column = 0; column < columnCount; column++)
      if (!tableView_rest.isColumnHidden(column))
        out << QString("<th>%1</th>").arg(tableView_rest.model() -> headerData(column, Qt::Horizontal).toString());
    out << "</tr></thead>\n";

    for (int row = 0; row < rowCount; row++) {
      out << "<tr>";
      for (int column = 0; column < columnCount; column++) {
        if (!tableView_rest.isColumnHidden(column)) {
          QString data = tableView_rest.model() -> data(tableView_rest.model() -> index(row, column)).toString().simplified();
          out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
        }
      }
      out << "</tr>\n";
    }
    out << "</table>\n"
    "<br><br>" <<
    "<br>" <<
    "<table border=1 cellspacing=0 cellpadding=2>\n";
    out << "<thead><tr bgcolor=#f0f0f0>";
    out << "</table>\n"
    "</body>\n"
    "</html>\n";
    QTextDocument * ListeDesRest = new QTextDocument();
    ListeDesRest -> setHtml(strStream);
    QPrinter printer;
    QPrintDialog * dialog = new QPrintDialog( & printer, NULL);
    if (dialog -> exec() == QDialog::Accepted) {
      ListeDesRest -> print( & printer);
    }
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName("/tmp/Events.pdf");
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));
    delete ListeDesRest;
}

void MainWindow::on_pushButton_35_clicked()
{
    ui -> stackedWidget_2 -> setCurrentIndex(26);

    QGridLayout * StatsLayout1 = ui -> stackedWidget_2 -> findChild < QGridLayout * > ("eventGrid");
    StatsLayout1 -> addWidget(ev.stat_type(), 0, 0);
}

void MainWindow::on_backEventStat_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(23);
}

void MainWindow::on_pushButton_addevent_clicked()
{
    QLineEdit *lineEdit_name_event = ui->stackedWidget_2->findChild<QLineEdit*>("lineEdit_name_event");
    QString name_event = lineEdit_name_event->text();

    QLineEdit *lineEdit_horaire_event = ui->stackedWidget_2->findChild<QLineEdit*>("lineEdit_horaire_event");
    QString horaire_event = lineEdit_horaire_event->text();

    QLineEdit *lineEdit_type_event = ui->stackedWidget_2->findChild<QLineEdit*>("lineEdit_type_event");
    QString type_event = lineEdit_type_event->text();

    QDateEdit *dateEdit_event = ui->stackedWidget_2->findChild<QDateEdit*>("dateEdit_event");
    QString date_event = dateEdit_event->date().toString();

    QLineEdit *lineEdit_entry_price = ui->stackedWidget_2->findChild<QLineEdit*>("entry_price_1");
    int entry_price_str = lineEdit_entry_price->text().toInt(); // Get the value as a QString

    QLineEdit *lineEdit_participant_number = ui->stackedWidget_2->findChild<QLineEdit*>("participant_number_1");
    int participant_number_str = lineEdit_participant_number->text().toInt(); // Get the value as a QString

    Event ev(0, name_event, date_event, horaire_event, type_event, entry_price_str, participant_number_str); // Pass the retrieved values as arguments
        bool test = ev.ajouter_event();

        if (test) {
            QTableView *Tableviewev = ui->stackedWidget_2->findChild<QTableView*>("tableView_ev");
            Tableviewev->setModel(ev.afficher_event());
            QMessageBox::information(nullptr, QObject::tr("Accepted!"),
                                     QObject::tr("The element is added \n"
                                                "Click ok to exit."), QMessageBox::Ok);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Error"),
                                     QObject::tr("The element is refused \n"
                                                "Click ok to exit ."), QMessageBox::Ok);
        }

}

void MainWindow::on_pushButton_33_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(24);

    QComboBox *Comboupdate = ui->stackedWidget_2->findChild<QComboBox*>("comboBox_edit_event");
    Comboupdate->clear();
    Comboupdate->addItem("Select");
    QSqlQuery query, query1;
    query.prepare("SELECT ID_EVENT FROM EVENEMENT");

    // Executing the query and fetching the ID_EVENT from the result set
    if (query.exec()) {
        while (query.next()) {
            int i=query.value(0).toInt();
            // Convert the ID_EVENT to a string format
            QString intString = QString::number(i);
            // Add the string representation of the ID_EVENT to the

            Comboupdate->addItem(intString);
        }
    }

    connect(Comboupdate, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){

    QString selectedItemText = Comboupdate->currentText();
    int ID = selectedItemText.toInt();



    // Prepare the SQL query to fetch the data for the selected ID
    QSqlQuery query1;

    query1.prepare("SELECT NOM_EVENT,DATE_EVENT,HORAIRE_EVENT,TYPE_EVENT,PRICE,PARTICIPANT_NUMBER FROM EVENEMENT WHERE ID_EVENT = :id");
    query1.bindValue(":id", ID);
    qDebug() << "preparation completed ";
    if (query1.exec() && query1.next()) {

        // Retrieve the values for each column
        QString nom_event = query1.value("NOM_EVENT").toString();
        QString type_event = query1.value("TYPE_EVENT").toString();
        QString horaire_event = query1.value("HORAIRE_EVENT").toString();
        QString Date_event = query1.value("DATE_EVENT").toString();
        int entry_price = query1.value("PRICE").toInt();
        int participant_number = query1.value("PARTICIPANT_NUMBER").toInt();

        qDebug() << "retrieving completed";

        // Populate the QLineEdit widgets with the retrieved values
        QLineEdit *lineEdit_name_event = ui->stackedWidget_2->findChild<QLineEdit*>("event_name_edit");
        lineEdit_name_event->setText(nom_event);

        QLineEdit *lineEdit_event_type_edit= ui->stackedWidget_2->findChild<QLineEdit*>("event_type_edit");
        lineEdit_event_type_edit->setText(type_event);

        QLineEdit *lineEdit_event_time_edit = ui->stackedWidget_2->findChild<QLineEdit*>("event_time_edit");
        lineEdit_event_time_edit->setText(horaire_event);

       QDateEdit *lineEdit_dateEdit_event = ui->stackedWidget_2->findChild<QDateEdit*>("dateEdit_event_2");
       QDate date = QDate::fromString(Date_event, "ddd MMM d yyyy");
       lineEdit_dateEdit_event -> setDate(date);


        QLineEdit *lineEdit_entry_price = ui->stackedWidget_2->findChild<QLineEdit*>("entry_price_2");
        QString entry_price_str = QString::number(entry_price); // Convert integer to QString

        lineEdit_entry_price->setText(entry_price_str); // Set the QString value to the QLineEdit widget

        QLineEdit *lineEdit_participant_number = ui->stackedWidget_2->findChild<QLineEdit*>("participant_number_2");
        QString participant_number_str = QString::number(participant_number); // Convert integer to QString
        lineEdit_participant_number->setText(participant_number_str); // Set the QString value to the QLineEdit widget
    }
    });
}

void MainWindow::on_pushButton_51_clicked()
{
    QLineEdit *event_name_edit = ui->stackedWidget_2->findChild<QLineEdit*>("event_name_edit");
    QString name_event = event_name_edit->text();

    QLineEdit *event_type_edit = ui->stackedWidget_2->findChild<QLineEdit*>("event_type_edit");
    QString type_event = event_type_edit->text();

    QLineEdit *event_time_edit = ui->stackedWidget_2->findChild<QLineEdit*>("event_time_edit");
    QString horaire_event= event_time_edit->text();

    QDateEdit *dateEdit_event = ui->stackedWidget_2->findChild<QDateEdit*>("dateEdit_event_2");
    QString Date_event = dateEdit_event->date().toString();

    QLineEdit *lineEditentry_price = ui->stackedWidget_2->findChild<QLineEdit*>("entry_price_2");
    int entry_price = lineEditentry_price->text().toInt();

    QLineEdit *lineEditparticipant_number = ui->stackedWidget_2->findChild<QLineEdit*>("participant_number_2");
    int participant_number = lineEditparticipant_number->text().toInt();

    QComboBox *comboupdate = ui->stackedWidget_2->findChild<QComboBox*>("comboBox_edit_event");
    QString selectedItemText = comboupdate->currentText();
    int ID = selectedItemText.toInt();
     Event ee(ID ,name_event , Date_event, horaire_event, type_event , entry_price , participant_number);

    bool test1 = ee.modifier_event(ID);

    if(test1){
        QTableView *Tableviewev = ui->stackedWidget_2->findChild<QTableView*>("tableView_ev");
        Tableviewev->setModel(ev.afficher_event());
        QMessageBox::information(nullptr, QObject::tr("Updated!"),
                                 QObject::tr("The element is updated \n"
                                            "Click ok to exit."), QMessageBox::Ok);
    }
    else{
        QMessageBox::critical(nullptr, QObject::tr("Error"),
                                 QObject::tr("The element is refused \n"
                                            "Click ok to exit."), QMessageBox::Ok);
    }
}

void MainWindow::on_back21_btn_8_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(23);

}

void MainWindow::on_back1_btn_5_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(23);

}

void MainWindow::on_chat_btn_clicked()
{
    MessengerServer server;
       if (!server.startServer(3333))
       {
           qDebug() << "Error:" << server.errorString();
       }
       qDebug() << "Server Started....";

       MessengerLANClient *m = new MessengerLANClient(); // Create an instance of the window dynamically
       m->show();

}

void MainWindow::on_calculator_btn_clicked()
{
    calculator *c = new calculator();
    c->show();
}

void MainWindow::on_tools_btn_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(28);
}


void MainWindow::on_pushButton_34_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(27);
}

void MainWindow::on_back_stats_dash18_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(5);

}

void MainWindow::on_pushButton_18887_clicked()
{
    QTableView *tableView_temp = ui->stackedWidget_2->findChild<QTableView*>("tableView_temp");
       tableView_temp->setModel(A.afficher_temperature());
}

void MainWindow::on_pushButton_ard_dash_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
}
