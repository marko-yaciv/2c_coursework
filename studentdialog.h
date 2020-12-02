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

    int m_course;
    Student m_pageOwner;
    StudyProcessData* m_allStudyProcessData;

    QMap<QWidget*, QPushButton*> m_noTeacherButtons;
    QMap<Discipline, QWidget*>   m_studCoursesWidgets;
    QMap<QWidget*, QPushButton*> m_popularTeacherButtons;
public:
    explicit StudentDialog(QWidget *parent = nullptr, const Student& stud = Student());
    ~StudentDialog();

    void updateOwnersMap(int course);
    void showCourseMap();
    void showNoTeacherButton(QPushButton* noTeacherBut);
    void showOwnerInfo();

    void setWhatIsThisForTeacher(QString& whatIsThis, const Teacher& teacher);
    void showTeacherUnderDiscipline(const Teacher teachItm, QWidget* where);
    void addTeacherToTarget(const QTableWidgetItem* itm,const Discipline discipl, QWidget* where);
    void showTeachersList(const Discipline discipl);

    void showTeachersToChange(const Discipline& discipl);

    void showTheMostPopularTeacher(const Discipline& discipl);
    void findTheMostPopularTeacher(Teacher& teacher, const Discipline& discipl);
    void removeOldTeacherFromWidget(QWidget* where);

    void on_discipl_contxtMenuRequested(const Discipline& discipl);

    void addTeachersToTable(const QList<Teacher>& teachers);
    void minimiseTeachesVect(QList<Teacher>& teachers);

    void setHeaders(const QList<QString>& name, QTableWidget* table);

    void findFreeTeachers(QList<Teacher>& teachers, const QDate& selectedDate);

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
