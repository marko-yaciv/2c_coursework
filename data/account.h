#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <QString>


/*This class represents accouni in system.
As every account is has it's ID and Password*/
class Account
{
protected:
    long m_id;
    QString m_password;
public:
    Account(const QString pw);
    ~Account();

    void setId(long id);
    void setPassword(const QString& pswrd);

    long getId() const;
    const QString& getPasword() const;

};

#endif // ACCOUNT_H
