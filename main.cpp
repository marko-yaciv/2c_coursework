#include "learnsystem.h"
#include "data/flparser.h"
#include "data/exept.h"
#include <QApplication>
#include <QMessageBox>
#include <QDir>

extern const int COURSES = 4;
extern QList<QString> groups;

extern QVector<Student> allStudents;
extern QVector<Teacher> allTeachers;
extern QVector<QVector<Discipline>> allDisciplines;

extern StudyProcessData allStudyProcessData;

int main(int argc, char *argv[])
{
    allDisciplines.resize(COURSES);

    auto dir = QDir::currentPath();

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/img/graduation-hat.png"));
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
        dataReader.readStudents(allStudents);

        dataReader.changeFilename(dir + TEACHERS_FILE);
        dataReader.readTeachers(allTeachers);

        dataReader.changeFilename(dir + STUD_STUDY_PROCESS_FILE);
        dataReader.readStudentStudyProcessData(allStudyProcessData);

        dataReader.changeFilename(dir + TEACHER_STUDY_PROCESS_FILE);
        dataReader.readTeacherStudyProcessData(allStudyProcessData);

    }  catch (Except& msg) {
           QMessageBox::critical(nullptr,"Fatal",msg.what());
           return -1;
    }
    LearnSystem w;
    w.setWindowIcon(QIcon(":/images/img/graduation-hat.png"));
    w.show();
    return a.exec();
}
