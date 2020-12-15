#include "learnsystem.h"
#include "data/flparser.h"
#include "data/exept.h"
#include <QApplication>
#include <QMessageBox>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/img/graduation-hat.png"));


    auto allStudyProcessData = StudyProcessData::getInstance();

    auto& allTeachers = allStudyProcessData->getAllTeachers();
    auto& allStudents = allStudyProcessData->getAllStudents();
    auto& allDisciplines = allStudyProcessData->getAllDisciplines();
    auto& groups = allStudyProcessData->getGroups();

    QMap<short, QString>postNames({std::pair<short,QString>(0,"Асистент"),
                                      std::pair<short,QString>(1,"Викладач"),
                                      std::pair<short,QString>(2,"Старший викладач"),
                                      std::pair<short,QString>(3,"Доцент"),
                                      std::pair<short,QString>(4,"Професор")});
    auto dir = QDir::currentPath();

    allStudyProcessData->setPostNames(postNames);
    FlParser dataReader;
    try {

        dataReader.changeFilename(dir + GROUPS_FILE);
        dataReader.readGroups(groups);

        dataReader.changeFilename(dir + "/datafiles/dcourse1.json");
        dataReader.readDisciplines(allDisciplines[0]);

        dataReader.changeFilename(dir + "/datafiles/dcourse2.json");
        dataReader.readDisciplines(allDisciplines[1]);

        dataReader.changeFilename(dir + "/datafiles/dcourse3.json");
        dataReader.readDisciplines(allDisciplines[2]);

        dataReader.changeFilename(dir + "/datafiles/dcourse4.json");
        dataReader.readDisciplines(allDisciplines[3]);

        dataReader.changeFilename(dir + STUDENTS_FILE);
        dataReader.readStudents(allStudents, allStudyProcessData);

        dataReader.changeFilename(dir + TEACHERS_FILE);
        dataReader.readTeachers(allTeachers, allStudyProcessData);

        dataReader.changeFilename(dir + STUD_STUDY_PROCESS_FILE);
        dataReader.readStudentStudyProcessData(allStudyProcessData);

        dataReader.changeFilename(dir + TEACHER_STUDY_PROCESS_FILE);
        dataReader.readTeacherStudyProcessData(allStudyProcessData);

    }  catch (Except& msg) {
           QMessageBox::critical(nullptr,"Помилка зчитування даних",msg.what());
           return -1;
    }
    LearnSystem w(nullptr);
    w.setWindowIcon(QIcon(":/images/img/graduation-hat.png"));
    w.show();
    return a.exec();
}
