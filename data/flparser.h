#ifndef FLPARSER_H
#define FLPARSER_H
#include <QFile>
#include <QJsonDocument>
#include "data/student.h"
#include "data/teacher.h"
#include "data/discipline.h"
#include "data/studyprocessdata.h"

#define GROUPS_FILE "/datafiles/groups.json"
#define STUDENTS_FILE "/datafiles/students.json"
#define TEACHERS_FILE "/datafiles/teachers.json"
#define STUD_STUDY_PROCESS_FILE "/datafiles/studstudyproc.json"
#define TEACHER_STUDY_PROCESS_FILE "/datafiles/teachstudyproc.json"

class FlParser
{
    QString filename;
    QFile file;
public:
    FlParser();
    ~FlParser();
    FlParser(const QString& flname);
    void changeFilename(const QString& fname);

    bool openWithValidation(QIODevice::OpenModeFlag openMode);

    void writeGroups(const QList<QString>& data);
    void writeStudents(const QVector<Student>& students);
    void writeTeachers(const QVector<Teacher>& teachers);
    void writeDisciplines(const QVector<Discipline>& discipls);
    void writeStudentStudyProcessData(const StudyProcessData& studentData);
    void writeTeacherStudyProcessData(const StudyProcessData& teacherData);

    void readGroups(QList<QString>& data);
    void readStudents(QVector<Student>& student);
    void readTeachers(QVector<Teacher>& teacher);
    void readDisciplines(QVector<Discipline>& discipl);
    void readStudentStudyProcessData(StudyProcessData& studentData);
    void readTeacherStudyProcessData(StudyProcessData& studentData);
};

#endif // FLPARSER_H
