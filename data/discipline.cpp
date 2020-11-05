#include "discipline.h"

Discipline::Discipline()
{
    this->m_name = "None";
}

Discipline::Discipline(QString& name)
{
    this->m_name = name;
}

Discipline::~Discipline()
{

}

void Discipline::rename(QString& newName)
{
    this->m_name = newName;
}

QString Discipline::getName() const
{
    return this->m_name;
}

bool Discipline::operator<(const Discipline &other) const
{
    return (this->m_name < other.m_name);
}
