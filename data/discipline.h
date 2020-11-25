#ifndef DISCIPLINE_H
#define DISCIPLINE_H
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QDate>
#include "exept.h"
class Discipline
{
    QString m_name;
    QPair<QDate,QDate> m_range;
    QList<QString> m_teachDays;

    bool m_isConductible;
    bool m_isValid;
public:
    Discipline();
    Discipline(const QString& name);
    ~Discipline();

    void rename(QString& newName);
    void setEnabled(bool enabled);

    bool isEnabled() const;
    bool isValid() const;
    QString getName() const;
    const QPair<QDate,QDate>& getTeachRange() const;
    const QList<QString>& getCourseDays() const;
    void read(const QJsonObject& json);
    void write(QJsonObject& json)const;

    bool operator<(const Discipline& other) const;
    bool operator==(const Discipline&other) const;
    bool operator!=(const Discipline&other) const;

};

#endif // DISCIPLINE_H
