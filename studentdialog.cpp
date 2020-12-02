#include "studentdialog.h"
#include "ui_studentdialog.h"
#include <QMenu>
#include <QPushButton>
#include <QLayout>
#include <QMenuBar>
#include <QtAlgorithms>
#include <QMessageBox>
#include <QStyle>

QList<QString> headers({"Initials","Post","Stage","Popularity"});

StudentDialog::StudentDialog(QWidget *parent, const Student& stud) :
    QDialog(parent),
    m_pageOwner(stud),
    m_allStudyProcessData(StudyProcessData::getInstance()),
    ui(new Ui::StudentDialog)
{
    ui->setupUi(this);
    this->setParent(parent);
    this->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);

    ui->calendarWidget->hide();

    ui->teachers->setColumnCount(4);
    setHeaders(headers,ui->teachers);
    ui->teachers->setColumnWidth(2,20);
    ui->teachers->setColumnWidth(3,20);

    showOwnerInfo();
    ui->toolBox->removeItem(0);

    if(m_pageOwner.getDisciplines().size() == 0)
        updateOwnersMap(m_course);

    showCourseMap();
}

StudentDialog::~StudentDialog()
{
    delete ui;
}

void StudentDialog::updateOwnersMap(int course){
    for(const auto& i : m_allStudyProcessData->getAllDisciplines()[course-1]){
        m_pageOwner.addDiscipline(i);
    }
}

void StudentDialog::showCourseMap()
{
    const QMap<Discipline, Teacher> studyMap = m_pageOwner.getStudyMap();
    for(auto &i : m_pageOwner.getDisciplines()){
        if(!i.isEnabled()) continue;
        QWidget* disciplWidget = new QWidget(ui->courseMap);

        disciplWidget->setStyleSheet("color:#fff;\
                                        font: 11pt \"Montserrat\" ");

        disciplWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(disciplWidget,&QWidget::customContextMenuRequested,this,[&](){on_discipl_contxtMenuRequested(i);});
        m_studCoursesWidgets.insert(i,disciplWidget);

        if(!studyMap.contains(i)){
            QPushButton* noTeacherBut = new QPushButton(disciplWidget);

            connect(noTeacherBut,&QPushButton::clicked,this,[=](){showTeachersList(i);});
            m_noTeacherButtons.insert(disciplWidget,noTeacherBut);

            showNoTeacherButton(noTeacherBut);
        }else{
            showTeacherUnderDiscipline(studyMap[i],disciplWidget);
        }

        ui->toolBox->addItem(disciplWidget,i.getName());
        ui->toolBox->setStyleSheet("color: #441864;\
                                   font: 11pt \"Open Sans\";");
    }
}

void StudentDialog::showNoTeacherButton(QPushButton* noTeacherBut)
{
    noTeacherBut->setGeometry(5,5,140,40);
    noTeacherBut->setStyleSheet("color: #fff;\
                                font: 11pt \"Open Sans\";\
                                background: rgba(47, 55, 99, 0.8);\
                                border: 1px solid rgba(72, 16, 16, 0.86);\
                                border-radius: 5px;");

    noTeacherBut->setIcon(QIcon(":/images/img/add-friend.png"));
    noTeacherBut->setIconSize(QSize(25,25));
    noTeacherBut->setText("Add Teacher");

    noTeacherBut->show();
}

void StudentDialog::showOwnerInfo()
{
    this->m_course = this->m_pageOwner.getCourse();

    ui->initials->setText(m_pageOwner.getFname() + "  " +
                          m_pageOwner.getLname() + "  " +
                          m_pageOwner.getFthName());
    ui->course->setText("Course: " + QString::number(m_course));
    ui->group->setText("Group: " + m_pageOwner.getGroup());
    ui->date->setText("Today is: " + QDate::currentDate().toString("dddd MM.yyyy"));
}

void StudentDialog::setWhatIsThisForTeacher(QString &whatIsThis, const Teacher& teacher)
{
    for(auto&dis:teacher.getDisciplines())
    {
        if(!dis.isEnabled()) continue;
        whatIsThis.append(dis.getName() + " (");
        for(auto& day: dis.getCourseDays())
        {
            whatIsThis.append(day + "; ");
        }
        whatIsThis.append(") \n");
    }
}

void StudentDialog::setHeaders(const QList<QString>& name, QTableWidget* table){
    for(int i = 0; i < name.size();++i)
    {
        QTableWidgetItem* header = new QTableWidgetItem;
        header->setText(name[i]);
        table->setHorizontalHeaderItem(i,header);
    }
}

void StudentDialog::findFreeTeachers(QList<Teacher> &freeTeachers, const QDate& selectedDate)
{
    QLocale objectForDayOfWeek;
    QString currentDay = objectForDayOfWeek.dayName(selectedDate.dayOfWeek(),QLocale::ShortFormat);
    QPair<QDate,QDate> teachRange;

    for(auto& teacher : m_allStudyProcessData->getAllTeachers())
    {
        auto courses = teacher.getDisciplines();
        bool isFree = true;

        for(auto& course:courses)
        {
            teachRange = course.getTeachRange();

            for(auto& day : course.getCourseDays())
            {
                if(selectedDate.month() < teachRange.first.month() || selectedDate.month() > teachRange.second.month())
                {
                    if(courses.size() == 1){
                        isFree = false;
                        break;
                    }
                    else continue;
                }
                else if(selectedDate.day() < teachRange.first.day() || selectedDate.day() > teachRange.second.day())
                {
                    if(courses.size() == 1){
                        isFree = false;
                        break;
                    }
                    else continue;
                }
                if(currentDay == day){
                    isFree = false;
                    break;
                }
            }
            if(!isFree) break;
        }
        if(isFree) freeTeachers.append(teacher);
    }

}

void StudentDialog::on_discipl_contxtMenuRequested(const Discipline &discipl)
{
    QMenu* menu = new QMenu;
    menu->addAction(tr("Show the most popular teacher"), this, [=](){showTheMostPopularTeacher(discipl);});
    menu->addAction(tr("Change Teacher"), this, [=](){showTeachersToChange(discipl);});
    menu->exec(cursor().pos());
}

void StudentDialog::addTeachersToTable(const QList<Teacher> &teachers)
{
    ui->teachers->clear();
    ui->teachers->setRowCount(0);
    ui->teachers->setColumnCount(4);
    setHeaders(headers,ui->teachers);
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

        QString whatIsThis;
        setWhatIsThisForTeacher(whatIsThis, i);
        inits->setToolTip(whatIsThis);

        inits->setText(i.getLname() + " " + i.getFname() + " " + i.getFthName());
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

void StudentDialog::minimiseTeachesVect(QList<Teacher> &teachers)
{
    std::sort(teachers.begin(),teachers.end(),[](Teacher& first, Teacher& sec){return first < sec;});

    for(int i = 0; i < teachers.size()-1;++i){
        while(teachers[i] == teachers[i+1]){
            teachers.removeAt(i+1);
            if(i >= teachers.size()-1) break;
        }
    }
}

void StudentDialog::showTeachersList(const Discipline discipl)
{
    disconnect(add_target_connection);

    QList<Teacher> teachersToShow;
    bool isConnected = false;
    for(auto& teacher : m_allStudyProcessData->getAllTeachers()){
        if(teacher.hasDiscipline(discipl)){
            teachersToShow.push_back(teacher);
            if(!isConnected){
                add_target_connection = connect(ui->teachers,&QTableWidget::itemDoubleClicked,this,[=](QTableWidgetItem* itm){
                    addTeacherToTarget(itm,discipl, m_studCoursesWidgets.value(discipl));});
                isConnected = true;
            }
        }
    }
    addTeachersToTable(teachersToShow);
}

void StudentDialog::showTeachersToChange(const Discipline &discipl)
{
    disconnect(change_target_connection);
    auto prevTeacher = m_studCoursesWidgets.value(discipl)->findChild<QLabel*>();

    if(prevTeacher != nullptr)
    {
        QList<Teacher> teachersToShow;
        QList<QString> excistTeacherInits = prevTeacher->text().split('\n').first().split(' ');
        excistTeacherInits.removeFirst();

        for(auto& i : m_allStudyProcessData->getAllTeachers())
        {
            QList<QString> teacherToShowInits({i.getLname(),i.getFname(),i.getFthName()});

            if(i.hasDiscipline(discipl) && teacherToShowInits != excistTeacherInits)
                teachersToShow.push_back(i);
        }
        addTeachersToTable(teachersToShow);

        change_target_connection = connect(ui->teachers,&QTableWidget::itemDoubleClicked,this,[=](QTableWidgetItem* item){
            addTeacherToTarget(item, discipl, m_studCoursesWidgets.value(discipl));});
    }
    else
    {
        QMessageBox::information(this,"Canot change", tr("You should add teacher fisrt") +
                                                      tr("to be able to change him"));
        return;
    }

}

void StudentDialog::showTheMostPopularTeacher(const Discipline &discipl)
{
//----finding the most popuar teacher---------
    Teacher theMostPopular = Teacher("None","None","None","None");
    findTheMostPopularTeacher(theMostPopular,discipl);

//-----------Printing teacher------------------
    if(theMostPopular.getPasword() == "None"){
        QTableWidgetItem*itm = new QTableWidgetItem;
        itm->setText("There is no teacher with that discipline yet");
        ui->teachers->clearContents();
        ui->teachers->setItem(0,0,itm);
        return;
    }
    QList<Teacher> teacherToPrint({theMostPopular});
    addTeachersToTable(teacherToPrint);

    disconnect(change_target_connection);
    change_target_connection = connect(ui->teachers,&QTableWidget::itemDoubleClicked,this,[=](QTableWidgetItem* itm){
        addTeacherToTarget(itm,discipl, m_studCoursesWidgets.value(discipl));});

}

void StudentDialog::findTheMostPopularTeacher(Teacher &teacher, const Discipline& discipl)
{
    const auto& allTeachers = m_allStudyProcessData->getAllTeachers();

    bool j = true;
    for(auto i = allTeachers.begin(); i != allTeachers.end(); ++i){
        if(i->hasDiscipline(discipl)){
            if(j){
                teacher = *i;
                j = false;
                continue;
            }
            if(i->getPopularity() > teacher.getPopularity()){
                teacher = *i;
            }
        }
    }
}

void StudentDialog::removeOldTeacherFromWidget(QWidget *where)
{
    auto prevLabels = where->findChildren<QLabel*>();
    auto& allTeachers = m_allStudyProcessData->getAllTeachers();

    if(prevLabels.size() > 0){
        auto prevInits = prevLabels[0]->text().split('\n').first().split(' ');
        prevInits.removeFirst();
        Teacher teach(prevInits[1], prevInits[0], prevInits[2],"Null");
        auto teacherToChange = std::find_if(allTeachers.begin(),allTeachers.end(),[teach](const Teacher& teacher){
                                                           return teacher.getFname() == teach.getFname() &&
                                                                  teacher.getLname() == teach.getLname()&&
                                                                teacher.getFthName() == teach.getFthName();
                                                           });
        teacherToChange->removeCourseTarget(m_studCoursesWidgets.key(where),m_pageOwner);
    }

    for (auto& widg: prevLabels)
      delete widg;
}

void StudentDialog::showTeacherUnderDiscipline(const Teacher teachItm, QWidget *where)
{
    if(m_noTeacherButtons.contains(where)){
        m_noTeacherButtons[where]->hide();
        delete m_noTeacherButtons[where];
        m_noTeacherButtons.remove(where);
    }

    removeOldTeacherFromWidget(where);

    QLabel* teacherInfo = new QLabel(where);
    teacherInfo->setObjectName("info");
    teacherInfo->setText("Teacher: " + teachItm.getLname() + " " +
                         teachItm.getFname() + " " +
                         teachItm.getFthName() +
                         "\nPost: " + teachItm.getPost() +
                         "\nPopularity = " + QString::number(teachItm.getPopularity()));
    teacherInfo->setFont(this->font());
    teacherInfo->show();
}

void StudentDialog::addTeacherToTarget(const QTableWidgetItem* itm, const Discipline discipl, QWidget* where)
{
    //finding correct teacher
    disconnect(add_target_connection);
    disconnect(change_target_connection);

    auto& allTeachers = m_allStudyProcessData->getAllTeachers();

    auto inits = itm->text().simplified().split(' ');
    Teacher teacherToFind(inits[1], inits[0], inits[2],"Null");
    auto teacherToChange = std::find_if(allTeachers.begin(),allTeachers.end(),[teacherToFind](const Teacher& teach){
                                                                              return teach.getFname() == teacherToFind.getFname() &&
                                                                                     teach.getLname() == teacherToFind.getLname() &&
                                                                                   teach.getFthName() == teacherToFind.getFthName();
                                                                              });

    if(teacherToChange == allTeachers.end()){
        QMessageBox::information(this,"Fail","Canot find teacher.\n Maybe you clicked not on teacher's name))");
        return;
    }


    //changing data
    teacherToChange->addCourseTarget(discipl,m_pageOwner);
    showTeacherUnderDiscipline(*teacherToChange,where);
    try {
        m_pageOwner.addStudyTarget(discipl,*teacherToChange);
    }  catch (Except& ex) {
        QMessageBox::information(this,"Attach failed", ex.what());
        return;
    }

}

void StudentDialog::on_logout_clicked()
{
    ui->teachers->clear();
    ui->course->clear();
    ui->group->clear();
    ui->initials->clear();

    for(int i = 0; i < m_pageOwner.getStudyMap().size(); ++i){
        ui->toolBox->removeItem(i);
    }

    auto& allStudents = m_allStudyProcessData->getAllStudents();
    allStudents.replace(allStudents.indexOf(m_pageOwner),m_pageOwner);

    emit this->finished(1);
    this->close();
}

void StudentDialog::on_sortB_clicked()
{
    QList<Teacher> teachersToSort;

    auto& allTeachers = m_allStudyProcessData->getAllTeachers();

    const QString teacherGroupToSort = ui->showTeachMode->currentText();
    if(teacherGroupToSort == "My Teachers")
    {
//------Parsing correct teachers from stydy map to QVector------
        auto begin  = m_pageOwner.getStudyMap().begin();
        auto end = m_pageOwner.getStudyMap().end();
        if((begin + 1) == end){
            QMessageBox::information(this,"Oops...","There are no teachers to sort");
            return;
        }
        for(auto i = begin; i != end; ++i)
            teachersToSort.append(i.value());

        minimiseTeachesVect(teachersToSort);

        if(ui->sortByName->isChecked())
        {
            std::sort(teachersToSort.begin(),teachersToSort.end(),[](const Teacher& first, const Teacher& second)
                                                            {
                                                                return first < second;
                                                            });
        }
        else if(ui->sortByPost->isChecked())
        {
            std::sort(teachersToSort.begin(),teachersToSort.end(),[&](const Teacher& first, const Teacher& second)
                                                            {
                                                                return first.compareByPost(second,m_allStudyProcessData->getPostNames());
                                                            });
        }
        else
        {
            QMessageBox::warning(this, "Sort failed","Please, choose  the type of sorting");
            return;
        }

    }
    else if(teacherGroupToSort == "All Teachers"){
        teachersToSort = allTeachers;

        if(ui->sortByName->isChecked())
        {
            std::sort(teachersToSort.begin(),teachersToSort.end(),[](const Teacher& first, const Teacher& second)
                                                            {
                                                                return first < second;
                                                            });
        }
        else if(ui->sortByPost->isChecked())
        {
            std::sort(teachersToSort.begin(),teachersToSort.end(),[&](const Teacher& first, const Teacher& second)
                                                            {
                                                                return first.compareByPost(second,m_allStudyProcessData->getPostNames());
                                                            });
        }
        else
        {
            QMessageBox::warning(this, "Sort failed","Please, choose  the type of sorting");
            return;
        }

    }
    else if(teacherGroupToSort == "Teachers with 1 course")
    {   
        for(const auto &teacher : allTeachers){
            if(teacher.getDisciplines().size() == 1)
                teachersToSort.append(teacher);
        }

        if(ui->sortByName->isChecked())
        {
            std::sort(teachersToSort.begin(),teachersToSort.end(),[](const Teacher& first, const Teacher& second)
                                                            {
                                                                return first < second;
                                                            });
        }
        else if(ui->sortByPost->isChecked())
        {
            std::sort(teachersToSort.begin(),teachersToSort.end(),[&](const Teacher& first, const Teacher& second)
                                                            {
                                                                return  first.compareByPost(second,m_allStudyProcessData->getPostNames());
                                                            });
        }
        else{
            QMessageBox::warning(this, "Sort failed","Please, choose  the type of sorting");
            return;
        }
    }
    addTeachersToTable(teachersToSort);
}

void StudentDialog::on_showTeachMode_activated(const QString &arg)
{
    QList<Teacher> teachersToPrint;
    auto& allTeachers = m_allStudyProcessData->getAllTeachers();
    if(arg == "All Teachers")
    {
        addTeachersToTable(allTeachers);
        return;
    }
    else if(arg == "My Teachers")
    {
        for(auto i = m_pageOwner.getStudyMap().begin(); i != m_pageOwner.getStudyMap().end(); ++i)
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

void StudentDialog::on_showByDateB_clicked()
{
    ui->calendarWidget->show();
}

void StudentDialog::on_calendarWidget_activated(const QDate& selectedDate)
{
        QList<Teacher> freeTeachers;

        findFreeTeachers(freeTeachers, selectedDate);
        addTeachersToTable(freeTeachers);
}

void StudentDialog::on_calendarWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu* menu = new QMenu;
    menu->addAction("Hide Calendar",[&](){ui->calendarWidget->hide();});
    menu->exec(cursor().pos());
}
