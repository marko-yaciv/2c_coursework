#include "learnsystem.h"
#include "data/flparser.h"
#include <QApplication>
#include <QMessageBox>
#include <data/exept.h>
extern const int COURSES = 4;
extern QList<QString> groups;

extern QVector<Student> allStudents;
extern QVector<Teacher> allTeachers;
extern QVector<QVector<Discipline>> allDisciplines;

extern StudyProcessData allStudyProcessData;

int main(int argc, char *argv[])
{
    allDisciplines.resize(COURSES);

    QApplication a(argc, argv);
    FlParser dataReader;
    try {
        dataReader.readGroups(groups);
        dataReader.changeFilename("datafiles/dcourse1.json");
        dataReader.readDisciplines(allDisciplines[0]);

        dataReader.changeFilename("datafiles/dcourse2.json");
        dataReader.readDisciplines(allDisciplines[1]);

        dataReader.changeFilename("datafiles/dcourse3.json");
        dataReader.readDisciplines(allDisciplines[2]);

        dataReader.changeFilename("datafiles/dcourse4.json");
        dataReader.readDisciplines(allDisciplines[3]);


        dataReader.readStudents(allStudents);
        dataReader.readTeachers(allTeachers);
        dataReader.readStudyProcess(allStudyProcessData);
    }  catch (Except& msg) {
           QMessageBox::critical(nullptr,"Fatal",msg.what());
           return -1;
    }
    LearnSystem w;
    w.setWindowIcon(QIcon(":/rec/img/graduation-hat.png"));
    w.show();
    return a.exec();
}
