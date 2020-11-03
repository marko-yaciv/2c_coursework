#ifndef TEACHERDIALOG_H
#define TEACHERDIALOG_H

#include <QDialog>

namespace Ui {
class TeacherDialog;
}

class TeacherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TeacherDialog(QWidget *parent = nullptr);
    ~TeacherDialog();

private:
    Ui::TeacherDialog *ui;
};

#endif // TEACHERDIALOG_H
