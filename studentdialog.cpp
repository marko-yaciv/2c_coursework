#include "studentdialog.h"
#include "ui_studentdialog.h"
#include <QMenu>
#include <QPushButton>
#include <QLayout>
#include <QMenuBar>
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
        this->studentCourses.insert(i.key(),disciplWidget);
        ui->toolBox->addItem(disciplWidget,i.key().getName());
    }


}

StudentDialog::~StudentDialog()
{
    allStudents.replace(allStudents.indexOf(pageOwner),pageOwner);
    delete ui;
}

void StudentDialog::on_discipline_customContextMenuRequested(const QPoint& pos)
{
    QMenu* menu = new QMenu;
    menu->addAction(tr("Add Teacher"),this,SLOT(showTeachersList()));

    menu->exec(cursor().pos());
}

void StudentDialog::showTeachersList(const Discipline& discipl)
{
    for(const auto& teacher : allTeachers){
        if(teacher.hasDiscipline(discipl)){
            QListWidgetItem* teachToShow = new QListWidgetItem;
            teachToShow->setText(teacher.getInitials()->at(0) + " " +
                                teacher.getInitials()->at(1) + " " +
                                teacher.getInitials()->at(2) + " Popularity.=. " +
                                QString::number(teacher.getPopularity()));
            ui->teachers->addItem(teachToShow);
            QWidget * w = studentCourses.value(discipl);
            connect(ui->teachers,&QListWidget::itemDoubleClicked,this,[=](){addTeacherToTarget(teacher, w);});
        }
    }
}
void StudentDialog::showTeacherUnderDiscipline(const Teacher &teachItm, QWidget *where)
{
    if(noTeacherButtons.contains(where)){
        noTeacherButtons[where]->hide();
    }
    for (auto wid: where->findChildren<QLabel*>())
      delete wid;
    QLabel* teacherInfo = new QLabel(where);
    auto teacherInits = teachItm.getInitials();
    teacherInfo->setText("Викладач: " + teacherInits->at(0) + " " +
                         teacherInits->at(1) + " " +
                         teacherInits->at(2));
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

    this->nativeParentWidget()->show();
    this->~StudentDialog();
}
