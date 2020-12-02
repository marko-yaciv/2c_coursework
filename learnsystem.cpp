#include "learnsystem.h"
#include "ui_learnsystem.h"

LearnSystem::LearnSystem(QWidget *parent)
    : QMainWindow(parent),
      m_allStudyProcessData(StudyProcessData::getInstance()),
      ui(new Ui::LearnSystem)
{
    ui->setupUi(this);

    ui->teachInfo->hide();
    ui->studGroup->hide();
    ui->enterB->hide();
    ui->backToRegister->hide();
    ui->group->addItems(m_allStudyProcessData->getGroups());
    ui->password->setEchoMode(QLineEdit::EchoMode::Password);
    ui->inLabel->setText("Sign Up");
    auto name_validator = new QRegExpValidator(QRegExp("[А-і-І-ї-я]{1,40}")) ;
    ui->Fname->setValidator(name_validator);
    ui->Lname->setValidator(name_validator);
    ui->FthName->setValidator(name_validator);

    showDisciplinesToChoose();


    ui->post->addItems(m_allStudyProcessData->getPostNames().values());

}
LearnSystem::~LearnSystem()
{
    m_allStudyProcessData = StudyProcessData::getInstance();
    auto& allDisciplines = m_allStudyProcessData->getAllDisciplines();
    auto& allStudents = m_allStudyProcessData->getAllStudents();
    auto& allTeachers = m_allStudyProcessData->getAllTeachers();

    for(auto& teacher : allTeachers){
        m_allStudyProcessData->updateMapForTeacher(teacher,teacher.getCourseMap());
    }
    for(auto& student : allStudents){
        m_allStudyProcessData->updateMapForStudent(student,student.getStudyMap());
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
        dataSaver.writeStudentStudyProcessData(m_allStudyProcessData);

        dataSaver.changeFilename(dir + TEACHER_STUDY_PROCESS_FILE);
        dataSaver.writeTeacherStudyProcessData(m_allStudyProcessData);

    }  catch (Except& msg) {
        delete ui;
        QMessageBox::critical(nullptr,"Fatal",msg.what());
        return;
    }
    delete StudyProcessData::getInstance();
    delete ui;
}

void LearnSystem::showDisciplinesToChoose()
{
    m_teacherCoursesWidg.clear();
    qDeleteAll(ui->scrollAreaWidgetContents->children());

    QVBoxLayout* layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    layout->setMargin(0);
    layout->setContentsMargins(1,0,0,1);
    for(auto& courseDiscipls: m_allStudyProcessData->getAllDisciplines()){
        for(auto& discipl: courseDiscipls){
            QCheckBox* courseName = new QCheckBox;
            courseName->setText(discipl.getName());
            layout->addWidget(courseName);
            layout->update();

            m_teacherCoursesWidg.append(courseName);

            connect(courseName,&QCheckBox::toggled,this,[&](bool checked){
               if(checked)
                    addCourseToTeacher(discipl);
               else{
                   removeCourseFromTeacher(discipl);
               }
            });
        }
    }
    ui->scrollAreaWidgetContents->setLayout(layout);
}

void LearnSystem::updateStudentsWithDiscipline(const int course)
{
    for(auto&i : m_allStudyProcessData->getAllStudents())
    {
        if(i.getCourse() == course)
        {
            for(auto &j: m_allStudyProcessData->getAllDisciplines()[course-1])
            {
                i.addDiscipline(j);
            }
        }
    }
}

void LearnSystem::makeNewMemberParticular(Student& membToCheck)
{
    for(auto student = m_allStudyProcessData->getAllStudents().begin();
             student != m_allStudyProcessData->getAllStudents().end();)
    {
        if(student->getId() == membToCheck.getId())
            membToCheck.setId(QRandomGenerator::global()->bounded(1001,8988));
        else
            ++student;
    }
}
void LearnSystem::makeNewMemberParticular(Teacher & membToCheck)
{
    for(auto teacher = m_allStudyProcessData->getAllTeachers().begin();
             teacher!= m_allStudyProcessData->getAllTeachers().end();)
    {
        if(teacher->getId() == membToCheck.getId())
            membToCheck.setId(QRandomGenerator::global()->bounded(1001,8988));
        else
            ++teacher;
    }
}

void LearnSystem::clearItems(){
    ui->Fname->clear();
    ui->Lname->clear();
    ui->FthName->clear();
    ui->password->clear();
    ui->stand->clear();
    for(auto&i: m_teacherCoursesWidg){
        i->setChecked(false);
    }
    m_registrDiscipls.clear();
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



void LearnSystem::addCourseToTeacher(const Discipline &course)
{
    m_registrDiscipls.append(course);
}

void LearnSystem::removeCourseFromTeacher(const Discipline& course)
{
    m_registrDiscipls.removeOne(course);
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

    auto& allStudents = m_allStudyProcessData->getAllStudents();
    auto& allTeachers = m_allStudyProcessData->getAllTeachers();

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
        m_studDialog = new StudentDialog(nullptr,allStudents.last());
        connect(m_studDialog,&StudentDialog::finished,this,&QMainWindow::show);
        clearItems();
        m_studDialog->open();

    }else if(ui->iamteach->isChecked()){
        Teacher newTeacher(ui->Fname->text(),
                           ui->Lname->text(),
                           ui->FthName->text(),
                           ui->post->currentText(),
                           ui->stand->text().toInt(),
                           ui->password->text());
        if(m_registrDiscipls.isEmpty()){
            QMessageBox::warning(this,"Failed to Create","Plese choose disciplines");
            return;
        }
        for(auto &i : m_registrDiscipls){
            newTeacher.addDiscipline(i);
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
        m_teachDialog = new TeacherDialog(nullptr,allTeachers.last());
        connect(m_teachDialog,&TeacherDialog::finished,this,&QMainWindow::show);
        clearItems();
        m_teachDialog->open();
    }
    else{
        QMessageBox::warning(this, "Registration failed", tr("Please, choose, who you are (teacher or student) ")+
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

    auto& allStudents = m_allStudyProcessData->getAllStudents();
    auto& allTeachers = m_allStudyProcessData->getAllTeachers();

    if(ui->iamstud->isChecked()){
        Student studToEnter(ui->Fname->text(),
                            ui->Lname->text(),
                            ui->FthName->text(),
                            ui->password->text());

        auto studInList = std::find(allStudents.begin(),allStudents.end(),studToEnter);

        if(studInList != allStudents.end()){
            m_studDialog = new StudentDialog(nullptr,*studInList);

            connect(m_studDialog,&StudentDialog::finished,this,&QMainWindow::show);
            clearItems();
            m_studDialog->open();
        }else{
            QMessageBox::warning(this,"Entering failed",
                                 "The student with entered initials doesn't exist,"
                                 "or password is incorrect."
                                 "Please, check the inputs and try again "
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
            m_teachDialog = new TeacherDialog(nullptr,*teachInList);
            connect(m_teachDialog,&TeacherDialog::finished,this,&QMainWindow::show);
            clearItems();
            m_teachDialog->open();
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
    ui->inLabel->setText("Sign Up");
}

void LearnSystem::on_actionShow_All_Teachers_triggered()
{
    m_membersWidget = new AllMembers(nullptr, m_allStudyProcessData->getAllTeachers());
    m_membersWidget->show();
}

void LearnSystem::on_actionShow_All_Students_triggered()
{
    m_membersWidget = new AllMembers(nullptr, m_allStudyProcessData->getAllStudents());
    m_membersWidget->show();
}

void LearnSystem::on_showPassword_pressed()
{
    ui->password->setEchoMode(QLineEdit::EchoMode::Normal);
}

void LearnSystem::on_showPassword_released()
{
    ui->password->setEchoMode(QLineEdit::EchoMode::Password);
}

void LearnSystem::on_actionDiscipline_triggered()
{
    m_createDisciplDialog = new NewDiscipline(this,m_allStudyProcessData->getAllDisciplines());
    connect(m_createDisciplDialog,&NewDiscipline::finished,this,[&](){showDisciplinesToChoose();
                                                                    updateStudentsWithDiscipline(m_createDisciplDialog->getNewDisciplineCourse());});
    m_createDisciplDialog->open();
}
