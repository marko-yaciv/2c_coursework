#include "flparser.h"

extern QVector<Teacher> allTeachers;
extern QVector<Student> allStudents;
extern QVector<QVector<Discipline>> allDisciplines;

FlParser::FlParser(const QString& flname)
{

    file.setFileName(flname);
}

void FlParser::changeFilename(const QString& flname)
{
    file.setFileName(flname);
}

bool FlParser::openWithValidation(const QIODevice::OpenModeFlag& openMode)
{
    if(!file.open(openMode)){
        qWarning("Couldn't open file ");
        return false;
    }
    if(file.atEnd() && openMode == QIODevice::ReadOnly){
        qWarning("File is empty");
        return true;
    }
    return true;
}

void FlParser::writeGroups(const QList<QString> &data)
{
    file.setFileName(GROUPS_FILE);

    if(!openWithValidation(QIODevice::WriteOnly)){
        file.close();
        throw Except("Can't open file " + file.fileName() + " for writing");
    }
    file.flush();
    QJsonArray groups;
    for(auto &i : data){
        groups.append(i);
    }
    file.write(QJsonDocument(groups).toJson(QJsonDocument::Indented));
    file.close();
}

void FlParser::writeStudents(const QVector<Student> &students)
{
    file.setFileName(STUDENTS_FILE);

    if(!openWithValidation(QIODevice::WriteOnly)){
        file.close();
        throw Except("Can't open file " + file.fileName() + " for writing");
    }

    file.flush();
    QJsonArray obj;
    for(auto&i:students){
        if(i.getPasword() == "None") continue;
        QJsonObject stud;
        i.write(stud);
        obj.append(stud);
    }

    file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    file.close();
}

void FlParser::writeTeachers(const QVector<Teacher>& teachers)
{
    file.setFileName(TEACHERS_FILE);

    if(!openWithValidation(QIODevice::WriteOnly)){
        file.close();
        throw Except("Can't open file " + file.fileName() + " for writing");
    }

    file.flush();
    QJsonArray obj;
    for(auto&i:teachers){
        if(i.getPasword() == "None") continue;
        QJsonObject teach;
        i.write(teach);
        obj.append(teach);
    }

    file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    file.close();
}

void FlParser::writeStudyProcess(const StudyProcessData &processData)
{
    file.setFileName(STUD_STUDY_PROCESS_FILE);

    if(!openWithValidation(QIODevice::WriteOnly)){
        file.close();
        throw Except("Can't open file " + file.fileName() + " for writing");
    }

    file.flush();
    QJsonObject studProcData;
    processData.writeMapsForStudents(studProcData);
    file.write(QJsonDocument(studProcData).toJson(QJsonDocument::Indented));
    file.close();

//----------------------------------------------

    file.setFileName(TEACHER_STUDY_PROCESS_FILE);

    if(!openWithValidation(QIODevice::WriteOnly)){
        file.close();
        throw Except("Can't open file " + file.fileName() + " for writing");
    }

    file.flush();
    QJsonObject teachProcData;
    processData.writeMapsForTeachers(teachProcData);
    file.write(QJsonDocument(teachProcData).toJson(QJsonDocument::Indented));
    file.close();
}


void FlParser::writeDisciplines(const QVector<Discipline>& discipls)
{
    if(!openWithValidation(QIODevice::WriteOnly)){
        file.close();
        throw Except("Can't open file " + file.fileName() + " for writing");
    }

    QJsonArray arr;
    for(auto &i:discipls){
        QJsonObject obj;
        i.write(obj);
        arr.append(obj);
    }
    file.write(QJsonDocument(arr).toJson());
    file.close();
}


//--------------------------------------------------------------
//-----------------Read-functions-------------------------------
void FlParser::readGroups(QList<QString>& data)
{
    file.setFileName(GROUPS_FILE);

    if(!openWithValidation(QIODevice::ReadOnly)){
        file.close();
        throw Except("Can't open file " + file.fileName() + " for reading");
    }
    QByteArray grps = file.readAll();
    QJsonDocument readDoc(QJsonDocument::fromJson(grps));
    if(readDoc.isNull() || !readDoc.isArray()){
        throw Except("Incorrect json in" + file.fileName());
    }

    QJsonArray groups = readDoc.array();
    for(const auto &i : groups){
        data.append(i.toString());
    }
    file.close();
}

void FlParser::readStudents(QVector<Student>& students)
{
    file.setFileName(STUDENTS_FILE);

    if(!openWithValidation(QIODevice::ReadOnly)){
        file.close();
        throw Except("Can't open file " + file.fileName() + " for reading");
    }

    QByteArray data = file.readAll();
    QJsonDocument readDoc(QJsonDocument::fromJson(data));
    if(readDoc.isNull() || !readDoc.isArray()){
        throw Except("Incorrect json in" + file.fileName());
    }

    QJsonArray studs = readDoc.array();
    for(int i  = 0; i < studs.size(); ++i){
        Student s;
        s.read(studs[i].toObject());
        for(auto &j : allDisciplines[s.getCourse()-1]){
            if(j.isEnabled()){
                s.addDiscipline(j);
            }
        }
        students.append(s);
    }

    file.close();
}

void FlParser::readTeachers(QVector<Teacher> &teachers)
{
    file.setFileName(TEACHERS_FILE);

    if(!openWithValidation(QIODevice::ReadOnly)){
        file.close();
        throw Except("Can't open file " + file.fileName() + " for reading");
    }

    QByteArray data = file.readAll();
    QJsonDocument readDoc(QJsonDocument::fromJson(data));
    if(readDoc.isNull() || !readDoc.isArray()){
        throw Except("Incorrect json in" + file.fileName());
    }
    QJsonArray tchrs = readDoc.array();
    for(int i  = 0; i < tchrs.size(); ++i){
        Teacher t;
        t.read(tchrs[i].toObject());
        teachers.append(t);
    }
    file.close();
}

void FlParser::readStudyProcess(StudyProcessData &procData)
{
    file.setFileName(STUD_STUDY_PROCESS_FILE);

    if(!openWithValidation(QIODevice::ReadOnly)){
        file.close();
        throw Except("Can't open file " + file.fileName() + " for reading");
    }

    QByteArray data = file.readAll();
    QJsonDocument readDoc(QJsonDocument::fromJson(data));
    if(readDoc.isNull() || !readDoc.isObject()){
        throw Except("Incorrect json in" + file.fileName());
    }
    QJsonObject procObg = readDoc.object();
    procData.readMapsOfStudents(procObg);
    file.close();
//----------------------------------------
    file.setFileName(TEACHER_STUDY_PROCESS_FILE);
    if(!openWithValidation(QIODevice::ReadOnly)){
        file.close();
        throw Except("Can't open file " + file.fileName() + " for reading");
    }

    data = file.readAll();
    readDoc = QJsonDocument::fromJson(data);
    if(readDoc.isNull() || !readDoc.isObject()){
        throw Except("Incorrect json in" + file.fileName());
    }
    procObg = readDoc.object();
    procData.readMapsOfTeachers(procObg);
    file.close();
}



void FlParser::readDisciplines(QVector<Discipline>& courses)
{
    if(!openWithValidation(QIODevice::ReadOnly)){
        file.close();
        throw Except("Can't open file " + file.fileName() + " for reading");
    }

    QByteArray data = file.readAll();
    QJsonDocument readDoc(QJsonDocument::fromJson(data));
    if(readDoc.isNull() || !readDoc.isArray()){
        throw Except("Incorrect json in" + file.fileName());
    }
        QJsonArray discipls = readDoc.array();
        for(int i  = 0; i < discipls.size(); ++i){
            Discipline d;
            d.read(discipls[i].toObject());
            if(d.isValid()){
                courses.append(d);
            }
        }

    file.close();
}

