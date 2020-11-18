#ifndef ALLMEMBERS_H
#define ALLMEMBERS_H

#include <QWidget>
#include "data/student.h"
#include "data/teacher.h"

extern QVector<Student> allStudents;
extern QVector<Teacher> allTeachers;
namespace Ui {
class allMembers;
}

class AllMembers : public QWidget
{
    Q_OBJECT

public:
    explicit AllMembers(QWidget *parent = nullptr, const QVector<Teacher>& members = QVector<Teacher>(0));
    explicit AllMembers(QWidget *parent = nullptr, const QVector<Student>& members = QVector<Student>(0));

    ~AllMembers();

private:
    Ui::allMembers *ui;
};

#endif // ALLMEMBERS_H
