#ifndef STUDENT_H
#define STUDENT_H

#include <QVector>
#include <QMap>
#include "account.h"
#include "teacher.h"
#include "discipline.h"
class Student: protected Account
{
    struct Initials{
        QString fName;
        QString lName;
        QString fthName;
    }inits;
    int course;
    QString group;
    QMap<Discipline, Teacher> studyMap;

public:
    Student();
    Student(const QString fName, const QString lName, const QString fthName,
            const int course, const QString group);
    ~Student();

    void setInitials(const QString fname, const QString lname, const QString fthname);
    void setCourse(const int course);
    void setGroup(const QString group);
    void addStudyTarget(const Discipline& discipline, const Teacher& Teacher);

    const Initials& getInitials() const;
    const QString& getGroup() const;
    int getCourse() const;
};

#endif // STUDENT_H
