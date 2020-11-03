#ifndef LEARNSYSTEM_H
#define LEARNSYSTEM_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class LearnSystem; }
QT_END_NAMESPACE

class LearnSystem : public QMainWindow
{
    Q_OBJECT

public:
    LearnSystem(QWidget *parent = nullptr);
    ~LearnSystem();

private slots:

    void on_signUpB_clicked();

    void on_iamstud_clicked();

    void on_iamteach_clicked();

    void on_logInB_clicked();

    void on_enterB_clicked();

private:
    Ui::LearnSystem *ui;
};
#endif // LEARNSYSTEM_H
