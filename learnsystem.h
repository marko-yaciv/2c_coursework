#ifndef LEARNSYSTEM_H
#define LEARNSYSTEM_H

#include <QMainWindow>
#include <QDir>
#include <QVector>
#include <QCheckBox>
#include <QMessageBox>
#include <QRandomGenerator>
#include "studentdialog.h"
#include "teacherdialog.h"
#include "allmembers.h"
#include "newdiscipline.h"
#include "data/exept.h"
#include "data/flparser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LearnSystem; }
QT_END_NAMESPACE


/* Class for  registration  start window*/
class LearnSystem : public QMainWindow
{
    Q_OBJECT
StudyProcessData* m_allStudyProcessData;

StudentDialog* m_studDialog;
TeacherDialog* m_teachDialog;
AllMembers* m_membersWidget;
NewDiscipline* m_newDisciplDialog;

QList<Discipline> m_registrDiscipls;
QVector<QCheckBox*> m_teacherCoursesWidg;

public:
    LearnSystem(QWidget *parent = nullptr);
    ~LearnSystem();

    bool isAccountExist(const Student&);
    bool isAccountExist(const Teacher&);
    void makeNewMemberParticular(Student&);
    void makeNewMemberParticular(Teacher&);

    void addCourseToTeacher(const Discipline& course);
    void removeCourseFromTeacher(const Discipline& course);
    void  clearItems();

    void validateNamesForEnter();
    void validateDataForSignUp();
    void showDisciplinesToChoose();
    void updateStudentsWithDiscipline(const int course);
private slots:

    void on_signUpB_clicked();

    void on_iamstud_clicked();

    void on_iamteach_clicked();

    void on_logInB_clicked();

    void on_enterB_clicked();

    void on_backToRegister_clicked();

    void on_actionShow_All_Teachers_triggered();

    void on_actionShow_All_Students_triggered();

    void on_showPassword_pressed();

    void on_showPassword_released();

    void on_actionDiscipline_triggered();

private:
    Ui::LearnSystem *ui;
};
#endif // LEARNSYSTEM_H
