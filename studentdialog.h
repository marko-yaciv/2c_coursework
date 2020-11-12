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
    QMap<QWidget*, QPushButton*> popularTeacherButtons;
public:
    explicit StudentDialog(QWidget *parent = nullptr, const Student& stud = Student());
    ~StudentDialog();

    void updateOwnersMap(int course);

    void showTeacherUnderDiscipline(const Teacher& teachItm, QWidget* where);
    void addTeacherToTarget(const Teacher& teachItm, QWidget* where);
    void showTeachersList(const Discipline& discipl);
    void showThePopulestTeacher(const Discipline& discipl);
    void on_discipl_contxtMenuRequested(const Discipline& discipl);

    void addTeachersToList(QVector<Teacher>& teachers);
signals:
    void showMain();
private slots:
    void on_logout_clicked();

    //void on_showTeachMode_currentTextChanged(const QString &arg1);

    void on_sortB_clicked();

    void on_showTeachMode_activated(const QString &arg1);
private:
    Ui::StudentDialog *ui;
};

#endif // STUDENTDIALOG_H
