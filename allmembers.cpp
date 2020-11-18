#include "allmembers.h"
#include "ui_allmembers.h"
#include <QLayout>

AllMembers::AllMembers(QWidget *parent, const QVector<Teacher>& membersToShow) :
    QWidget(parent),
    ui(new Ui::allMembers)
{
    ui->setupUi(this);
    ui->members->setColumnCount(3);
    QTableWidgetItem*fname = new QTableWidgetItem("First Name");
    QTableWidgetItem*lname = new QTableWidgetItem("Last Name");
    QTableWidgetItem* id = new QTableWidgetItem("ID");

    ui->members->setHorizontalHeaderItem(0,fname);
    ui->members->setHorizontalHeaderItem(1,lname);
    ui->members->setHorizontalHeaderItem(2,id);

    for(const auto&i:membersToShow){
        QTableWidgetItem*fname = new QTableWidgetItem(i.getFname());
        QTableWidgetItem*lname = new QTableWidgetItem(i.getLname());
        QTableWidgetItem* id = new QTableWidgetItem(QString::number(i.getId()));

        ui->members->setRowCount(ui->members->rowCount()+1);

        ui->members->setItem(ui->members->rowCount()-1,0,fname);
        ui->members->setItem(ui->members->rowCount()-1,1,lname);
        ui->members->setItem(ui->members->rowCount()-1,2,id);
    }
}

AllMembers::AllMembers(QWidget *parent, const QVector<Student>& membersToShow) :
    QWidget(parent),
    ui(new Ui::allMembers)
{
    ui->setupUi(this);

    ui->members->setColumnCount(3);
    QTableWidgetItem*fname = new QTableWidgetItem("First Name");
    QTableWidgetItem*lname = new QTableWidgetItem("Last Name");
    QTableWidgetItem* id = new QTableWidgetItem("ID");

    ui->members->setHorizontalHeaderItem(0,fname);
    ui->members->setHorizontalHeaderItem(1,lname);
    ui->members->setHorizontalHeaderItem(2,id);

    for(const auto&i:membersToShow){
        QTableWidgetItem*fname = new QTableWidgetItem(i.getFname());
        QTableWidgetItem*lname = new QTableWidgetItem(i.getLname());
        QTableWidgetItem* id = new QTableWidgetItem(QString::number(i.getId()));

        ui->members->setRowCount(ui->members->rowCount()+1);

        ui->members->setItem(ui->members->rowCount()-1,0,fname);
        ui->members->setItem(ui->members->rowCount()-1,1,lname);
        ui->members->setItem(ui->members->rowCount()-1,2,id);
    }
}

AllMembers::~AllMembers()
{
    ui->members->clear();
    ui->members->setRowCount(0);
    delete ui;
}
