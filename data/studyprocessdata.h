#ifndef STUDYPROCESSDATA_H
#define STUDYPROCESSDATA_H

#include <QMap>
#include <QJsonObject>
#include "student.h"
#include "teacher.h"
#include "discipline.h"
#include "exept.h"

/*
This class contains studet's and
teacher's data about their courses.
*/
class StudyProcessData
{
  QList<Student> m_allStudents;
  QList<Teacher> m_allTeachers;
  QVector<QVector<Discipline>> m_allDisciplines;

  QMap<Teacher, QMultiMap<Discipline, Student>> m_teachersCourseMaps;
  QMap<Student, QMap<Discipline, Teacher>> m_studentsStudyMaps;

  QList<QString> m_groups;
  QMap<short,QString> m_postNames;

  static StudyProcessData* allData;
  static const int COURSES;

  StudyProcessData();
  StudyProcessData(const StudyProcessData&);
  StudyProcessData& operator=(StudyProcessData&);


private:
  void fetchTeachersWithDisciplines(Student& studentItr, QJsonObject& ownerMap);
  void fetchStudentsWithDisciplines(Teacher& studentItr, QJsonObject& ownerMap);

public:
    ~StudyProcessData();
    static StudyProcessData* getInstance();

    void updateMapForStudent(const Student& mapOwner,const QMap<Discipline,Teacher>& map);
    void updateMapForTeacher(const Teacher& mapOwner,const QMultiMap<Discipline,Student>& map);

    QList<Student>& getAllStudents();
    QList<Teacher>& getAllTeachers();

    QVector<QVector<Discipline>>& getAllDisciplines();
    QMap<short, QString>& getPostNames();
    QList<QString>& getGroups();


    void setAllDisciplines(const QVector<QVector<Discipline>>& disciplines);
    void setPostNames(const QMap<short,QString>& names);
    void setGroups(const QList<QString>& groups);

    void writeMapsForStudents(QJsonObject& json) const;
    void writeMapsForTeachers(QJsonObject& json) const;

    void readMapsOfStudents(const QJsonObject& json);
    void readMapsOfTeachers(const QJsonObject& json);
};


#endif // STUDYPROCESSDATA_H
