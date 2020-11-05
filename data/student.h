#ifndef STUDENT_H
#define STUDENT_H

#include <QVector>
#include <QMap>
#include <QStringList>

#include "account.h"
#include "human.h"


class Discipline;
class Teacher;

class Student: public Account, public Human
{
    int m_course;
    QString m_group;
    QMap<Discipline, Teacher> m_studyMap;

public:
    Student();
    Student(const QString& fName, const QString& lName, const QString& fthName,
            const QString& group, const QString& password);
    ~Student();

    void setGroup(const QString group);
    void setInitials(const QString fname, const QString lname, const QString fthname);
    void addStudyTarget(Discipline& discipline,Teacher& Teacher);

    const QVector<QString>* getInitials() const;
    const QString& getGroup() const;
    int getCourse() const;

    const QMap<Discipline, Teacher>& getStudyMap() const;

    bool operator==(const Student& other) const;
};

#endif // STUDENT_H
