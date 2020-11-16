#include "human.h"


Human::Human()
{
    this->m_fName = "";
    this->m_lName = "";
    this->m_fthName = "";
}

Human::Human(const QString fname, const QString lname, const QString fthname)
{
    this->m_fName = fname;
    this->m_lName = lname;
    this->m_fthName = fthname;
}

const QString &Human::getFname() const
{
    return m_fName;
}

const QString &Human::getLname() const
{
    return m_lName;
}

const QString &Human::getFthName() const
{
    return m_fthName;
}

Human::~Human()
{

}
