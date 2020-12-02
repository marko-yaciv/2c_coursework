#include "allmembers.h"
#include "ui_allmembers.h"
#include <QLayout>

AllMembers::AllMembers(QWidget *parent, const QList<Teacher>& membersToShow) :
    QWidget(parent),
    ui(new Ui::allMembers)
{
    ui->setupUi(this);

    setHeaders();

    for(const auto&i:membersToShow)
        showMembersData(i.getLname(),i.getFname(),i.getId());
}

AllMembers::AllMembers(QWidget *parent, const QList<Student>& membersToShow) :
    QWidget(parent),
    ui(new Ui::allMembers)
{
    ui->setupUi(this);

    setHeaders();

    for(const auto&i:membersToShow)
        showMembersData(i.getLname(),i.getFname(),i.getId());
}

void AllMembers::showMembersData(QString lname, QString fname, int id)
{
    QTableWidgetItem* fnameW = new QTableWidgetItem(fname);
    QTableWidgetItem* lnameW = new QTableWidgetItem(lname);
    QTableWidgetItem* idW = new QTableWidgetItem(QString::number(id));

    ui->members->setRowCount(ui->members->rowCount()+1);

    ui->members->setItem(ui->members->rowCount()-1,0,fnameW);
    ui->members->setItem(ui->members->rowCount()-1,1,lnameW);
    ui->members->setItem(ui->members->rowCount()-1,2,idW);
}

void AllMembers::setHeaders()
{
    ui->members->setColumnCount(3);
    QTableWidgetItem*fname = new QTableWidgetItem("First Name");
    QTableWidgetItem*lname = new QTableWidgetItem("Last Name");
    QTableWidgetItem* id = new QTableWidgetItem("ID");

    ui->members->setHorizontalHeaderItem(0,fname);
    ui->members->setHorizontalHeaderItem(1,lname);
    ui->members->setHorizontalHeaderItem(2,id);
}

AllMembers::~AllMembers()
{
    ui->members->clear();
    ui->members->setRowCount(0);
    delete ui;
}
