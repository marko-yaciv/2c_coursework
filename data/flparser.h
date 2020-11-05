#ifndef FLPARSER_H
#define FLPARSER_H
#include <QFile>
#include "data/student.h"
#include "data/teacher.h"
#include "data/discipline.h"
class FlParser
{
    QString filename;
    QFile file;
public:
    FlParser(const QString& flname);
    void changeFilename(const QString& fname);

    void writeData(const QString&);
    void writeData(const Student&);
    void writeData(const Teacher&);
    //void writeData(const Discipline&);

    void readData(QList<QString>& data);
    void readData(QVector<Student>& students);
    void readData(QVector<Teacher>& teachers);
    void readData(QVector<Discipline>& disciplines);
};

#endif // FLPARSER_H
