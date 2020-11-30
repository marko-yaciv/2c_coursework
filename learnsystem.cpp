#include "learnsystem.h"
#include "ui_learnsystem.h"

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

    ui->teachInfo->hide();
    ui->studGroup->hide();
    ui->enterB->hide();
    ui->backToRegister->hide();
    ui->group->addItems(groups);
    ui->password->setEchoMode(QLineEdit::EchoMode::Password);
    ui->inLabel->setText("Sign Up");
    auto name_validator = new QRegExpValidator( QRegExp( "[А-і-І-ї-я]{1,40}" )) ;
    ui->Fname->setValidator(name_validator);
    ui->Lname->setValidator(name_validator);
    ui->FthName->setValidator(name_validator);

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

    for(auto& j : allTeachers){
        allStudyProcessData.updateMapForTeacher(j,j.getCourseMap());
    }
    for(auto& i : allStudents){
        allStudyProcessData.updateMapForStudent(i,i.getStudyMap());
    }

    auto dir = QDir::currentPath();
    FlParser dataSaver;
    try {
        dataSaver.changeFilename(dir + "/datafiles/dcourse1.json");
        dataSaver.writeDisciplines(allDisciplines[0]);

        dataSaver.changeFilename(dir + "/datafiles/dcourse2.json");
        dataSaver.writeDisciplines(allDisciplines[1]);

        dataSaver.changeFilename(dir + "/datafiles/dcourse3.json");
        dataSaver.writeDisciplines(allDisciplines[2]);

        dataSaver.changeFilename(dir + "/datafiles/dcourse4.json");
        dataSaver.writeDisciplines(allDisciplines[3]);

        dataSaver.changeFilename(dir + STUDENTS_FILE);
        dataSaver.writeStudents(allStudents);

        dataSaver.changeFilename(dir + TEACHERS_FILE);
        dataSaver.writeTeachers(allTeachers);

        dataSaver.changeFilename(dir + STUD_STUDY_PROCESS_FILE);
        dataSaver.writeStudentStudyProcessData(allStudyProcessData);

        dataSaver.changeFilename(dir + TEACHER_STUDY_PROCESS_FILE);
        dataSaver.writeTeacherStudyProcessData(allStudyProcessData);

    }  catch (Except& msg) {
        delete ui;
        QMessageBox::critical(nullptr,"Fatal",msg.what());
    }
}

void LearnSystem::makeNewMemberParticular(Student & membToCheck)
{
    for(auto student = allStudents.begin(); student != allStudents.end();){
        if(student->getId() == membToCheck.getId()){
            membToCheck.setId(QRandomGenerator::global()->bounded(1001,8988));
        }else{
            ++student;
        }
    }
}
void LearnSystem::makeNewMemberParticular(Teacher & membToCheck)
{
    for(auto teacher = allTeachers.begin(); teacher!= allTeachers.end();){
        if(teacher->getId() == membToCheck.getId()){
            membToCheck.setId(QRandomGenerator::global()->bounded(1001,8988));
        }else{
            ++teacher;
        }
    }
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

void LearnSystem::validateNamesForEnter()
{
    if(ui->Fname->text().isEmpty() ||
            ui->Lname->text().isEmpty() ||
            ui->FthName->text().isEmpty() ||
            ui->password->text().isEmpty()){
        throw Except("Please, fill all fields and try again)");
    }
}

void LearnSystem::validateDataForSignUp()
{
    if(ui->teachInfo->isHidden() && ui->iamteach->isChecked()){
        throw Except("Please, choose all data and try again)");
    }
    if(ui->studGroup->isHidden() && ui->iamstud->isChecked()){
        throw Except("Please, choose all data and try again)");
    }
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
    ui->inLabel->setText("Log In");
}

void LearnSystem::on_signUpB_clicked()
{
    try {
        validateNamesForEnter();
        validateDataForSignUp();
    }  catch (Except& ex) {
        QMessageBox::warning(this,"Registration failed", ex.what());
        return;
    }

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
                                 "Try to log In your account");
            return;
        }
        makeNewMemberParticular(newStudent);
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
                                 "Try to log In your account");
            return;
        }
        makeNewMemberParticular(newTeacher);
        allTeachers.append(newTeacher);

//--------------------shows teacher's window--------------------------
        teachDialog = new TeacherDialog(nullptr,allTeachers.last());
        connect(teachDialog,&TeacherDialog::finished,this,&QMainWindow::show);
        clearItems();
        teachDialog->open();
    }
    else{
        QMessageBox::warning(this, "Registration failed", tr("Please, choose, who you are (teacher or student)")+
                                                    tr("And try again"));
        return;
    }
    this->close();
}

void LearnSystem::on_enterB_clicked()
{
    try {
        validateNamesForEnter();
    }  catch (Except& ex) {
        QMessageBox::warning(this,"Logging in failed", ex.what());
        return;
    }

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
    else {
        QMessageBox::warning(this, "Log in failed", tr("Please, choose, who you are (teacher or student)")+
                                                    tr("And try again"));
        return;
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
    ui->inLabel->setText("Gign Up");
}

void LearnSystem::on_actionShow_All_Teachers_triggered()
{
    membersWidget = new AllMembers(nullptr, allTeachers);
    membersWidget->show();
}

void LearnSystem::on_actionShow_All_Students_triggered()
{
    membersWidget = new AllMembers(nullptr, allStudents);
    membersWidget->show();
}

void LearnSystem::on_showPassword_pressed()
{
    ui->password->setEchoMode(QLineEdit::EchoMode::Normal);
}

void LearnSystem::on_showPassword_released()
{
    ui->password->setEchoMode(QLineEdit::EchoMode::Password);
}
