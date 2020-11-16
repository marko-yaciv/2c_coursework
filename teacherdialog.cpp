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
    ui->label->setText(teacher.getFname());
}

TeacherDialog::~TeacherDialog()
{
    delete ui;
}
