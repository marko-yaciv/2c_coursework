#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include "data/student.h"
#include "data/discipline.h"
#include "data/teacher.h"
namespace Ui {
class StudentDialog;
}

class StudentDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StudentDialog(QWidget *parent = nullptr, const Student& stud = Student());
    ~StudentDialog();

private:
    Ui::StudentDialog *ui;
};

#endif // STUDENTDIALOG_H
