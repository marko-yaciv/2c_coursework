#ifndef TEACHER_H
#define TEACHER_H

#include <QString>
#include <QVector>
#include <QMultiMap>
#include <QJsonObject>
#include <QJsonArray>
#include "human.h"
#include "account.h"
#include "exept.h"
class Discipline;
class Student;

class Teacher : public Account, public Human
{
    int m_stage;
    QString m_post;
    int m_populatity;
    QList<Discipline> m_courses;
    QMultiMap<Discipline, Student> m_courseVisitors;

    friend class StudyProcessData;

private:
    void fetchDisciplines(const QJsonArray& courses, const QVector<QVector<Discipline>>& allDisciplines);
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
    void addDiscipline(const Discipline& discipline);
    void addCourseTarget(const Discipline discipline,const Student stud);

    void removeCourseTarget(const Discipline& discipline,const Student& stud);
    void removeDiscipline(const Discipline& discipline);

    const QList<Student> getCourseVistors(const Discipline& dis) const;
    const QMultiMap<Discipline,Student>& getCourseMap() const;
    const QList<Discipline>& getDisciplines() const;
    const QString& getPost() const;
    int getPopularity() const;
    int getStage() const;

    bool hasDiscipline(const Discipline& discipl) const;

    void write(QJsonObject& json) const;
    void read(const QJsonObject& json, const QVector<QVector<Discipline>>& allDisciplines);

    bool compareByPost(const Teacher& other, const QMap<short,QString>& postNames) const;
    bool operator==(const Teacher& other) const;
    bool operator!=(const Teacher& other) const;
    bool operator>(const Teacher& other) const;
    bool operator<(const Teacher& other) const;


};

#endif // TEACHER_H
