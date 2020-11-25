#ifndef FLPARSER_H
#define FLPARSER_H
#include <QFile>
#include <QJsonDocument>
#include "data/student.h"
#include "data/teacher.h"
#include "data/discipline.h"
#include "data/studyprocessdata.h"

#define GROUPS_FILE "datafiles/groups.json"
#define STUDENTS_FILE "datafiles/students.json"
#define TEACHERS_FILE "datafiles/teachers.json"
#define STUD_STUDY_PROCESS_FILE "datafiles/studstudyproc.json"
#define TEACHER_STUDY_PROCESS_FILE "datafiles/teachstudyproc.json"

class FlParser
{
    QString filename;
    QFile file;
public:
    FlParser(){};
    FlParser(const QString& flname);
    void changeFilename(const QString& fname);

    bool openWithValidation(const QIODevice::OpenModeFlag& openMode);

    void writeGroups(const QList<QString>& data);
    void writeStudents(const QVector<Student>& students) noexcept(false);
    void writeTeachers(const QVector<Teacher>& teachers)noexcept(false);
    void writeDisciplines(const QVector<Discipline>& discipls)noexcept(false);
    void writeStudyProcess(const StudyProcessData& studentData)noexcept(false);


    void readGroups(QList<QString>& data)noexcept(false);
    void readStudents(QVector<Student>& student)noexcept(false);
    void readTeachers(QVector<Teacher>& teacher)noexcept(false);
    void readDisciplines(QVector<Discipline>& discipl)noexcept(false);
    void readStudyProcess(StudyProcessData& studentData)noexcept(false);
};

#endif // FLPARSER_H
