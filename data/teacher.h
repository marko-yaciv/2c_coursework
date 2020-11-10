#ifndef TEACHER_H
#define TEACHER_H

#include <QString>
#include <QVector>
#include <QMultiMap>
#include <QJsonObject>
#include <QJsonArray>
#include "human.h"
#include "account.h"


class Discipline;
class Student;

class Teacher : public Account, public Human
{
    QString m_post;
    int m_stage;

    int m_populatity;

    QMultiMap<Discipline, Student> m_courseVisitors;
public:
    Teacher();
    Teacher(const QString& fname, const QString& lname, const QString& fthname, const QString& post,
            const int stage, const QString& password);
    Teacher(const QString& fname, const QString& lname, const QString& fthname,
            const QString& password);
    ~Teacher();

    void setInitials(const QString& fname, const QString& lname, const QString& fthname);
    void setPost(const QString& post);
    void setStage(const int stage);
    void setCourseVisitors(QMultiMap<Discipline, Student> map);
    void updateDisciplineWithStuds(Discipline& discipline, Student& stud);
//    /* Sets thre discipline to Teacher which he/she teaches.*/
    void setDiscipline(const Discipline& discipline);

//    /* Appends the student to the teacher's list of students
//       Returns size of All Students array*/
    //int addStudent(const Student& newStudent);

    bool hasDiscipline(const Discipline& discipl) const;
    const QList<Discipline>* getDissciplines() const;
    const QList<Student>* getSudents() const;
    const QList<Student>* getSudents(const Discipline& discipl) const;
    const QMultiMap<Discipline,Student>& getVisitors() const;
    const QVector<QString>* getInitials() const;
    const QString& getPost() const;
    int getStage() const;
    int getPopularity() const;

    bool operator==(const Teacher& other) const;
    bool operator!=(const Teacher& other) const;

    void write(QJsonObject& json) const;
    void read(const QJsonObject& json);
};

#endif // TEACHER_H
