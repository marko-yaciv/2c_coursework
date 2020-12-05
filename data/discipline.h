#ifndef DISCIPLINE_H
#define DISCIPLINE_H
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QDate>
#include "exept.h"

/*This class represents discipline in every learning system
discipline has it's name? conduct range and
days when it is being conducted*/
class Discipline
{
    QString m_name;
    QPair<QDate,QDate> m_range;
    QList<QString> m_teachDays;

    bool m_isConductible;
    bool m_isValid;

private:
    void validate();
public:
    Discipline();
    Discipline(const QString& name);
    ~Discipline();

    void rename(QString& newName);
    void setEnabled(bool enabled);
    void addTeachDay(QString& day);
    void removeTeachDay(QString& day);
    void setConductRange(QDate& start, QDate& finish);

    bool isValid() const;
    bool isEnabled() const;

    QString getName() const;
    const QList<QString>& getCourseDays() const;
    const QPair<QDate,QDate>& getConductRange() const;

    void write(QJsonObject& json)const;
    void read(const QJsonObject& json);

    bool operator<(const Discipline& other) const;
    bool operator==(const Discipline&other) const;
    bool operator!=(const Discipline&other) const;

};

#endif // DISCIPLINE_H
