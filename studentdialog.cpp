#include "studentdialog.h"
#include "ui_studentdialog.h"

StudentDialog::StudentDialog(QWidget *parent, const Student& stud) :
    QDialog(parent),
    ui(new Ui::StudentDialog)
{
    ui->setupUi(this);
    ui->label->setText(stud.getInitials()->at(0));
}

StudentDialog::~StudentDialog()
{
    delete ui;
}
