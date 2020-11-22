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

void Discipline::setEnabled(bool enabled)
{
    m_isConductible = enabled;
}

bool Discipline::isEnabled() const
{
    return m_isConductible;
}

QString Discipline::getName() const
{
    return this->m_name;
}

const QPair<QDate, QDate> &Discipline::getTeachRange() const
{
    return m_range;
}

const QList<QString> &Discipline::getCourseDays() const
{
    return m_teachDays;
}

bool Discipline::operator<(const Discipline &other) const
{
    return m_name < other.m_name;
}

bool Discipline::operator==(const Discipline &other) const
{
    return m_name == other.m_name;
}

bool Discipline::operator!=(const Discipline &other) const
{
    return m_name != other.m_name;
}

void Discipline::write(QJsonObject &json)const
{

    json["name"] = m_name;
    json["rangeFrom"] = m_range.first.toString("dd.MM.yyyy");
    json["rangeTo"] = m_range.second.toString("dd.MM.yyyy");

    QJsonArray days;
    for(auto&i:m_teachDays)
        days.append(i);

    if(!days.isEmpty())
        json["teachDays"] = days;
}

void Discipline::read(const QJsonObject &json)
{
    if(json.contains("name") && json["name"].isString())
        m_name = json["name"].toString();

    if(json.contains("rangeFrom") && json["rangeFrom"].isString())
        m_range.first = QDate::fromString(json["rangeFrom"].toString(),"dd.MM.yyyy");
    else
        m_range.first.setDate(2020,9,1);

    if(json.contains("rangeTo") && json["rangeTo"].isString()){
        m_range.second = QDate::fromString(json["rangeTo"].toString(), "dd.MM.yyyy");

        QDate currentDate = QDate::currentDate();

        if(m_range.second.month() < currentDate.month()){
            m_isConductible = false;
        }
        else if(m_range.second.month() == currentDate.month()){
            if(m_range.second.day() <= currentDate.day()){
                m_isConductible = false;
            }
            else{
                m_isConductible = true;
            }
        }
        else{
            m_isConductible = true;
        }
    }
    else
        m_range.second.setDate(2020,12,30);

    if(json.contains("teachDays") && json["teachDays"].isArray()){
        QJsonArray days = json["teachDays"].toArray();
        for(const auto&i:days)
            m_teachDays.append(i.toString());
    }else{
        for(int i = 1; i <= 7; ++i){
            QLocale day;
            m_teachDays.append(day.dayName(i,QLocale::ShortFormat));
        }
    }

}
