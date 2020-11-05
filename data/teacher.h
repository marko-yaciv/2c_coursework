#ifndef TEACHER_H
#define TEACHER_H

#include <QString>
#include <QVector>
#include <QMultiMap>
#include "human.h"
#include "account.h"


class Discipline;
class Student;

class Teacher : public Account, public Human
{
    QString m_post;
    int m_stage;

    int populatity;

    QMultiMap<Discipline,Student> m_courseStudents;


    //Discipline m_discipline;

    /*The teacher has a list of students who wisit his course*/
    //QVector<Student> m_courseStudents;
public:
    Teacher();
    Teacher(const QString& fname, const QString& lname, const QString& fthname, const QString& post,
            const int stage, const QString& password);
    ~Teacher();

    void setInitials(const QString& fname, const QString& lname, const QString& fthname);
    void setPost(const QString& post);
    void setStage(const int stage);

    void updateDisciplineWithStuds(Discipline& discipline, Student& stud);
//    /* Sets thre discipline to Teacher which he/she teaches.*/
//    void setDiscipline(const Discipline& discipline);

//    /* Appends the student to the teacher's list of students
//       Returns size of All Students array*/
//    int addStudent(const Student& newStudent);

    const QVector<QString>* getInitials() const;
    const QString& getPost() const;
    int getStage() const;
    int getPopularity();
    const QList<Student>* getSudents() const;

    bool operator==(const Teacher& other) const;

};

#endif // TEACHER_H
