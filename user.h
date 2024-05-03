#ifndef USER_H
#define USER_H
#include "employee.h"

class user
{
private:
    Employee em;
    QString email;
    QString role;
    QString mdp;

public:
    user(){};
    user(Employee e, QString email, QString role, QString mdp)
    {
        this->em = e;
        this->email = email;
        this->mdp = mdp;
        this->role = role;
    }

    // setters
    void setEm(Employee e) { this->em = e; }
    void setEmail(QString m) { this->email = m; }
    void setMdp(QString m) { this->mdp = m; }

    // getters
    Employee getEm() { return em; }
    QString getEmail() { return email; }
    QString getMdp() { return mdp; }
    QString getrole() { return role; }

    // functions
    bool Ajouter_user(int id);
    user Login(QString e, QString mdp);
    bool Checkuser(QString e, QString mdp);
    bool Checkemail(QString e);
    QString Getpassword(QString e);
    user session(QString e, QString mdp);
};

#endif // USER_H
