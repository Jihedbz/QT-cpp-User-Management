#include "user.h"

bool user::Ajouter_user(int id)
{

    QSqlQuery query;

    query.prepare("insert into COMPTE (USERNAME, PASSWORD, ROLE, ID_EM)"
                  "values (:USERNAME,:PASSWORD,:ROLE,:ID_EM)");
    query.bindValue(":USERNAME", email);
    query.bindValue(":PASSWORD", mdp);
    query.bindValue(":ROLE", role);
    query.bindValue(":ID_EM", id);

    qDebug() << "Added";
    return query.exec();
}

bool user::Checkuser(QString e, QString mdp)
{

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM COMPTE WHERE USERNAME=:email AND PASSWORD=:mdp");
    query.bindValue(":email", e);
    query.bindValue(":mdp", mdp);
    if (query.exec() && query.first())
    {
        int count = query.value(0).toInt();
        return count > 0;
    }
    else
    {
        return false;
    }
}

bool user::Checkemail(QString e)
{

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM COMPTE WHERE USERNAME=:email ");
    query.bindValue(":email", e);
    if (query.exec() && query.first())
    {
        int count = query.value(0).toInt();
        return count > 0;
    }
    else
    {
        return false;
    }
}

user user::Login(QString e, QString mdp)
{
    QSqlQuery query, qry;

    query.prepare("select * from COMPTE where USERNAME=:email and PASSWORD=:mdp");
    query.bindValue(":email", e);
    query.bindValue(":mdp", mdp);
    query.exec();
    query.next();

    int id = query.value(4).toInt();
    QString role = query.value(3).toString();

    QString nom, prenom, tel, addresse, email, fonction, date, sexe;
    int ide, salaire;
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
            // adresse
            addresse = qry.value(5).toString();
            // fonction
            fonction = qry.value(6).toString();
            // date
            date = qry.value(7).toString();
            // telephone
            tel = qry.value(8).toString();
            // salaire
            salaire = qry.value(9).toInt();
        }
    }

    Employee em(ide, nom, prenom, email, sexe, addresse, fonction, date, tel, salaire);

    user u(em, e, fonction, mdp);
    return u;
}

QString user::Getpassword(QString e)
{
    QSqlQuery query;
    query.prepare("SELECT PASSWORD FROM COMPTE WHERE USERNAME = :email");
    query.bindValue(":email", e);
    if (!query.exec())
    {
        qDebug() << "Error executing query!";
        return "";
    }

    if (query.next())
    {
        QString password = query.value(0).toString();
        return password;
    }
    else
    {
        return "";
    }
}
