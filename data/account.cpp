#include "account.h"
#include <QRandomGenerator>

Account::Account(const QString pw)
{
    this->m_id = QRandomGenerator::global()->bounded(1001,8998);
    this->m_password = pw;
}

Account::~Account()
{

}

void Account::setId(long id)
{
    this->m_id = id;
}

void Account::setPassword(const QString &pswrd)
{
    this->m_password = pswrd;
}

long Account::getId() const
{
    return this->m_id;
}

const QString& Account::getPasword() const
{
    return this->m_password;
}
