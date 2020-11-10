#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include "data/student.h"
#include "data/discipline.h"
#include "data/teacher.h"

namespace Ui {
class StudentDialog;
}

class StudentDialog : public QDialog
{
    Q_OBJECT
    QMap<Discipline, QWidget*> studentCourses;
    Student pageOwner;
    int course;
    QMap<QWidget*, QPushButton*> noTeacherButtons;
public:
    explicit StudentDialog(QWidget *parent = nullptr, const Student& stud = Student());
    ~StudentDialog();

    void updateOwnersMap(int course);

    void showTeacherUnderDiscipline(const Teacher& teachItm, QWidget* where);
    void addTeacherToTarget(const Teacher& teachItm, QWidget* where);
    void showTeachersList(const Discipline& discipl);

private slots:
    void on_discipline_customContextMenuRequested(const QPoint& pos);
    void on_logout_clicked();

private:
    Ui::StudentDialog *ui;
};

#endif // STUDENTDIALOG_H
