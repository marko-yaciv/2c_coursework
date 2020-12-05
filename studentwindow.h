#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QtAlgorithms>
#include <QMessageBox>
#include <QPushButton>
#include <QMenuBar>
#include <QLayout>
#include <QStyle>
#include <QMenu>

#include "data/student.h"
#include "data/discipline.h"
#include "data/teacher.h"
#include "data/studyprocessdata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StudentDialog; }
QT_END_NAMESPACE

/*This class is for Student's personal cabinet*/
class StudentDialog : public QDialog
{
    Q_OBJECT

    int m_course;
    Student m_pageOwner;
    StudyProcessData* m_allStudyProcessData;

    QMap<QWidget*, QPushButton*> m_noTeacherButtons;
    QMap<Discipline, QWidget*>   m_studCoursesWidgets;
    QMap<QWidget*, QPushButton*> m_popularTeacherButtons;
public:
    explicit StudentDialog(QWidget *parent = nullptr, const Student& stud = Student());
    ~StudentDialog();

    /*functions for setting
    enviroment for window*/
    void showCourseMap();
    void showOwnerInfo();
    void updateOwnersMap(int course);
    void showNoTeacherButton(QPushButton* noTeacherBut);
    void setHeaders(const QList<QString>& name, QTableWidget* table);

    /*functions to determine
    which teachers to show in table*/
    void showTeachersList(const Discipline discipl);
    void showTeachersToChange(const Discipline& discipl);
    void showTheMostPopularTeacher(const Discipline& discipl);

    /*functions that add teacher to target,
    show it and, if needed, remove old one*/
    void removeOldTeacherFromWidget(QWidget* where);
    void showTeacherUnderDiscipline(const Teacher teachItm, QWidget* where);
    QList<Teacher>::iterator findTeacherForTarget(const QTableWidgetItem*);
    void addTeacherToTarget(const QTableWidgetItem* itm,const Discipline discipl, QWidget* where);

    /*prints teachers in table*/
    void addTeachersToTable(const QList<Teacher>& teachers);
    void setWhatIsThisForTeacher(QString& whatIsThis, const Teacher& teacher);

    /*functions that consolidate
    teachers to show or to sort*/
    void minimiseTeachersVect(QList<Teacher>& teachers);
    void sortTeachers(QList<Teacher>& teacherList);
    void findFreeTeachers(QList<Teacher>& teachers, const QDate& selectedDate);
    void findTheMostPopularTeacher(Teacher& teacher, const Discipline& discipl);
    void findPageOwnerTeachers(QList<Teacher>& teachersToFind);
    void findTeachersWithOneCourse(QList<Teacher>& teachersToFind,const QList<Teacher>& allTeachers);

    /*custom slot*/
    void on_discipl_contxtMenuRequested(const Discipline& discipl);
private:
    QMetaObject::Connection add_target_connection;
    QMetaObject::Connection change_target_connection;

private slots:
    void on_logout_clicked();

    void on_sortB_clicked();

    void on_showTeachMode_activated(const QString &arg1);

    void on_showByDateB_clicked();

    void on_calendarWidget_activated(const QDate &date);

    void on_calendarWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::StudentDialog *ui;
};

#endif // STUDENTDIALOG_H
