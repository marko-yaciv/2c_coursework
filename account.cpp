#include "account.h"

Account::Account()
{

}

Account::Account(const QString pw)
{
    this->id = qrand()%10000+1001;
    this->password = pw;
}

long Account::getId() const
{
    return this->id;
}

QString Account::getPasword() const
{
    return this->password;
}
