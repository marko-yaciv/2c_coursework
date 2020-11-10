#include "discipline.h"

Discipline::Discipline()
{
    this->m_name = "None";
}

Discipline::Discipline(const QString& name)
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

bool Discipline::operator==(const Discipline &other) const
{
    return this->m_name == other.m_name;
}

bool Discipline::operator!=(const Discipline &other) const
{
    return this->m_name != other.m_name;
}

void Discipline::write(QJsonObject &json)const
{
    json["name"] = m_name;
}

void Discipline::read(const QJsonObject &json)
{
    if(json.contains("name") && json["name"].isString())
        m_name = json["name"].toString();
}
