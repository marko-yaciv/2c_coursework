#include "learnsystem.h"
#include "ui_learnsystem.h"

#include <QMessageBox>
#include <QCheckBox>
#include "data/flparser.h"

const int COURSES = 4;
static short postNumber = 0;
QList<QString> groups;
QVector<Student> allStudents;
QVector<Teacher> allTeachers;
QVector<QVector<Discipline>> allDisciplines;
QMap<short,QString> postNames;

LearnSystem::LearnSystem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LearnSystem)
{
    ui->setupUi(this);

    this->setMaximumSize(500,580);
    this->setBaseSize(500,550);

    ui->teachInfo->hide();
    ui->studGroup->hide();
    ui->enterB->hide();
    ui->backToRegister->hide();
    ui->group->addItems(groups);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setContentsMargins(1,0,0,1);
    for(auto& courseDiscipls: allDisciplines){
        for(auto& discipl: courseDiscipls){
            QCheckBox* courseName = new QCheckBox;
            courseName->setText(discipl.getName());
            layout->addWidget(courseName);
            layout->update();
            teacherCoursesWidg.append(courseName);

            connect(courseName,&QCheckBox::clicked,this,[=](){
               if(courseName->isChecked())
                    addCoursesToTeacher(discipl);
            });
        }
    }
    ui->scrollAreaWidgetContents->setLayout(layout);


}
LearnSystem::~LearnSystem()
{
    FlParser studWriter(STUDENTS_FILE);
    studWriter.writeStudents(allStudents);
    FlParser teacherWriter(TEACHERS_FILE);
    teacherWriter.writeTeachers(allTeachers);
    delete ui;
}

void LearnSystem::clearItems(){
    ui->Fname->clear();
    ui->Lname->clear();
    ui->FthName->clear();
    ui->password->clear();
    ui->group->clear();
    ui->post->clear();
    ui->stand->clear();
}

void LearnSystem::addCoursesToTeacher(const Discipline &course)
{
    newTeach.setDiscipline(course);
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
        if(std::find(allStudents.begin(),allStudents.end(),newStudent) != allStudents.end()){
            QMessageBox::warning(this,"Failed to Create",
                                 "Failed to create account."
                                 "It is already exists. "
                                 "Try to enter your account");
            return;
        }

        allStudents.append(newStudent);

        studDialog = new StudentDialog(this,allStudents.last());
        clearItems();
        studDialog->show();

    }else if(ui->iamteach->isChecked()){
        postNames.insert(postNumber++,ui->post->currentText());
        Teacher newTeacher(ui->Fname->text(),
                           ui->Lname->text(),
                           ui->FthName->text(),
                           ui->post->currentText(),
                           ui->stand->text().toInt(),
                           ui->password->text());
        for(auto &i : *newTeach.getDissciplines()){
            newTeacher.setDiscipline(i);
        }
        if(std::find(allTeachers.begin(),allTeachers.end(),newTeacher) != allTeachers.end()){
            QMessageBox::warning(this,"Failed to Create",
                                 "Failed to create account."
                                 "It is already exists. "
                                 "Try to enter your account");
            return;
        }
        allTeachers.append(newTeacher);

        teachDialog = new TeacherDialog(this,allTeachers.last());
        clearItems();
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
            studDialog = new StudentDialog(this,*studInList);
            clearItems();
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
        auto teachInList = std::find(allTeachers.begin(),allTeachers.end(),TeachToEnter);

        if(teachInList != allTeachers.end()){
            teachDialog = new TeacherDialog(this,*teachInList);
            clearItems();
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
