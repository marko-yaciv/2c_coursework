#include "studentdialog.h"
#include "ui_studentdialog.h"
#include <QMenu>
#include <QPushButton>
#include <QLayout>
#include <QMenuBar>
#include <QtAlgorithms>
extern const int COURSES;
extern QVector<QVector<Discipline>> allDisciplines;
extern QVector<Teacher> allTeachers;
extern QVector<Student> allStudents;

void StudentDialog::updateOwnersMap(int course){
    for(auto&i: allDisciplines[course-1]){
        pageOwner.addDiscipline(i);
    }
}




StudentDialog::StudentDialog(QWidget *parent, const Student& stud) :
    QDialog(parent),
    ui(new Ui::StudentDialog)
{
    this->setParent(parent);
    this->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);

    ui->setupUi(this);
    this->pageOwner = stud;
    this->course = this->pageOwner.getCourse();
    auto inits = pageOwner.getInitials();
    for(auto i: *inits){
        ui->initials->setText(ui->initials->text() + i + "  ");
    }
    ui->course->setText("Course: " + QString::number(course));
    ui->group->setText("Group: " + pageOwner.getGroup());
    ui->toolBox->removeItem(0);

    updateOwnersMap(course);
    this->pageOwner.setStudyMap(stud.getStudyMap());
    const QMap<Discipline, Teacher> studyMap = pageOwner.getStudyMap();

    for(auto i  = studyMap.begin(); i != studyMap.end(); ++i){
        QWidget* disciplWidget = new QWidget(ui->courseMap);

        disciplWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(disciplWidget,&QWidget::customContextMenuRequested,this,[=](){on_discipl_contxtMenuRequested(i.key());});

        this->studentCourses.insert(i.key(),disciplWidget);

        if(i.value().getInitials()->at(0) == "None"){
            QPushButton* noTeacherBut = new QPushButton(disciplWidget);

            noTeacherBut->setText("Add Teacher");
            noTeacherBut->setFixedSize(100,30);
            noTeacherBut->setFont(QFont(this->font()));

            noTeacherButtons.insert(disciplWidget,noTeacherBut);

            connect(noTeacherBut,&QPushButton::clicked,this,[=](){showTeachersList(i.key());});

            noTeacherBut->show();
        }else{
            showTeacherUnderDiscipline(i.value(),disciplWidget);
        }
        ui->toolBox->addItem(disciplWidget,i.key().getName());
    }


}

StudentDialog::~StudentDialog()
{
    allStudents.replace(allStudents.indexOf(pageOwner),pageOwner);
    delete ui;
}
void StudentDialog::on_discipl_contxtMenuRequested(const Discipline &discipl)
{
    QMenu* menu = new QMenu;
    menu->addAction(tr("Show the most popular teacher"),this,[=](){showThePopulestTeacher(discipl);});

    menu->exec(cursor().pos());
}

void StudentDialog::addTeachersToList(QVector<Teacher> &teachers)
{
    ui->teachers->clear();
    for(auto &i : teachers){
        const QList<Discipline>* disciplines = i.getDissciplines();
        QListWidgetItem* item = new QListWidgetItem;
        item->setText(i.getInitials()->at(0) + " " +
                      i.getInitials()->at(1) + " " +
                      i.getInitials()->at(2) +
                      "\nPost: " +
                      i.getPost() +
                      "\nPopularity = " +
                      QString::number(i.getPopularity()) + "\nDisciplines:\n");
        for(auto &i : *disciplines){
            item->setText(item->text() + i.getName() + ";\n");
        }
        ui->teachers->addItem(item);
    }
}
void StudentDialog::showTeachersList(const Discipline& discipl)
{
    QVector<Teacher> teachersToShow;
    for(const auto& teacher : allTeachers){
        if(teacher.hasDiscipline(discipl)){
            teachersToShow.push_back(teacher);
            QWidget * w = studentCourses.value(discipl);
            connect(ui->teachers,&QListWidget::itemDoubleClicked,this,[=](){addTeacherToTarget(teacher, w);});
        }
    }
    addTeachersToList(teachersToShow);
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
        QListWidgetItem*itm = new QListWidgetItem;
        itm->setText("There is no teacher with that discipline yet");
        ui->teachers->clear();
        ui->teachers->addItem(itm);
        return;
    }
    QVector<Teacher> teacherToPrint({theMostPopular});
    addTeachersToList(teacherToPrint);

    QWidget * w = studentCourses.value(discipl);
    connect(ui->teachers,&QListWidget::itemDoubleClicked,this,[=](){addTeacherToTarget(theMostPopular, w);});

}


void StudentDialog::showTeacherUnderDiscipline(const Teacher &teachItm, QWidget *where)
{
    if(noTeacherButtons.contains(where)){
        noTeacherButtons[where]->close();
        noTeacherButtons.remove(where);
    }
    for (auto wid: where->findChildren<QLabel*>())
      delete wid;

    QLabel* teacherInfo = new QLabel(where);
    auto teacherInits = teachItm.getInitials();
    teacherInfo->setText("Teacher: " + teacherInits->at(0) + " " +
                         teacherInits->at(1) + " " +
                         teacherInits->at(2) +
                         "\nPost:" + teachItm.getPost() +
                         "\nPopularity = " + QString::number(teachItm.getPopularity()));
    teacherInfo->setFont(this->font());
    teacherInfo->show();
}

void StudentDialog::addTeacherToTarget(const Teacher& teachItm, QWidget* where)
{
    showTeacherUnderDiscipline(teachItm,where);
    pageOwner.addStudyTarget(studentCourses.key(where),teachItm);
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

    emit this->finished(1);
    this->close();
}

bool compareTeachers(const Teacher& first, const Teacher& second){
    return first > second;
}

void StudentDialog::on_sortB_clicked()
{
    if(ui->showTeachMode->currentText() == "My Teachers")
    {
        QVector<Teacher> myTeachers;
//------Parsing correct teachers from stydy map to QVector------
        for(auto i = pageOwner.getStudyMap().begin(); i != pageOwner.getStudyMap().end(); ++i)
        {
            if(i.value().getPasword() == "None") continue;
            myTeachers.append(i.value());
        }
//------------------Sorting--------------------------
        if(ui->sortByName->isChecked())
        {
            std::sort(myTeachers.begin(),myTeachers.end(),[](const Teacher& first, const Teacher& second)
                                                            {
                                                                return first < second;
                                                            });
        }
        else if(ui->sortByPost->isChecked())
        {
            std::sort(myTeachers.begin(),myTeachers.end(),[](const Teacher& first, const Teacher& second)
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
        addTeachersToList(myTeachers);

    }else if(ui->showTeachMode->currentText() == "All Teachers"){
//------------------Sorting--------------------------
        if(ui->sortByName->isChecked())
        {
            std::sort(allTeachers.begin(),allTeachers.end(),[](const Teacher& first, const Teacher& second)
                                                            {
                                                                return first < second;
                                                            });
        }
        else if(ui->sortByPost->isChecked())
        {
            std::sort(allTeachers.begin(),allTeachers.end(),[](const Teacher& first, const Teacher& second)
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
        addTeachersToList(allTeachers);
    }
    else if(ui->showTeachMode->currentText() == "Teachers with 1 course")
    {
        ui->teachers->selectAll();
//------Parsing teachers from ListWidget to QVector--------
        QVector<Teacher> teachersToSort;

        for(int i = 0; i<ui->teachers->count();++i){
            QVector<QString>inits = ui->teachers->item(i)->text().split('\n').first().split(' ').toVector();
            for(auto &j:allTeachers){
                if(*j.getInitials() == inits){
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
        addTeachersToList(teachersToSort);
    }
}

void StudentDialog::on_showTeachMode_activated(const QString &arg)
{
    QVector<Teacher> teachersToPrint;
    if(arg == "All Teachers")
    {
        addTeachersToList(allTeachers);
        return;
    }
    else if(arg == "My Teachers")
    {
        for(auto i = pageOwner.getStudyMap().begin(); i != pageOwner.getStudyMap().end(); ++i)
        {
            if(i.value().getPasword() == "None") continue;
            teachersToPrint.append(i.value());
        }
    }
    else if(arg == "Teachers with 1 course"){
        for(auto &j : allTeachers){
            const QList<Discipline>* discipls = j.getDissciplines();
            if(discipls->size() == 1){
                teachersToPrint.append(j);
            }
        }
    }
    addTeachersToList(teachersToPrint);
}

