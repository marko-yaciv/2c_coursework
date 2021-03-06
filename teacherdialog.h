#ifndef TEACHERDIALOG_H
#define TEACHERDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "data/student.h"
#include "data/discipline.h"
#include "data/teacher.h"
#include "data/studyprocessdata.h"
namespace Ui {
class TeacherDialog;
}

class TeacherDialog : public QDialog
{
    Q_OBJECT

    Teacher pageOwner;

    QList<QString> headers;
public:
    explicit TeacherDialog(QWidget *parent = nullptr, const Teacher& teacher = Teacher());
    ~TeacherDialog();

    void showStudentsList(const QVector<Student>& students);
    void setHeaders(const QList<QString>& name, QTableWidget* table);
signals:
    void showMain();
private slots:
    void on_logout_clicked();
    void on_showStudMode_activated(const QString &arg1);

    void on_sortB_clicked();

    void on_courses_itemDoubleClicked(QTableWidgetItem *item);

private:
    Ui::TeacherDialog *ui;
};

#endif // TEACHERDIALOG_H
