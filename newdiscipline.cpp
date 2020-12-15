#include "newdiscipline.h"
#include "ui_newdiscipline.h"

NewDiscipline::NewDiscipline(QWidget *parent):
    QDialog(parent),
    disciplines(StudyProcessData::getInstance()->getAllDisciplines()),
    ui(new Ui::NewDiscipline)
{
     ui->setupUi(this);

    days = {"Пн","Вт","Ср","Чт","Пт"};
    showDaysToChoose();
}

NewDiscipline::~NewDiscipline()
{
    delete ui;
}

void NewDiscipline::showDaysToChoose()
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

void NewDiscipline::checkInputValidity(QString& name, int course)
{
    if(name.size() == 0 || course == 0){
        throw Except("Неправильне ім'я дисципліни або курс");
    }
}

void NewDiscipline::checkDisciplinePresence()
{
    for(auto& i : disciplines){
        if(i.contains(newDiscipline)){
            throw Except("Ця дисципліна вже існує");
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
    int course = ui->course->value();

    QDate start;
    QDate finish;
    if(ui->spinBox->value()==1)
    {
        start = QDate(QDate::currentDate().year(),9,1);
        finish = QDate(QDate::currentDate().year(),12,30);
    }
    else
    {
        start = QDate(QDate::currentDate().year(),2,1);
        finish = QDate(QDate::currentDate().year(),5,30);
    }


    try {
        checkDisciplinePresence();
        checkInputValidity(name,course);
    }  catch (Except& ex) {
        QMessageBox::warning(this,"Не вдалося створити",ex.what());
        return;
    }
    disciplineCourse = course;
    newDiscipline.rename(name);
    newDiscipline.setConductRange(start,finish);

    disciplines[course - 1].append(newDiscipline);

    emit this->finished(1);
    this->close();
}
