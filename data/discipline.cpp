#include "discipline.h"

Discipline::Discipline(QString& name)
{
    this->name = name;
}

int Discipline::addTeacher(const Teacher &teacher)
{
    this->teachersOfDiscipline.append(teacher);

    return teachersOfDiscipline.size();
}

void Discipline::rename(QString& newName)
{
    this->name = newName;
}

QString Discipline::getName() const
{
    return this->name;
}

const QVector<Teacher>& Discipline::getTeachersList() const
{
    return this->teachersOfDiscipline;
}
