#ifndef TEACHERDIALOG_H
#define TEACHERDIALOG_H

#include <QDialog>
#include "data/student.h"
#include "data/discipline.h"
#include "data/teacher.h"
namespace Ui {
class TeacherDialog;
}

class TeacherDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TeacherDialog(QWidget *parent = nullptr, const Teacher& teacher = Teacher());
    ~TeacherDialog();

private:
    Ui::TeacherDialog *ui;
};

#endif // TEACHERDIALOG_H
