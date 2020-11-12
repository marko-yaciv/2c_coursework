#ifndef LEARNSYSTEM_H
#define LEARNSYSTEM_H

#include <QMainWindow>
#include <QVector>

#include "studentdialog.h"
#include "teacherdialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class LearnSystem; }
QT_END_NAMESPACE

class LearnSystem : public QMainWindow
{
    Q_OBJECT
StudentDialog* studDialog;
TeacherDialog* teachDialog;
Teacher newTeach;
QVector<QWidget*> teacherCoursesWidg;

public:
    LearnSystem(QWidget *parent = nullptr);
    ~LearnSystem();
    bool isAccountExist(const Student&);
    bool isAccountExist(const Teacher&);

    bool isStudentDataCorrect(const QVector<QString>& studentDataToEnter, QString accountPassword);
    bool isTeacherDataCorrect(const QVector<QString>& teacherDataToEnter, QString accountPassword);

    void addCoursesToTeacher(const Discipline& course);
    void  clearItems();

private slots:

    void on_signUpB_clicked();

    void on_iamstud_clicked();

    void on_iamteach_clicked();

    void on_logInB_clicked();

    void on_enterB_clicked();

    void on_backToRegister_clicked();

private:
    Ui::LearnSystem *ui;
};
#endif // LEARNSYSTEM_H
