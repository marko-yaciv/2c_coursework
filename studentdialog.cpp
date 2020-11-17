#include "studentdialog.h"
#include "ui_studentdialog.h"
#include <QMenu>
#include <QPushButton>
#include <QLayout>
#include <QMenuBar>
#include <QtAlgorithms>
#include <QMessageBox>
extern const int COURSES;

extern QVector<QVector<Discipline>> allDisciplines;
extern QVector<Teacher> allTeachers;
extern QVector<Student> allStudents;

extern StudyProcessData allStudyProcessData;

QList<QString> headers({"Initials","Post","Stage","Popularity"});

StudentDialog::StudentDialog(QWidget *parent, const Student& stud) :
    QDialog(parent),
    ui(new Ui::StudentDialog)
{
    ui->setupUi(this);
    this->setParent(parent);
    this->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);

    ui->teachers->setColumnCount(4);
    setHeaders(headers);
    ui->teachers->setColumnWidth(2,20);
    ui->teachers->setColumnWidth(3,20);

    this->pageOwner = stud;
    this->course = this->pageOwner.getCourse();

    ui->initials->setText(pageOwner.getFname() + "  " +
                          pageOwner.getLname() + "  " +
                          pageOwner.getFthName());
    ui->course->setText("Course: " + QString::number(course));
    ui->group->setText("Group: " + pageOwner.getGroup());
    ui->toolBox->removeItem(0);

    updateOwnersMap(course);
//    pageOwner.setStudyMap(stud.getStudyMap());
    const QMap<Discipline, Teacher> studyMap = pageOwner.getStudyMap();

    for(auto i  = pageOwner.getDisciplines().begin(); i != pageOwner.getDisciplines().end(); ++i){
        QWidget* disciplWidget = new QWidget(ui->courseMap);

        disciplWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(disciplWidget,&QWidget::customContextMenuRequested,this,[=](){on_discipl_contxtMenuRequested(*i);});

        this->studCoursesWidgets.insert(*i,disciplWidget);

        if(studyMap[*i].getFname() == "None"){
            QPushButton* noTeacherBut = new QPushButton(disciplWidget);

            noTeacherBut->setText("Add Teacher");
            noTeacherBut->setFixedSize(100,30);
            noTeacherBut->setFont(QFont(this->font()));

            noTeacherButtons.insert(disciplWidget,noTeacherBut);

            connect(noTeacherBut,&QPushButton::clicked,this,[=](){showTeachersList(*i);});

            noTeacherBut->show();
        }else{
            showTeacherUnderDiscipline(studyMap[*i],disciplWidget);
        }
        ui->toolBox->addItem(disciplWidget,i->getName());
    }


}

StudentDialog::~StudentDialog()
{
    allStudents.replace(allStudents.indexOf(pageOwner),pageOwner);
    delete ui;
}

void StudentDialog::updateOwnersMap(int course){
    for(const auto&i: allDisciplines[course-1]){
        pageOwner.addDiscipline(i);
    }
}
void StudentDialog::setHeaders(QList<QString> name){
    for(int i = 0; i < name.size();++i){
        QTableWidgetItem* header = new QTableWidgetItem;
        header->setText(name[i]);
        ui->teachers->setHorizontalHeaderItem(i,header);
    }
}
void StudentDialog::on_discipl_contxtMenuRequested(const Discipline &discipl)
{
    QMenu* menu = new QMenu;
    menu->addAction(tr("Show the most popular teacher"), this, [=](){showThePopulestTeacher(discipl);});
    menu->addAction(tr("Change Teacher"), this, [=](){showTeachersToChange(discipl);});
    menu->exec(cursor().pos());
}

void StudentDialog::addTeachersToTable(const QVector<Teacher> &teachers)
{
    ui->teachers->clear();
    ui->teachers->setRowCount(0);
    ui->teachers->setColumnCount(4);
    setHeaders(headers);
    if(!teachers.size()){
        QTableWidgetItem* itm = new QTableWidgetItem;
        itm->setText("No teachers");
        ui->teachers->setRowCount(ui->teachers->rowCount() + 1);
        ui->teachers->setItem(ui->teachers->rowCount()-1,0,itm);
        return;
    }
    for(auto &i : teachers){
        QTableWidgetItem* inits = new QTableWidgetItem;
        QTableWidgetItem* post = new QTableWidgetItem;
        QTableWidgetItem* stage = new QTableWidgetItem;
        QTableWidgetItem* populatity = new QTableWidgetItem;

        inits->setText(i.getLname() + " " + i.getFname());
        post->setText(i.getPost());
        stage->setText(QString::number(i.getStage()));
        populatity->setText(QString::number(i.getPopularity()));

        ui->teachers->setRowCount(ui->teachers->rowCount() + 1);
        ui->teachers->setItem(ui->teachers->rowCount()-1,0,inits);
        ui->teachers->setItem(ui->teachers->rowCount()-1,1,post);
        ui->teachers->setItem(ui->teachers->rowCount()-1,2,stage);
        ui->teachers->setItem(ui->teachers->rowCount()-1,3,populatity);
    }
}

void StudentDialog::minimiseTeachesVect(QVector<Teacher> &teachers)
{
    std::sort(teachers.begin(),teachers.end(),[](Teacher& first, Teacher& sec){return first < sec;});

    for(int i = 0; i < teachers.size();++i){
        while(teachers[i] == teachers[i+1]){
            teachers.remove(i+1);
            if(i >= teachers.size()-1) break;
        }
    }
}




void StudentDialog::showTeachersList(const Discipline& discipl)
{
    QVector<Teacher> teachersToShow;
    for(auto i = allTeachers.begin(); i != allTeachers.end();++i){
        if(i->hasDiscipline(discipl)){
            teachersToShow.push_back(*i);
            QWidget * w = studCoursesWidgets.value(discipl);
            connect(ui->teachers,&QTableWidget::itemDoubleClicked,this,[=](){addTeacherToTarget(*i, w);});
        }
    }
    addTeachersToTable(teachersToShow);
}

void StudentDialog::showTeachersToChange(const Discipline &discipl)
{
    QVector<Teacher> teachersToShow;
    for(auto i = allTeachers.begin(); i != allTeachers.end();++i){
        QList<QString> excistTeacherInits = studCoursesWidgets.value(discipl)->findChild<QLabel*>()->text().split('\n').first().split(' ');
        excistTeacherInits.removeFirst();

        QList<QString> teacherToShowInits({i->getLname(),i->getFname(),i->getFthName()});

        if(i->hasDiscipline(discipl) && teacherToShowInits != excistTeacherInits){
            teachersToShow.push_back(*i);
            QWidget * w = studCoursesWidgets.value(discipl);
            connect(ui->teachers,&QTableWidget::itemDoubleClicked,this,[=](){addTeacherToTarget(*i, w);});
        }
    }
    addTeachersToTable(teachersToShow);
}

void StudentDialog::showThePopulestTeacher(const Discipline &discipl)
{
//----finding the most popuar teacher---------
    auto theMostPopular = Teacher("None","None","None","None");
    bool j = true;
    for(auto i = allTeachers.begin(); i != allTeachers.end(); ++i){
        if(i->hasDiscipline(discipl)){
            if(j){
                theMostPopular = *i;
                j = false;
                continue;
            }
            if(i->getPopularity() > theMostPopular.getPopularity()){
                theMostPopular = *i;
            }
        }
    }
//-----------Printing teacher------------------
    if(theMostPopular.getPasword() == "None"){
        QTableWidgetItem*itm = new QTableWidgetItem;
        itm->setText("There is no teacher with that discipline yet");
        ui->teachers->clearContents();
        ui->teachers->setItem(0,0,itm);
        return;
    }
    QVector<Teacher> teacherToPrint({theMostPopular});
    addTeachersToTable(teacherToPrint);

    QWidget * w = studCoursesWidgets.value(discipl);
    connect(ui->teachers,&QTableWidget::itemDoubleClicked,this,[&](){addTeacherToTarget(theMostPopular, w);});

}


void StudentDialog::showTeacherUnderDiscipline(const Teacher &teachItm, QWidget *where)
{
    if(noTeacherButtons.contains(where)){
        noTeacherButtons[where]->hide();
        delete noTeacherButtons[where];
        noTeacherButtons.remove(where);
    }
    for (auto widg: where->findChildren<QLabel*>())
      delete widg;

    QLabel* teacherInfo = new QLabel(where);

    teacherInfo->setText("Teacher: " + teachItm.getLname() + " " +
                         teachItm.getFname() + " " +
                         teachItm.getFthName() +
                         "\nPost: " + teachItm.getPost() +
                         "\nPopularity = " + QString::number(teachItm.getPopularity()));
    teacherInfo->setFont(this->font());
    teacherInfo->show();
}

void StudentDialog::addTeacherToTarget(Teacher& teachItm, QWidget* where)
{
    teachItm.addCourseTarget(studCoursesWidgets.key(where),pageOwner);
    showTeacherUnderDiscipline(teachItm,where);

    pageOwner.addStudyTarget(studCoursesWidgets.key(where),teachItm);

    allTeachers.replace(allTeachers.indexOf(teachItm),teachItm);
}

void StudentDialog::on_logout_clicked()
{
    ui->teachers->clear();
    ui->course->clear();
    ui->group->clear();
    ui->initials->clear();
    for(int i = 0; i < pageOwner.getStudyMap().size(); ++i){
        ui->toolBox->removeItem(i);
    }
    allStudents.replace(allStudents.indexOf(pageOwner),pageOwner);
    emit this->finished(1);
    this->close();
}

bool compareTeachers(const Teacher& first, const Teacher& second){
    return first > second;
}

void StudentDialog::on_sortB_clicked()
{
    QVector<Teacher> teachersToSort;
    const QString teacherGroupToSort = ui->showTeachMode->currentText();
    QTableWidgetItem* itm = new QTableWidgetItem;
    itm->setText("No teacher to sort");
    if(teacherGroupToSort == "My Teachers")
    {
//------Parsing correct teachers from stydy map to QVector------
        auto begin  = pageOwner.getStudyMap().begin();
        auto end = pageOwner.getStudyMap().end();
        if((begin + 1) == end){
            QMessageBox::information(this,"Oops...","There are no teachers to sort");
            return;
        }
        for(auto i = begin; i != end; ++i)
        {
            teachersToSort.append(i.value());
        }
        minimiseTeachesVect(teachersToSort);
//------------------Sorting--------------------------
        if(ui->sortByName->isChecked())
        {
            std::sort(teachersToSort.begin(),teachersToSort.end(),[](const Teacher& first, const Teacher& second)
                                                            {
                                                                return first < second;
                                                            });
        }
        else if(ui->sortByPost->isChecked())
        {
            std::sort(teachersToSort.begin(),teachersToSort.end(),[](const Teacher& first, const Teacher& second)
                                                            {
                                                                return first > second;
                                                            });
        }
        else
        {
            //print warning
            return;
        }
//-----------------------------------------------
        qWarning("My teachers");
        addTeachersToTable(teachersToSort);

    }else if(teacherGroupToSort == "All Teachers"){
        teachersToSort = allTeachers;
//------------------Sorting--------------------------
        if(ui->sortByName->isChecked())
        {
            std::sort(teachersToSort.begin(),teachersToSort.end(),[](const Teacher& first, const Teacher& second)
                                                            {
                                                                return first < second;
                                                            });
        }
        else if(ui->sortByPost->isChecked())
        {
            std::sort(teachersToSort.begin(),teachersToSort.end(),[](const Teacher& first, const Teacher& second)
                                                            {
                                                                return first > second;
                                                            });
        }
        else
        {
            //print warning
            return;
        }
//-----------------------------------------------------
        qWarning("All teachers");
        addTeachersToTable(teachersToSort);
    }
    else if(teacherGroupToSort == "Teachers with 1 course")
    {   
//------Parsing teachers from ListWidget to QVector--------
        for(int i = 0; i < ui->teachers->rowCount();++i){
            QList<QString> initsToShow = ui->teachers->item(i,0)->text().split(' ');
            for(auto &j:allTeachers){
                QList<QString> inits({j.getFname(),j.getLname(),j.getFthName()});
                if(inits == initsToShow){
                    teachersToSort.append(j);
                }
            }
        }
//------------------Sorting--------------------------
        if(ui->sortByName->isChecked())
        {
            std::sort(teachersToSort.begin(),teachersToSort.end(),[](const Teacher& first, const Teacher& second)
                                                            {
                                                                return first < second;
                                                            });
        }
        else if(ui->sortByPost->isChecked())
        {
            std::sort(teachersToSort.begin(),teachersToSort.end(),[](const Teacher& first, const Teacher& second)
                                                            {
                                                                return  first > second;
                                                            });
        }
        else
        {
            //print warning
            return;
        }
//----------------------------------------------------
        qWarning("Teachers with 1 course");
        addTeachersToTable(teachersToSort);
    }
}

void StudentDialog::on_showTeachMode_activated(const QString &arg)
{
    QVector<Teacher> teachersToPrint;
    if(arg == "All Teachers")
    {
        addTeachersToTable(allTeachers);
        return;
    }
    else if(arg == "My Teachers")
    {
        for(auto i = pageOwner.getStudyMap().begin(); i != pageOwner.getStudyMap().end(); ++i)
        {
            teachersToPrint.append(i.value());
        }
        minimiseTeachesVect(teachersToPrint);
    }
    else if(arg == "Teachers with 1 course"){
        for(const auto &j : allTeachers){
            const QList<Discipline> discipls = j.getDisciplines();
            if(discipls.size() == 1){
                teachersToPrint.append(j);
            }
        }
    }
    addTeachersToTable(teachersToPrint);
}

