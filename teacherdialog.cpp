#include "teacherdialog.h"
#include "ui_teacherdialog.h"

TeacherDialog::TeacherDialog(QWidget *parent, const Teacher& teacher) :
    QDialog(parent),
    ui(new Ui::TeacherDialog)
{
    ui->setupUi(this);
    auto inits = teacher.getInitials();
    ui->label->setText(inits->at(0));
    delete inits;
}

TeacherDialog::~TeacherDialog()
{
    delete ui;
}
