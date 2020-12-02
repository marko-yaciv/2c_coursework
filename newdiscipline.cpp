#include "newdiscipline.h"
#include "ui_newdiscipline.h"

NewDiscipline::NewDiscipline(QWidget *parent, QVector<QVector<Discipline>>& allDiscipines):
    QDialog(parent),
    discipls(allDiscipines),
    ui(new Ui::NewDiscipline)
{
     ui->setupUi(this);
    ui->beginDate->setDate(QDate::currentDate());
    ui->endDate->setDate(QDate::currentDate());
    ui->beginDate->setDisplayFormat("dd.MM");
    ui->endDate->setDisplayFormat("dd.MM");

    days = {"Пн","Вт","Ср","Чт","Пт"};
    fillDays();
}

NewDiscipline::~NewDiscipline()
{
    delete ui;
}

void NewDiscipline::fillDays()
{

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setContentsMargins(1,0,0,1);
    for(auto& day : days){
        QCheckBox* courseDay = new QCheckBox;
        courseDay->setContentsMargins(1,0,1,0);
        courseDay->setText(day);
        layout->addWidget(courseDay);
        layout->update();

        connect(courseDay,&QCheckBox::toggled,this,[&](bool checked){
           if(checked)
                newDiscipline.addTeachDay(day);
           else{
               newDiscipline.removeTeachDay(day);
           }
        });
    }
    ui->scrollAreaWidgetContents->setLayout(layout);


}

void NewDiscipline::checkInputValidity(QString& name, int course, QDate start, QDate finish)
{
    if(name.size() == 0 || course == 0){
        throw Except("Incorrect name or course of discipline");
    }
    if(start.month() > finish.month()){
        throw Except(tr("The month of start teaching") +
                     tr("should be smaller that moth of finish"));
    }
}

void NewDiscipline::checkDisciplPresence()
{
    for(auto& i : discipls){
        if(i.contains(newDiscipline)){
            throw Except("That discipline already excists");
        }
    }
}

int NewDiscipline::getNewDisciplineCourse()
{
    return disciplineCourse;
}

void NewDiscipline::on_pushButton_clicked()
{
    auto name = ui->name->text();
    QDate start = ui->beginDate->date();
    QDate finish = ui->endDate->date();
    int course = ui->course->value();


    try {
        checkDisciplPresence();
        checkInputValidity(name,course, start, finish);
    }  catch (Except& ex) {
        QMessageBox::warning(this,"Creation failed",ex.what());
        return;
    }
    disciplineCourse = course;
    newDiscipline.rename(name);
    newDiscipline.setConductRange(start,finish);

    discipls[course - 1].append(newDiscipline);

    emit this->finished(1);
    this->close();
}
