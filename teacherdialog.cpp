#include "teacherdialog.h"
#include "ui_teacherdialog.h"

extern const int COURSES;
extern QVector<QVector<Discipline>> allDisciplines;
//extern QVector<Teacher> allTeachers;
extern QVector<Student> allStudents;

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
