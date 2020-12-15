#ifndef NEWDISCIPLINE_H
#define NEWDISCIPLINE_H

#include <QDialog>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QMessageBox>
#include "data/studyprocessdata.h"
#include "data/discipline.h"
#include "data/exept.h"
namespace Ui {
class NewDiscipline;
}

/*Class for dialog for addding new discipline*/
class NewDiscipline : public QDialog
{
    Q_OBJECT
    QVector<QVector<Discipline>>& disciplines;
    Discipline newDiscipline;
    QList<QString> days;
    int disciplineCourse;
public:
    explicit NewDiscipline(QWidget *parent);
    ~NewDiscipline();

    void showDaysToChoose();
    void checkInputValidity(QString& name, int course);
    void checkDisciplinePresence();

    int getNewDisciplineCourse();
private slots:
    void on_pushButton_clicked();

private:
    Ui::NewDiscipline *ui;
};

#endif // NEWDISCIPLINE_H
