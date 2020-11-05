#include "learnsystem.h"
#include "ui_learnsystem.h"

#include <QMessageBox>

#include "data/flparser.h"
#define GROUPS_FILE "datafiles/groups.txt"
#define STUDENTS_FILE "datafiles/students.txt"
#define TEACHERS_FILE "datafiles/teachers.txt"

const int COURSES = 4;
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

    for(int i = 1; i <=COURSES; ++i){
        dataReader.changeFilename("datafiles/course" + QString::number(i) + "_discipl.txt");
        dataReader.readData(allDisciplines);
    }

    for(auto discipl: allDisciplines){
        ui->discipline->addItem(discipl.getName());
    }

}
LearnSystem::~LearnSystem()
{
    delete ui;
}

bool LearnSystem::isAccountExist(const Student& newStudent)
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

bool LearnSystem::isStudentDataCorrect(const QVector<QString> &studentDataToEnter, QString accountPassword)
{


    for(auto student : allStudents){
        auto inits = student.getInitials();
        if(studentDataToEnter == *inits){
            delete inits;
            if(accountPassword == student.getPasword()){
                studDialog = new StudentDialog(nullptr,student);
                return true;
            }else{
                QMessageBox::warning(this,"Entering failed",
                                     "Probably you entered incorrect password."
                                     "Please, check and try again");
                return false;
            }
        }
    }
    return false;
}

bool LearnSystem::isTeacherDataCorrect(const QVector<QString> &teacherDataToEnter, QString accountPassword)
{
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
        teachDialog->show();
    }
    this->hide();
}

void LearnSystem::on_enterB_clicked()
{

    if(ui->iamstud->isChecked()){
        Student studToEnter(ui->Fname->text(),
                            ui->Lname->text(),
                            ui->FthName->text(),
                            ui->password->text());


        auto studInList = std::find(allStudents.begin(),allStudents.end(),studToEnter);

        if(studInList != allStudents.end()){
            studDialog = new StudentDialog(nullptr,*studInList);
            studDialog->show();
        }else{
            QMessageBox::warning(this,"Entering failed",
                                 "The student with entered initials doesn't exist,"
                                 "or password is incorrect."
                                 "Please, check the inputs and try again"
                                 "or sign up");
            return;
        }
    }else if(ui->iamteach->isChecked()){
        Teacher TeachToEnter(ui->Fname->text(),
                            ui->Lname->text(),
                            ui->FthName->text(),
                            ui->password->text());
        auto teachInList = std::find(allTeachers.begin(),allTeachers.end()+1,TeachToEnter);

        if(teachInList != allTeachers.end()+1){
            teachDialog = new TeacherDialog(nullptr,*teachInList);
            teachDialog->show();
        }else{
            QMessageBox::warning(this,"Entering failed",
                                 "The teacher with entered initials doesn't exist,"
                                 "or password is incorrect."
                                 "Please, check the inputs and try again"
                                 "or sign up");
            return;
        }

    }
    this->hide();
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
