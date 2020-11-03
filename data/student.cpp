#include "student.h"

Student::Student()
{

}

Student::Student(const QString fname, const QString lname, const QString fthname,
                 const int course, const QString group)
{
    this->inits.fName = fname;
    this->inits.lName  = lname;
    this->inits.fthName = fthname;
    this->course = course;
    this->group = group;
}

void Student::setInitials(const QString fname, const QString lname, const QString fthname)
{
    this->inits.fName = fname;
    this->inits.lName  = lname;
    this->inits.fthName = fthname;
}

void Student::setCourse(const int course)
{
    this->course = course;
}

void Student::setGroup(const QString group)
{
    this->group = group;
}

void Student::addStudyTarget(const Discipline &discipline, const Teacher &teacher)
{
    this->studyMap.insert(discipline, teacher);
}

const Student::Initials &Student::getInitials() const
{
    return this->inits;
}

const QString &Student::getGroup() const
{
    return this->group;
}

int Student::getCourse() const
{
    return this->course;
}
