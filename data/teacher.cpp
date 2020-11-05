#include "teacher.h"
#include "discipline.h"
#include "student.h"
Teacher::Teacher():Account("None"), Human()
{
    this->m_post = "None";
    this->m_stage = 0;
    this->populatity = 0;
}

Teacher::Teacher(const QString& fname, const QString& lname, const QString& fthname,
                 const QString& post, const int stage, const QString& password):
                 Account(password), Human(fname, lname,fthname)
{
    this->m_post = post;
    this->m_stage = stage;
    this->populatity = 0;
    //this->m_courseStudents = new QMultiMap<Discipline,Student>;
}

Teacher::~Teacher()
{
    //delete this->m_courseStudents;
}

void Teacher::setInitials(const QString& fname, const QString& lname, const QString& fthname)
{
    this->m_fName = fname;
    this->m_lName = lname;
    this->m_fthName = fthname;
}

void Teacher::setPost(const QString& post)
{
    this->m_post = post;
}

void Teacher::setStage(const int stage)
{
    this->m_stage = stage;
}

void Teacher::updateDisciplineWithStuds(Discipline &discipline,Student &stud)
{
    this->m_courseStudents.insert(discipline, stud);
    ++populatity;
}

//void Teacher::setDiscipline(const Discipline& discipline)
//{
//    this->m_disciplines.append(discipline);
//}

//int Teacher::addStudent(const Student& newStudent)
//{
//    this->m_courseStudents.append(newStudent);
//    return m_courseStudents.size();
//}

const QVector<QString> *Teacher::getInitials() const
{
    return new QVector<QString>({m_fName, m_lName, m_fthName});
}

const QString &Teacher::getPost() const
{
    return this->m_post;
}

int Teacher::getStage() const
{
    return this->m_stage;
}

int Teacher::getPopularity()
{
    return populatity;
}

const QList<Student> *Teacher::getSudents() const
{
    return new QList<Student>(this->m_courseStudents.values());
}

bool Teacher::operator==(const Teacher &other) const
{
//    return (*this->getInitials() == *other.getInitials()) &&
//            (this->getPost() == other.getPost());
    return this->getId() == other.getId();
}
