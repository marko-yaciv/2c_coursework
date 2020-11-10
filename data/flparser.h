#ifndef FLPARSER_H
#define FLPARSER_H
#include <QFile>
#include <QJsonDocument>
#include "data/student.h"
#include "data/teacher.h"
#include "data/discipline.h"

#define GROUPS_FILE "datafiles/groups.bin"
#define STUDENTS_FILE "datafiles/students.json"
#define TEACHERS_FILE "datafiles/teachers.json"
class FlParser
{
    QString filename;
    QFile file;
public:
    FlParser(const QString& flname);
    void changeFilename(const QString& fname);

//    void writeData(const QString&);
//    void writeStudentInfo(const Student&);
//    void writeTeacherInfo(const Teacher&);

//    void writeMapForStudent(const QMap<Discipline,Teacher>& map);
//    void writeMapForTeacher(const QMultiMap<Discipline,Student>& map);

//    void readMapForStudent(QMap<Discipline,Teacher>& map);
//    void readMapForTeacher(QMultiMap<Discipline,Student>& map);

    void readData(QList<QString>& data);
//    void readStudents(QVector<Student>& students);
//    void readTeachers(QVector<Teacher>& teachers);
//    void readDisciplines(QVector<Discipline>& disciplines);

    void writeStudents(const QVector<Student>& students);
    void writeTeachers(const QVector<Teacher>& teachers);
    void writeDiscipl(const QList<QString> discipls);

    void readStudents(QVector<Student>& student);
    void readTeachers(QVector<Teacher>& teacher);
    void readDisciplines(QVector<Discipline>& discipl);
};

#endif // FLPARSER_H
