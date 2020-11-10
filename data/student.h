#ifndef STUDENT_H
#define STUDENT_H

#include <QVector>
#include <QMap>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include "account.h"
#include "human.h"


class Discipline;
class Teacher;

class Student: public Account, public Human
{
    int m_course;
    QString m_group;

    QMap<Discipline,Teacher> m_studyMap;

public:
    Student();
    Student(const QString& fName, const QString& lName, const QString& fthName,
            const QString& group, const QString& password);
    Student(const QString& fName, const QString& lName, const QString& fthName,
            const QString& password);
    ~Student();

    void setGroup(const QString group);
    void setInitials(const QString fname, const QString lname, const QString fthname);
    void setStudyMap(const QMap<Discipline,Teacher>& map);
    void addDiscipline(const Discipline& discipl);
    void addStudyTarget(const Discipline& discipline,const Teacher& Teacher);

    int countDisciplines() const;

    const QMap<Discipline, Teacher>& getStudyMap() const;
    const QVector<QString>* getInitials() const;
    const QString& getGroup() const;
    int getCourse() const;

    const Teacher* getTeacher(const Discipline &discipl) const;
    const Discipline* getDiscipline(const Teacher &teach) const;


    void write(QJsonObject& json) const;
    void read(const QJsonObject json);

    void operator=(const Student& other);
    bool operator==(const Student& other) const;
};

#endif // STUDENT_H
