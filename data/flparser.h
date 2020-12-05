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

/*This class represents reading&writing
all data from .json files,
located in <file> path(projectDir/datafiles)*/
class FlParser
{

    QFile file;
public:
    FlParser();
    ~FlParser();
    FlParser(const QString& flname);

    void changeFilename(const QString& fname);
    bool openWithValidation(QIODevice::OpenModeFlag openMode);

    void writeGroups(const QList<QString>& data);
    void writeStudents(const QList<Student>& students);
    void writeTeachers(const QList<Teacher>& teachers);
    void writeDisciplines(const QVector<Discipline>& discipls);
    void writeStudentStudyProcessData(const StudyProcessData* studentData);
    void writeTeacherStudyProcessData(const StudyProcessData* teacherData);

    void readGroups(QList<QString>& data);
    void readStudents(QList<Student>& student, StudyProcessData* allData);
    void readTeachers(QList<Teacher>& teacher, StudyProcessData* allData);
    void readDisciplines(QVector<Discipline>& discipl);
    void readStudentStudyProcessData(StudyProcessData* studentData);
    void readTeacherStudyProcessData(StudyProcessData* studentData);
};

#endif // FLPARSER_H
