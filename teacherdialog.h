#ifndef TEACHERDIALOG_H
#define TEACHERDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QCheckBox>
#include <QMenu>
#include "data/student.h"
#include "data/discipline.h"
#include "data/teacher.h"
#include "data/studyprocessdata.h"
namespace Ui {
class TeacherDialog;
}

/*This class is for Teacher's personal cabinet*/
class TeacherDialog : public QDialog
{
    Q_OBJECT

    Teacher pageOwner;
    StudyProcessData* m_allStudyProcessData;
    QList<QString> headers;
public:
    explicit TeacherDialog(QWidget *parent = nullptr, const Teacher& teacher = Teacher());
    ~TeacherDialog();

    /*functions to show students*/
    void showStudentsList(const QList<Student>& students);
    void setHeaders(const QList<QString>& name, QTableWidget* table);
    void minimiseStudentVect(QList<Student> &teachers);

    /*helper functions to set enviroment*/
    void showDisciplinesToChoose();
    void showOwnerDisciplines();

    void commitChanges();

private slots:
    void on_logout_clicked();
    void on_showStudMode_activated(const QString &arg1);

    void on_sortB_clicked();

    void on_courses_itemDoubleClicked(QTableWidgetItem *item);

    void on_pushButton_clicked();

    void on_scrollArea_customContextMenuRequested(const QPoint &pos);

private:
    Ui::TeacherDialog *ui;
};

#endif // TEACHERDIALOG_H
