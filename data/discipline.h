#ifndef DISCIPLINE_H
#define DISCIPLINE_H
#include <QString>
#include <QVector>
#include "teacher.h"
class Discipline
{
    QString name;
    QVector<Teacher> teachersOfDiscipline;
public:
    Discipline(QString& name);
    int addTeacher(const Teacher& teacher);
    void rename(QString& newName);

    QString getName() const;
    const QVector<Teacher>& getTeachersList() const;
};

#endif // DISCIPLINE_H
