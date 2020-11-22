#include "learnsystem.h"
#include "data/flparser.h"
#include <QApplication>

extern const int COURSES = 4;
extern QList<QString> groups;

extern QVector<Student> allStudents;
extern QVector<Teacher> allTeachers;
extern QVector<QVector<Discipline>> allDisciplines;

extern StudyProcessData allStudyProcessData;

int main(int argc, char *argv[])
{
    allDisciplines.resize(COURSES);

    FlParser dataReader(GROUPS_FILE);
    dataReader.readData(groups);

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

    QApplication a(argc, argv);
    LearnSystem w;
    w.setWindowIcon(QIcon(":/rec/img/graduation-hat.png"));
    w.show();
    return a.exec();
    return 0;
}
