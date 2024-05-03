#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "employee.h"
#include "aides.h"
#include "restaurant.h"
#include "benif.h"
#include "event.h"
#include "arduino.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_Login_btn_clicked();

    void on_forget_btn_clicked();

    void on_forget_btn_2_clicked();

    void on_pushButton_8_clicked();

    void on_back1_btn_3_clicked();

    void on_back1_btn_2_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_back1_btn_7_clicked();

    void on_update_em_button_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_12_clicked();

    void on_back1_btn_4_clicked();

    bool control(int n);

    bool control_benif(int n);

    void on_pdf_employee_clicked();

    void on_Recover_btn_clicked();

    void on_Login_btn_2_clicked();

    void on_Recover_btn_2_clicked();

    void on_forget_btn_3_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_19_clicked();

    void on_back_aid_dash_clicked();

    void on_food_btn_clicked();

    void on_money_btn_clicked();

    void on_dress_btn_clicked();

    void on_Delete_aid_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_pdf_aid_clicked();

    void on_aid_recherche_textChanged(const QString &arg1);

    void on_pushButton_16_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_23_clicked();

    void on_combo_r_nom_currentIndexChanged(const QString &arg1);

    void on_pushButton_24_clicked();

    void on_pushButton_25_clicked();

    void on_ConfirmApiData_clicked();

    void on_clear_api_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_18_clicked();

    void on_pdf_r_clicked();

    void on_lineEdit_rechercheem_2_textChanged(const QString &arg1);

    void on_del_rest_clicked();

    void on_asc_clicked();

    void on_dec_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_21_clicked();

    void on_back1_btn_9_clicked();

    void on_add_rest_2_clicked();

    bool control_rest(int n);

    void on_pushButton_20_clicked();

    void on_back_re_2_clicked();

    void on_pushButton_55_clicked();

    void on_pushButton_54_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_56_clicked();

    void on_back1_btn_157_clicked();

    void on_radioButton_5_clicked();

    void on_radioButton_6_clicked();

    void on_btn_update_aid_clicked();

    void on_back_update_aiddash_clicked();

    void on_back_list_benifdash_clicked();

    void on_pushButton_22_clicked();

    void on_update_res_clicked();

    void on_back1_btn_8_clicked();

    void on_pushButton_349_clicked();

    void on_pushButton_304_clicked();

    void on_pushButton_364_clicked();

    void on_lineEdit_recherchebn_textChanged(const QString &arg1);

    void on_pushButton_58_clicked();

    void on_back1_btn_140_clicked();

    void on_pushButton_30_clicked();

    void on_backzzzz_clicked();

    void on_pushButton_57_clicked();

    void on_pushButton_305_clicked();

    bool postrequest(QString smsmsg, QString phonenumber);

    void on_back1_btn_135_clicked();

    void on_pushButton_31_clicked();

    void on_lineEdit_rechercheem_textChanged(const QString &arg1);

    void on_button_employee_clicked();

    void on_button_restaurants_clicked();

    void on_button_benif_clicked();

    void on_button_donations_clicked();

    void on_button_events_clicked();

    void on_lineEdit_rechercheev_textChanged(const QString &arg1);

    void on_pushButton_32_clicked();

    void on_pushButton_15_clicked();

    void on_delete_btn_ev_clicked();

    void on_pushButton_36_clicked();

    void on_pushButton_35_clicked();

    void on_backEventStat_clicked();

    void on_pushButton_addevent_clicked();

    void on_pushButton_33_clicked();

    void on_pushButton_51_clicked();

    void on_back21_btn_8_clicked();

    void on_back1_btn_5_clicked();

    void on_chat_btn_clicked();

    void on_calculator_btn_clicked();

    void on_tools_btn_clicked();


    void on_pushButton_34_clicked();

    void on_back_stats_dash18_clicked();

    void on_pushButton_18887_clicked();

    void on_pushButton_ard_dash_clicked();

private:
    Ui::MainWindow *ui;
    Employee Etmp;
    Aides ads;
    Restaurant Rtmp;
    Benif bn;
    Arduino A;
    QByteArray data;

    QString apikey = "eyJ2ZXIiOiIxLjAiLCJ0eXAiOiJKV1QiLCJhbGciOiJFUzM4NCIsImtpZCI6ImRzRUN2TDVaTENQbTl1R081RHltUjZCRTdMcnFGak5hX1VKbl9Ody1zdVUifQ.eyJhcHBfbmFtZSI6IlNtYXJ0IHNoYXJlIGFuZCBjYXJlIiwic3ViIjoiajRsTHJsOXJCVkhWbmdZNzBYdmlxdTJQdkt0a3A1b0ciLCJpc3MiOiJodHRwczpcL1wvYXBpLm9yYW5nZS5jb21cL29hdXRoXC92MyIsImV4cCI6MTY4MzEyMTE1MywiYXBwX2lkIjoiQmJua1huOHh1UUZEUXZtNSIsImlhdCI6MTY4MzExNzU1MywiY2xpZW50X2lkIjoiajRsTHJsOXJCVkhWbmdZNzBYdmlxdTJQdkt0a3A1b0ciLCJqdGkiOiIzYzRhY2NiNC05M2I1LTRjZWEtOTIxNi1mNWY5ODk2OTZlYzcifQ.ZcoPfDUSnsOiCPHz7BvepR47bKU4adC-PfuP6TokUGlXXjtdScu2cxd7MfWsy-rNBYuVhIgNVfdxy-r4iCFHEtVkxClrREYoo1FMVCS6dV_O8AEwtKyI3ruLOenk5k2y";
};
#endif // MAINWINDOW_H
