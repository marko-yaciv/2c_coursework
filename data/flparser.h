#ifndef FLPARSER_H
#define FLPARSER_H
#include <QFile>
#include <QJsonDocument>
#include "data/student.h"
#include "data/teacher.h"
#include "data/discipline.h"
#include "data/studyprocessdata.h"

#define GROUPS_FILE "datafiles/groups.bin"
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

    void writeStudents(const QVector<Student>& students);
    void writeTeachers(const QVector<Teacher>& teachers);
    void writeDiscipl(const QList<QString> discipls);
    void writeStudyProcess(const StudyProcessData& studentData);


    void readData(QList<QString>& data);
    void readStudents(QVector<Student>& student);
    void readTeachers(QVector<Teacher>& teacher);
    void readDisciplines(QVector<Discipline>& discipl);
    void readStudyProcess(StudyProcessData& studentData);
};

#endif // FLPARSER_H
