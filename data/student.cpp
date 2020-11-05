#include "student.h"
#include "discipline.h"
#include "teacher.h"
Student::Student():Account("None"), Human()
{
    this->m_course = 0;
    this->m_group = "None";
}

Student::Student(const QString& fname, const QString& lname, const QString& fthname,
                 const QString& group, const QString& password):
                 Account(password),Human(fname, lname,fthname)
{
    this->m_group = group;
    this->m_course = group.at(3).unicode() - 48;
    //this->m_studyMap = new QMap<Discipline, Teacher>;
}

Student::~Student()
{
    //delete this->m_studyMap;
}

void Student::setInitials(const QString fname, const QString lname, const QString fthname)
{
    this->m_fName = fname;
    this->m_lName  = lname;
    this->m_fthName = fthname;
}

void Student::setGroup(const QString group)
{
    this->m_group = group;
    this->m_course = m_group.at(3).unicode() - 48;
}

void Student::addStudyTarget(Discipline &discipline,Teacher &teacher)
{
    this->m_studyMap.insert(discipline, teacher);
}

const QVector<QString>* Student::getInitials() const
{
    return new QVector<QString>({m_fName, m_lName, m_fthName});
}

const QString &Student::getGroup() const
{
    return this->m_group;
}

int Student::getCourse() const
{
    return this->m_course;
}

const QMap<Discipline, Teacher>& Student::getStudyMap() const
{
    return m_studyMap;
}

bool Student::operator==(const Student &other) const
{
//    return (*this->getInitials() == *other.getInitials()) &&
//            (this->getGroup()==other.getGroup());
    return this->getId() == other.getId();
}
