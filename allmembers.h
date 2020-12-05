#ifndef ALLMEMBERS_H
#define ALLMEMBERS_H

#include <QWidget>
#include "data/student.h"
#include "data/teacher.h"

namespace Ui {
class allMembers;
}
/*This class created for opening widget to
view all members that have been registered*/
class AllMembers : public QWidget
{
    Q_OBJECT

public:
    explicit AllMembers(QWidget *parent = nullptr, const QList<Teacher>& members = QList<Teacher>());
    explicit AllMembers(QWidget *parent = nullptr, const QList<Student>& members = QList<Student>());

    void showMembersData(QString lname, QString fname, int id);
    void setHeaders();

    ~AllMembers();

private:
    Ui::allMembers *ui;
};

#endif // ALLMEMBERS_H
