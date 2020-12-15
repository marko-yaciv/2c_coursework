#include "studentdialog.h"
#include "ui_studentdialog.h"

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

    headers = {"ПІБ","Посада","Стаж","Популярність"};
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
    auto& allStudents = m_allStudyProcessData->getAllStudents();
    allStudents.replace(allStudents.indexOf(m_pageOwner),m_pageOwner);
    delete ui;
}


/*functions for setting enviroment for window*/
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

void StudentDialog::showOwnerInfo()
{
    this->m_course = this->m_pageOwner.getCourse();

    ui->initials->setText(m_pageOwner.getFname() + "  " +
                          m_pageOwner.getLname() + "  " +
                          m_pageOwner.getFthName());
    ui->course->setText("Курс: " + QString::number(m_course));
    ui->group->setText("Група: " + m_pageOwner.getGroup());
    ui->date->setText("Сьогодні: " + QDate::currentDate().toString("dddd MM.yyyy"));
}


void StudentDialog::updateOwnersMap(int course){
    for(const auto& i : m_allStudyProcessData->getAllDisciplines()[course-1]){
        m_pageOwner.addDiscipline(i);
    }
}

void StudentDialog::showNoTeacherButton(QPushButton* noTeacherBut)
{
    noTeacherBut->setGeometry(5,5,180,40);
    noTeacherBut->setStyleSheet("color: #fff;\
                                font: 11pt \"Open Sans\";\
                                background: rgba(47, 55, 99, 0.8);\
                                border: 1px solid rgba(72, 16, 16, 0.86);\
                                border-radius: 5px;");

    noTeacherBut->setIcon(QIcon(":/images/img/add-friend.png"));
    noTeacherBut->setIconSize(QSize(25,25));
    noTeacherBut->setText("Додати викладача");

    noTeacherBut->show();
}

void StudentDialog::setHeaders(const QList<QString>& name, QTableWidget* table){
    for(int i = 0; i < name.size();++i)
    {
        QTableWidgetItem* header = new QTableWidgetItem;
        header->setText(name[i]);
        table->setHorizontalHeaderItem(i,header);
    }
}


/* functions to determine which teachers to show in table*/
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
        QMessageBox::information(this,"Не вдалося змінити викладача", tr("Спочатку варто додати викладача\n") +
                                                      tr(" щоб мати змогу змінити його"));
        return;
    }

}

void StudentDialog::showTheMostPopularTeacher(const Discipline &discipl)
{
    Teacher theMostPopular = Teacher("None","None","None","None");
    findTheMostPopularTeacher(theMostPopular,discipl);

    if(theMostPopular.getPasword() == "None"){
        QTableWidgetItem*itm = new QTableWidgetItem;
        itm->setText("Ще немає викладача з такою дисципліною");
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


/*functions that add teacher to target, show it and, if needed, remove old one*/

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
    teacherInfo->setText("ПІБ: " + teachItm.getLname() + " " +
                         teachItm.getFname() + " " +
                         teachItm.getFthName() +
                         "\nПосада: " + teachItm.getPost() +
                         "\nПопулярність = " + QString::number(teachItm.getPopularity()));
    teacherInfo->setFont(this->font());
    teacherInfo->show();
}

QList<Teacher>::iterator StudentDialog::findTeacherForTarget(const QTableWidgetItem * itm)
{
    auto& allTeachers = m_allStudyProcessData->getAllTeachers();

    auto inits = itm->text().simplified().split(' ');
    Teacher teacherToFind(inits[1], inits[0], inits[2],"Null");
    auto teacherToChange = std::find_if(allTeachers.begin(),allTeachers.end(),[teacherToFind](const Teacher& teach){
                                                                              return teach.getFname() == teacherToFind.getFname() &&
                                                                                     teach.getLname() == teacherToFind.getLname() &&
                                                                                   teach.getFthName() == teacherToFind.getFthName();
                                                                              });
    if(teacherToChange == allTeachers.end()){
        throw Except("Не вдалося знайти викладача.\n Можливо ви не натиснули на його ініціали))");
    }
    return teacherToChange;
}


void StudentDialog::addTeacherToTarget(const QTableWidgetItem* itm, const Discipline discipl, QWidget* where)
{
    disconnect(add_target_connection);
    disconnect(change_target_connection);

    try {
        auto teacherToChange = findTeacherForTarget(itm);
        teacherToChange->addCourseTarget(discipl,m_pageOwner);
        showTeacherUnderDiscipline(*teacherToChange,where);
        m_pageOwner.addStudyTarget(discipl,*teacherToChange);
    }  catch (Except& ex) {
        QMessageBox::information(this,"Fail",ex.what());
        return;
    }
}


/*functions to print teachers in table*/
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



void StudentDialog::minimiseTeachersList(QList<Teacher> &teachers)
{
    std::sort(teachers.begin(),teachers.end(),[](Teacher& first, Teacher& sec){return first < sec;});

    for(int i = 0; i < teachers.size()-1;++i){
        while(teachers[i] == teachers[i+1]){
            teachers.removeAt(i+1);
            if(i >= teachers.size()-1) break;
        }
    }
}

void StudentDialog::sortTeachers(QList<Teacher>& teachersToSort)
{
    if(ui->sortByName->isChecked())
    {
        std::sort(teachersToSort.begin(),teachersToSort.end(),[&](const Teacher& first, const Teacher& second)
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
        throw Except("Будь ласка, виберіть спосіб сортування");
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
            teachRange = course.getConductRange();

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


void StudentDialog::findPageOwnerTeachers(QList<Teacher> &teachersToFind)
{
    auto begin  = m_pageOwner.getStudyMap().begin();
    auto end = m_pageOwner.getStudyMap().end();
    if((begin + 1) == end){
        throw Except("Немає викладачів");
    }
    for(auto i = begin; i != end; ++i)
        teachersToFind.append(i.value());
}

void StudentDialog::findTeachersWithOneCourse(QList<Teacher>& teachersToFind, const QList<Teacher>& allTeachers)
{
    for(const auto &teacher : allTeachers)
        if(teacher.getDisciplines().size() == 1)
            teachersToFind.append(teacher);
}


/*custom slot*/

void StudentDialog::on_discipl_contxtMenuRequested(const Discipline &discipl)
{
    QMenu* menu = new QMenu;
    menu->addAction(tr("Показати найпопулярнішого викладача"), this, [=](){showTheMostPopularTeacher(discipl);});
    menu->addAction(tr("Змінити викладача"), this, [=](){showTeachersToChange(discipl);});
    menu->exec(cursor().pos());
}



/*slots*/

void StudentDialog::on_sortB_clicked()
{
    QList<Teacher> teachersToSort;
    auto& allTeachers = m_allStudyProcessData->getAllTeachers();
    const QString teacherGroupToSort = ui->showTeachMode->currentText();

    if(teacherGroupToSort == "Мої викладачі")
    {
        try {
            findPageOwnerTeachers(teachersToSort);
            minimiseTeachersList(teachersToSort);
            sortTeachers(teachersToSort);
        }  catch (Except& ex) {
            QMessageBox::warning(this, "Помилка сортування",ex.what());
            return;
        }
    }
    else if(teacherGroupToSort == "Усі викладачі")
    {
        teachersToSort = allTeachers;
        try {
            sortTeachers(teachersToSort);
        }  catch (Except& ex) {
            QMessageBox::warning(this, "Помилка сортування",ex.what());
            return;
        }

    }
    else if(teacherGroupToSort == "Викладачі з 1 курсом")
    {   
        try {
            findTeachersWithOneCourse(teachersToSort, allTeachers);
            sortTeachers(teachersToSort);
        }  catch (Except& ex) {
            QMessageBox::warning(this, "Помилка сортування",ex.what());
            return;
        }
    }
    addTeachersToTable(teachersToSort);
}

void StudentDialog::on_showTeachMode_activated(const QString &arg)
{
    QList<Teacher> teachersToPrint;
    auto& allTeachers = m_allStudyProcessData->getAllTeachers();
    if(arg == "Усі викладачі")
    {
        addTeachersToTable(allTeachers);
        return;
    }
    else if(arg == "Мої викладачі")
    {
        findPageOwnerTeachers(teachersToPrint);
        minimiseTeachersList(teachersToPrint);
    }
    else if(arg == "Викладачі з 1 курсом")
    {
        findTeachersWithOneCourse(teachersToPrint, allTeachers);
    }
    addTeachersToTable(teachersToPrint);
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

    this->close();
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

void StudentDialog::on_calendarWidget_customContextMenuRequested(const QPoint &)
{
    QMenu* menu = new QMenu;
    menu->addAction("Сховати календар",[&](){ui->calendarWidget->hide();});
    menu->exec(cursor().pos());
}
