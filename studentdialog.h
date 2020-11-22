#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>
#include "data/student.h"
#include "data/discipline.h"
#include "data/teacher.h"
#include "data/studyprocessdata.h"
namespace Ui {
class StudentDialog;
}

class StudentDialog : public QDialog
{
    Q_OBJECT
    QMap<Discipline, QWidget*> studCoursesWidgets;
    Student pageOwner;
    int course;
    QMap<QWidget*, QPushButton*> noTeacherButtons;
    QMap<QWidget*, QPushButton*> popularTeacherButtons;
public:
    explicit StudentDialog(QWidget *parent = nullptr, const Student& stud = Student());
    ~StudentDialog();

    void updateOwnersMap(int course);

    void showTeacherUnderDiscipline(const Teacher teachItm, QWidget* where);
    void addTeacherToTarget(const QTableWidgetItem* itm,const Discipline discipl, QWidget* where);
    void showTeachersList(const Discipline discipl);

    void showTeachersToChange(const Discipline& discipl);

    void showTheMostPopularTeacher(const Discipline& discipl);
    void on_discipl_contxtMenuRequested(const Discipline& discipl);

    void addTeachersToTable(const QVector<Teacher>& teachers);
    void minimiseTeachesVect(QVector<Teacher>& teachers);

    void setHeaders(const QList<QString>& name, QTableWidget* table);

    QMetaObject::Connection add_target_connection;
signals:
    void showMain();
private slots:
    void on_logout_clicked();

    //void on_showTeachMode_currentTextChanged(const QString &arg1);

    void on_sortB_clicked();

    void on_showTeachMode_activated(const QString &arg1);

    void on_showByDateB_clicked();

    void on_calendarWidget_activated(const QDate &date);

    void on_calendarWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::StudentDialog *ui;
};

#endif // STUDENTDIALOG_H
