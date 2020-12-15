#ifndef STUDENT_H
#define STUDENT_H

#include <QVector>
#include <QMap>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include "account.h"
#include "human.h"
#include "exept.h"
class Discipline;
class Teacher;

/*Class for representing
student in learm system context.
Every student has it's own <studyMap>*/
class Student: public Account, public Human
{
    int m_course;
    QString m_group;

    QList<Discipline> m_courses;
    QMap<Discipline,Teacher> m_studyMap;

    friend class StudyProcessData;

public:
    Student();
    Student(const QString& fName, const QString& lName, const QString& fthName,
            const QString& group, const QString& password);
    Student(const QString& fName, const QString& lName, const QString& fthName,
            const QString& password);
    ~Student();

    void addDiscipline(const Discipline& discipl);
    void addStudyTarget(const Discipline& discipline,const Teacher& Teacher);

    void removeStudyTarget(const Discipline& discipline);


    const QList<Discipline> &getDisciplines() const;
    const QMap<Discipline, Teacher>& getStudyMap() const;
    const QString& getGroup() const;
    int getCourse() const;

    void write(QJsonObject& json) const;
    void read(const QJsonObject json);

    bool lessThan(const Student& other) const;

    Student& operator=(const Student& other);
    bool operator==(const Student& other) const;

    bool operator<(const Student& other) const;

};

#endif // STUDENT_H
