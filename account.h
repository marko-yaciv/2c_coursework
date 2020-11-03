#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <QString>

class Account
{
    long id;
    QString password;
public:
    Account();
    Account(const QString pw);
    long getId() const;
    QString getPasword() const;
};

#endif // ACCOUNT_H
