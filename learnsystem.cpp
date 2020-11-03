#include "learnsystem.h"
#include "ui_learnsystem.h"

LearnSystem::LearnSystem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LearnSystem)
{
    ui->setupUi(this);


    ui->teachInfo->hide();
    ui->logIn->hide();
}
LearnSystem::~LearnSystem()
{
    delete ui;
}
void LearnSystem::on_iamstud_clicked()
{
    ui->teachInfo->hide();
    ui->studGroup->show();
}

void LearnSystem::on_iamteach_clicked()
{
    ui->studGroup->hide();
    ui->teachInfo->show();
}

void LearnSystem::on_logInB_clicked()
{
    ui->registerW->hide();
    ui->logIn->show();
}


void LearnSystem::on_signUpB_clicked()
{

}

void LearnSystem::on_enterB_clicked()
{

}
