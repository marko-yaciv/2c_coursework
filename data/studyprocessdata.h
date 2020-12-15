#ifndef STUDYPROCESSDATA_H
#define STUDYPROCESSDATA_H

#include <QMap>
#include <QJsonObject>
#include "student.h"
#include "teacher.h"
#include "discipline.h"
#include "exept.h"

/*
This class contains studets' and
teachers' data about their courses, and
subjects
*/
class StudyProcessData
{
  QList<Student> m_allStudents;
  QList<Teacher> m_allTeachers;
  QVector<QVector<Discipline>> m_allDisciplines;

  QMap<Student, QMap<Discipline, Teacher>> m_studentsStudyMaps;
  QMap<Teacher, QMultiMap<Discipline, Student>> m_teachersCourseMaps;

  QList<QString> m_groups;
  QMap<short,QString> m_postNames;

  static StudyProcessData* allData;
  static const int COURSES;

private:
  StudyProcessData();
  StudyProcessData(const StudyProcessData&);
  StudyProcessData& operator=(StudyProcessData&);

  void fetchTeachersWithDisciplinesForStudent(Student& studentItr, QJsonObject& ownerMap);
  void fetchStudentsWithDisciplinesForTeacher(Teacher& studentItr, QJsonObject& ownerMap);

public:
    ~StudyProcessData();
    static StudyProcessData* getInstance();

    void updateMapForStudent(const Student& mapOwner,const QMap<Discipline,Teacher>& map);
    void updateMapForTeacher(const Teacher& mapOwner,const QMultiMap<Discipline,Student>& map);

    QList<QString>& getGroups();
    QList<Student>& getAllStudents();
    QList<Teacher>& getAllTeachers();
    QMap<short, QString>& getPostNames();
    QVector<QVector<Discipline>>& getAllDisciplines();


    void setGroups(const QList<QString>& groups);
    void setPostNames(const QMap<short,QString>& names);
    //void setAllDisciplines(const QVector<QVector<Discipline>>& disciplines);

    void writeMapsForStudents(QJsonObject& json) const;
    void writeMapsForTeachers(QJsonObject& json) const;

    void readMapsOfStudents(const QJsonObject& json);
    void readMapsOfTeachers(const QJsonObject& json);
};


#endif // STUDYPROCESSDATA_H
