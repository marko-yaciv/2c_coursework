#include "learnsystem.h"
#include "ui_learnsystem.h"

#include <QMessageBox>

#include "data/flparser.h"
#define GROUPS_FILE "groups.txt"
#define STUDENTS_FILE "students.txt"
#define TEACHERS_FILE "teachers.txt"

LearnSystem::LearnSystem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LearnSystem)
{
    ui->setupUi(this);


    ui->teachInfo->hide();
    ui->studGroup->hide();
    ui->enterB->hide();
    ui->backToRegister->hide();

    QStringList groups;
    FlParser dataReader(GROUPS_FILE);

    dataReader.readData(groups);
    ui->group->addItems(groups);

    dataReader.changeFilename(STUDENTS_FILE);
    dataReader.readData(allStudents);

    dataReader.changeFilename(TEACHERS_FILE);
    dataReader.readData(allTeachers);

}
LearnSystem::~LearnSystem()
{
    delete ui;
}

bool LearnSystem::isAccountExist(const Student & newStudent)
{
    for(auto&i:allStudents){
        if(i == newStudent){
            return true;
        }
    }
    return false;
}

bool LearnSystem::isAccountExist(const Teacher & newTeacher)
{
    for(auto&i:allTeachers){
        if(i == newTeacher){
            return true;
        }
    }
    return false;
}


void LearnSystem::on_iamstud_clicked()
{
    if(ui->logInB->isEnabled()){
        ui->teachInfo->hide();
        ui->studGroup->show();
        ui->signUpB->show();
        ui->enterB->hide();
    }
}

void LearnSystem::on_iamteach_clicked()
{
    if(ui->logInB->isEnabled()){
        ui->studGroup->hide();
        ui->teachInfo->show();
        ui->signUpB->show();
        ui->enterB->hide();
    }
}

void LearnSystem::on_logInB_clicked()
{
    ui->studGroup->hide();
    ui->teachInfo->hide();
    ui->signUpB->hide();
    ui->enterB->show();
    ui->logInWidget->hide();
    ui->logInB->setEnabled(false);
    ui->backToRegister->show();
}


void LearnSystem::on_signUpB_clicked()
{
    if(ui->iamstud->isChecked()){
        Student newStudent(ui->Fname->text(),
                           ui->Lname->text(),
                           ui->FthName->text(),
                           ui->group->currentText(),
                           ui->password->text());
        if(isAccountExist(newStudent)){
            QMessageBox::warning(this,"Failed to Create",
                                 "Failed to create account."
                                 "It is already exists. "
                                 "Try to enter your account");
            return;
        }

        allStudents.append(newStudent);
        FlParser writeStudent(STUDENTS_FILE);
        writeStudent.writeData(newStudent);

        studDialog = new StudentDialog(nullptr,allStudents.last());
        this->hide();
        studDialog->show();

    }else if(ui->iamteach->isChecked()){
        Teacher newTeacher(ui->Fname->text(),
                           ui->Lname->text(),
                           ui->FthName->text(),
                           ui->post->text(),
                           ui->stand->text().toInt(),
                           ui->password->text());
        if(isAccountExist(newTeacher)){
            QMessageBox::warning(this,"Failed to Create",
                                 "Failed to create account."
                                 "It is already exists. "
                                 "Try to enter your account");
            return;
        }

        allTeachers.append(newTeacher);
        FlParser writeTeacher(TEACHERS_FILE);
        writeTeacher.writeData(newTeacher);

        teachDialog = new TeacherDialog(nullptr,allTeachers.last());
        this->hide();
        teachDialog->show();
    }
}

void LearnSystem::on_enterB_clicked()
{
    if(ui->iamstud->isChecked()){
/*        studDialog = new StudentDialog(nullptr, );
        this->hide();
        studDialog->show()*/;
    }else if(ui->iamteach->isChecked()){
//        teachDialog = new TeacherDialog;
//        this->hide();
//        teachDialog->show();
    }
}

void LearnSystem::on_backToRegister_clicked()
{
    if(ui->iamstud->isChecked()){
        ui->teachInfo->hide();
        ui->studGroup->show();
        ui->signUpB->show();
        ui->enterB->hide();
    }else if(ui->iamteach->isChecked()){
        ui->studGroup->hide();
        ui->teachInfo->show();
        ui->signUpB->show();
        ui->enterB->hide();
    }
    ui->logInWidget->show();
    ui->logInB->setEnabled(true);
    ui->backToRegister->hide();
}
