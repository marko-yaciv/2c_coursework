#include "teacherdialog.h"
#include "ui_teacherdialog.h"

TeacherDialog::TeacherDialog(QWidget *parent,const Teacher& teacher) :
    QDialog(parent),
    pageOwner(teacher),
    pageOwnerMap(pageOwner.getCourseMap()),
    m_allStudyProcessData(StudyProcessData::getInstance()),
    ui(new Ui::TeacherDialog)
{
    ui->setupUi(this);

    headers = {"Назва курсу","Кількість відвідувачів"};
    ui->courses->setColumnCount(2);
    ui->courses->setColumnWidth(0,224);
    ui->courses->setColumnWidth(1,112);
    setHeaders(headers,ui->courses);
    ui->scrollArea->hide();

    headers = {"ПІБ","Група","Курс"};
    ui->students->setColumnCount(3);
    setHeaders(headers,ui->students);

    ui->initials->setText(pageOwner.getLname() + " " + pageOwner.getFname() + " " + pageOwner.getFthName());

    auto myStudents = pageOwnerMap.values();
    minimiseStudentsList(myStudents);
    ui->CntOfVisits->setText("Відвідувачі: " + QString::number(myStudents.size()));

    showOwnerDisciplines();
}

TeacherDialog::~TeacherDialog()
{
    auto& allTeachers = m_allStudyProcessData->getAllTeachers();
    allTeachers.replace(allTeachers.indexOf(pageOwner),pageOwner);
    delete ui;
}



void TeacherDialog::showStudentsList(const QList<Student> &students)
{
    ui->students->clear();
    ui->students->setRowCount(0);
    headers = {"ПІБ","Група","Курс"};
    setHeaders(headers,ui->students);

    if(!students.size())
    {
        QTableWidgetItem* itm = new QTableWidgetItem;
        itm->setText("Немає студентів");
        ui->students->setRowCount(ui->students->rowCount() + 1);
        ui->students->setItem(ui->students->rowCount()-1,0,itm);
        return;
    }
    for(auto &i : students){
        QTableWidgetItem* inits = new QTableWidgetItem;
        QTableWidgetItem* group = new QTableWidgetItem;
        QTableWidgetItem* course = new QTableWidgetItem;

        inits->setText(i.getLname() + " " + i.getFname() + " " + i.getFthName());
        group->setText(i.getGroup());
        course->setText(QString::number(i.getCourse()));

        group->setTextAlignment(Qt::AlignmentFlag::AlignHCenter);
        course->setTextAlignment(Qt::AlignmentFlag::AlignHCenter);

        ui->students->setRowCount(ui->students->rowCount() + 1);
        ui->students->setItem(ui->students->rowCount()-1,0,inits);
        ui->students->setItem(ui->students->rowCount()-1,1,group);
        ui->students->setItem(ui->students->rowCount()-1,2,course);
    }
}

void TeacherDialog::setHeaders(const QList<QString> &name, QTableWidget *table)
{
    for(int i = 0; i < name.size();++i){
        QTableWidgetItem* header = new QTableWidgetItem;
        header->setText(name[i]);
        table->setHorizontalHeaderItem(i,header);
    }
}

void TeacherDialog::on_logout_clicked()
{
    ui->initials->clear();
    ui->CntOfVisits->clear();
    ui->courses->clear();
    ui->students->clear();

    auto& allTeachers = m_allStudyProcessData->getAllTeachers();

    allTeachers.replace(allTeachers.indexOf(pageOwner),pageOwner);

    emit this->finished(1);
    this->close();

}
void TeacherDialog::minimiseStudentsList(QList<Student>& students)
{
    std::sort(students.begin(),students.end(),[](Student& first, Student& sec){return first < sec;});

    for(int i = 0; i < students.size()-1;++i)
    {
        while(students[i] == students[i+1])
        {
            students.removeAt(i+1);
            if(i >= students.size()-1) break;
        }
    }
}


//void TeacherDialog::removeTargets(const Discipline &discipline)
//{
//    auto& allTeachers = m_allStudyProcessData->getAllTeachers();
//    int pos = allTeachers.indexOf(pageOwner);
//    auto& map = allTeachers[pos].getCourseMap();

//    for(auto i = map.begin(); i != map.end();++i){

//        i->removeStudyTarget(discipline);

//        try {
//            pageOwner.removeCourseTarget(discipline, *i);
//        }  catch (const Except& ex) {
//            pageOwner.removeDiscipline(discipline);
//        }
//    }
//}

void TeacherDialog::showDisciplinesToChoose()
{
    if(ui->scrollAreaWidgetContents->children().size() > 0) return;

    QVBoxLayout* layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    layout->setMargin(0);
    layout->setContentsMargins(1,0,0,1);
    for(auto& courseDiscipls: m_allStudyProcessData->getAllDisciplines()){
        for(auto& discipl: courseDiscipls){
            if(!pageOwner.hasDiscipline(discipl))
                continue;

            QCheckBox* courseName = new QCheckBox;
            courseName->setText(discipl.getName());
            layout->addWidget(courseName);
            layout->update();


            connect(courseName,&QCheckBox::toggled,this,[&](bool checked){
               if(checked){
                    pageOwner.addDiscipline(discipl);
                    showOwnerDisciplines();
               }
               else{
                   //removeTargets(discipl);
                   //showOwnerDisciplines();
               }
            });
        }
    }
    ui->scrollAreaWidgetContents->setLayout(layout);
}

void TeacherDialog::showOwnerDisciplines()
{
    ui->courses->clear();
    ui->courses->setRowCount(0);
    auto courses = pageOwner.getDisciplines();
    ui->courses->setRowCount(courses.size());
    int j = 0;
    for(auto& discipline : courses)
    {
        if(!discipline.isEnabled())
        {
            QTableWidgetItem * courseName = new QTableWidgetItem(discipline.getName() + "(Недоступна)");
            QTableWidgetItem * cntOfVisitors = new QTableWidgetItem("000");
            ui->courses->setItem(j,0,courseName);
            ui->courses->setItem(j++,1,cntOfVisitors);
            continue;
        }
        QTableWidgetItem * courseName = new QTableWidgetItem(discipline.getName());
        QTableWidgetItem * cntOfVisitors = new QTableWidgetItem(QString::number(pageOwnerMap.values(discipline).size()));
        ui->courses->setItem(j, 0, courseName);
        ui->courses->setItem(j++, 1, cntOfVisitors);
    }
}

void TeacherDialog::commitChangesInDisciplines()
{
    ui->courseLabel->show();
    ui->scrollArea->hide();
}

void TeacherDialog::on_showStudMode_activated(const QString& showMode)
{
    QList<Student> studentsToShow;
    if(showMode == "Усі студенти")
    {
       studentsToShow = m_allStudyProcessData->getAllStudents();
    }
    else if(showMode == "Мої студенти")
    {
        studentsToShow = pageOwner.getCourseMap().values();
        minimiseStudentsList(studentsToShow);

    }
    showStudentsList(studentsToShow);
}

void TeacherDialog::on_sortB_clicked()
{
    QList<Student> studentsToSort;
    if(ui->showStudMode->currentText() == "Усі студенти")
    {
        studentsToSort = m_allStudyProcessData->getAllStudents();
    }
    else if(ui->showStudMode->currentText() == "Мої студенти")
    {
        studentsToSort = pageOwner.getCourseMap().values();
    }
    minimiseStudentsList(studentsToSort);
    std::sort(studentsToSort.begin(), studentsToSort.end(),
              [](const Student& first,const Student& second)
                {
                  return first.lessThan(second);
                });

    showStudentsList(studentsToSort);
}

void TeacherDialog::on_courses_itemDoubleClicked(QTableWidgetItem *courseName)
{
    Discipline course(courseName->text());
    auto studentsToShow = pageOwnerMap.values(course);
    showStudentsList(studentsToShow);
}

void TeacherDialog::on_pushButton_clicked()
{
    ui->courseLabel->hide();
    ui->scrollArea->show();
    showDisciplinesToChoose();
}

void TeacherDialog::on_scrollArea_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu;
    menu->addAction(tr("Зберегти зміни"), this, [&](){commitChangesInDisciplines();});
    menu->exec(cursor().pos());
}
