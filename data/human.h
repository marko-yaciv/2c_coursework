#ifndef HUMAN_H
#define HUMAN_H

#include<QString>
class Human
{
protected:
    QString m_fName;
    QString m_lName;
    QString m_fthName;

public:
    Human();
    Human(const QString fname, const QString lname, const QString fthname);
    const QString& getFname() const;
    const QString& getLname() const;
    const QString& getFthName() const;
    ~Human();
};

#endif // HUMAN_H
