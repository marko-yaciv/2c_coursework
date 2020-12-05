#ifndef NEWDISCIPLINE_H
#define NEWDISCIPLINE_H

#include <QDialog>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QMessageBox>
#include "data/discipline.h"
#include "data/exept.h"
namespace Ui {
class NewDiscipline;
}

/*Class for dialog for addding new discipline*/
class NewDiscipline : public QDialog
{
    Q_OBJECT
    QVector<QVector<Discipline>>& discipls;
    Discipline newDiscipline;
    QList<QString> days;
    int disciplineCourse;
public:
    explicit NewDiscipline(QWidget *parent, QVector<QVector<Discipline>>& allDiscipines);
    ~NewDiscipline();

    void fillDays();
    void checkInputValidity(QString& name, int course);
    void checkDisciplPresence();

    int getNewDisciplineCourse();
private slots:
    void on_pushButton_clicked();

private:
    Ui::NewDiscipline *ui;
};

#endif // NEWDISCIPLINE_H
