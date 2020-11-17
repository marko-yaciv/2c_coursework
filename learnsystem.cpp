#include "learnsystem.h"
#include "ui_learnsystem.h"

#include "data/flparser.h"

const int COURSES = 4;
QList<QString> groups;

QVector<Student> allStudents;
QVector<Teacher> allTeachers;
QVector<QVector<Discipline>> allDisciplines;

QMap<short,QString> postNames;

StudyProcessData allStudyProcessData;

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

            connect(courseName,&QCheckBox::toggled,this,[=](bool checked){
               if(checked)
                    addCoursesToTeacher(discipl);
            });
        }
    }
    ui->scrollAreaWidgetContents->setLayout(layout);
    postNames = decltype (postNames)({std::pair<short,QString>(0,"labAssist"),
                                      std::pair<short,QString>(1,"teacher"),
                                      std::pair<short,QString>(2,"seniorTeacher"),
                                      std::pair<short,QString>(3,"docent"),
                                      std::pair<short,QString>(4,"professor")});
    ui->post->addItems(postNames.values());

}
LearnSystem::~LearnSystem()
{
    for(auto&i:allStudents){
        allStudyProcessData.updateMapForStudent(i,i.getStudyMap());
    }
    for(auto&i:allTeachers){
        allStudyProcessData.updateMapForTeacher(i,i.getVisitors());
    }

    FlParser dataSaver;
    dataSaver.writeStudents(allStudents);
    dataSaver.writeTeachers(allTeachers);
    dataSaver.writeStudyProcess(allStudyProcessData);
    delete ui;
}

void LearnSystem::clearItems(){
    ui->Fname->clear();
    ui->Lname->clear();
    ui->FthName->clear();
    ui->password->clear();
    ui->stand->clear();
    for(auto&i: teacherCoursesWidg){
        i->setChecked(false);
    }
    registrDiscipls.clear();
}

void LearnSystem::addCoursesToTeacher(const Discipline &course)
{
    registrDiscipls.append(course);
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

//------------Shows student's window---------------------------------
        studDialog = new StudentDialog(nullptr,allStudents.last());
        connect(studDialog,&StudentDialog::finished,this,&QMainWindow::show);
        clearItems();
        studDialog->open();

    }else if(ui->iamteach->isChecked()){
        Teacher newTeacher(ui->Fname->text(),
                           ui->Lname->text(),
                           ui->FthName->text(),
                           ui->post->currentText(),
                           ui->stand->text().toInt(),
                           ui->password->text());
        for(auto &i : registrDiscipls){
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

//--------------------shows teacher's window--------------------------
        teachDialog = new TeacherDialog(nullptr,allTeachers.last());
        connect(teachDialog,&TeacherDialog::finished,this,&QMainWindow::show);
        clearItems();
        teachDialog->open();
    }
    this->close();
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

            connect(studDialog,&StudentDialog::finished,this,&QMainWindow::show);
            clearItems();
            studDialog->open();
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
            teachDialog = new TeacherDialog(nullptr,*teachInList);
            connect(teachDialog,&TeacherDialog::finished,this,&QMainWindow::show);
            clearItems();
            teachDialog->open();
        }else{
            QMessageBox::warning(this,"Entering failed",
                                 "The teacher with entered initials doesn't exist,"
                                 "or password is incorrect."
                                 "Please, check the inputs and try again"
                                 "or sign up");
            return;
        }

    }
    this->close();
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
