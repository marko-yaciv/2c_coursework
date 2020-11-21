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
    QList<Discipline> m_courses;
    QMultiMap<Discipline, Student> m_courseVisitors;

    friend class StudyProcessData;
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
    void addCourseTarget(const Discipline discipline,const Student stud);

    void removeStudent(const Discipline& discipline,const Student& stud);
    void setDiscipline(const Discipline& discipline);

    bool hasDiscipline(const Discipline& discipl) const;

    const QMultiMap<Discipline,Student>& getCourseMap() const;

    const QList<Discipline>& getDisciplines() const;
    const QList<Student> getCourseVistors(const Discipline& dis) const;
    const QString& getPost() const;
    int getPopularity() const;
    int getStage() const;

    bool operator==(const Teacher& other) const;
    bool operator!=(const Teacher& other) const;
    bool operator>(const Teacher& other) const;
    bool operator<(const Teacher& other) const;

    void write(QJsonObject& json) const;
    void read(const QJsonObject& json);

};

#endif // TEACHER_H
