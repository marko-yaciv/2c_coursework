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
    QMap<Student, QMap<Discipline, Teacher>> m_studentsStudyMaps;

    QMap<Teacher, QMultiMap<Discipline, Student>> m_teachersCourseMaps;
public:
    StudyProcessData();
    ~StudyProcessData();

    void updateMapForStudent(const Student& mapOwner,const QMap<Discipline,Teacher>& map);
    void updateMapForTeacher(const Teacher& mapOwner,const QMultiMap<Discipline,Student>& map);

    const QMap<Discipline,Teacher> getMapOfStudent(const Student& mapOwner)const noexcept(false);
    const QMultiMap<Discipline,Student> getMapOfTeacher(const Teacher& mapOwner)const noexcept(false);

    void writeMapsForStudents(QJsonObject& json) const;
    void writeMapsForTeachers(QJsonObject& json) const;

    void readMapsOfStudents(const QJsonObject& json);
    void readMapsOfTeachers(const QJsonObject& json);

    void convertDataFromShortForm();
};


#endif // STUDYPROCESSDATA_H
