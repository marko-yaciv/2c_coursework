#include "flparser.h"


FlParser::FlParser()
{

}

FlParser::~FlParser()
{

}

FlParser::FlParser(const QString& flname)
{
    file.setFileName(flname);
}

void FlParser::changeFilename(const QString& flname)
{
    file.setFileName(flname);
}

bool FlParser::openWithValidation(QIODevice::OpenModeFlag openMode)
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

void FlParser::writeGroups(const QList<QString>& data)
{
    if(!openWithValidation(QIODevice::WriteOnly)){
        file.close();
        throw Except("Не вдалося відкрити файл " + file.fileName() + " для запису");
    }
    file.flush();
    QJsonArray groups;
    for(auto &i : data){
        groups.append(i);
    }
    file.write(QJsonDocument(groups).toJson(QJsonDocument::Indented));

    file.close();
}

void FlParser::writeStudents(const QList<Student>& students)
{
    if(!openWithValidation(QIODevice::WriteOnly)){
        file.close();
        throw Except("Не вдалося відкрити файл " + file.fileName() + " для запису");
    }

    file.flush();
    QJsonArray obj;
    for(auto &i : students){
        if(i.getPasword() == "None") continue;
        QJsonObject stud;
        i.write(stud);
        obj.append(stud);
    }

    file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));

    file.close();
}

void FlParser::writeTeachers(const QList<Teacher>& teachers)
{
    if(!openWithValidation(QIODevice::WriteOnly)){
        file.close();
        throw Except("Не вдалося відкрити файл " + file.fileName() + " для запису");
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

void FlParser::writeStudentStudyProcessData(const StudyProcessData* studentData)
{
    if(!openWithValidation(QIODevice::WriteOnly)){
        file.close();
        throw Except("Не вдалося відкрити файл " + file.fileName() + " для запису");
    }

    file.flush();
    QJsonObject studProcData;
    studentData->writeMapsForStudents(studProcData);
    file.write(QJsonDocument(studProcData).toJson(QJsonDocument::Indented));

    file.close();
}

void FlParser::writeTeacherStudyProcessData(const StudyProcessData* teacherData)
{
    if(!openWithValidation(QIODevice::WriteOnly)){
        file.close();
        throw Except("Не вдалося відкрити файл " + file.fileName() + " для запису");
    }

    file.flush();
    QJsonObject teachProcData;
    teacherData->writeMapsForTeachers(teachProcData);
    file.write(QJsonDocument(teachProcData).toJson(QJsonDocument::Indented));

    file.close();
}


void FlParser::writeDisciplines(const QVector<Discipline>& discipls)
{
    if(!openWithValidation(QIODevice::WriteOnly)){
        file.close();
        throw Except("Не вдалося відкрити файл " + file.fileName() + " для запису");
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
    if(!openWithValidation(QIODevice::ReadOnly))
    {
        file.close();
        throw Except("Не вдалося відкрити файл " + file.fileName() + " для зчитування");
    }

    QByteArray grps = file.readAll();
    QJsonDocument readDoc(QJsonDocument::fromJson(grps));
    if(readDoc.isNull() || !readDoc.isArray())
    {
        throw Except("Пошкоджений файл" + file.fileName());
    }

    QJsonArray groups = readDoc.array();
    for(const auto &i : groups){
        data.append(i.toString());
    }

    file.close();
}

void FlParser::readStudents(QList<Student>& students, StudyProcessData* allData)
{
    if(!openWithValidation(QIODevice::ReadOnly)){
        file.close();
        throw Except("Не вдалося відкрити файл " + file.fileName() + " для зчитування");
    }

    QByteArray data = file.readAll();
    QJsonDocument readDoc(QJsonDocument::fromJson(data));
    if(readDoc.isNull() || !readDoc.isArray()){
        throw Except("Пошкоджений файл" + file.fileName());
    }

    QJsonArray studs = readDoc.array();
    for(int i  = 0; i < studs.size(); ++i){
        Student s;
        s.read(studs[i].toObject());
        for(auto &j : allData->getAllDisciplines()[s.getCourse()-1]){
            if(j.isEnabled()){
                s.addDiscipline(j);
            }
        }
        students.append(s);
    }

    file.close();
}

void FlParser::readTeachers(QList<Teacher>& teachers, StudyProcessData* allData)
{
    if(!openWithValidation(QIODevice::ReadOnly)){
        file.close();
        throw Except("Не вдалося відкрити файл " + file.fileName() + " для зчитування");
    }

    QByteArray data = file.readAll();
    QJsonDocument readDoc(QJsonDocument::fromJson(data));
    if(readDoc.isNull() || !readDoc.isArray()){
        throw Except("Пошкоджений файл" + file.fileName());
    }
    QJsonArray tchrs = readDoc.array();
    for(int i  = 0; i < tchrs.size(); ++i){
        Teacher t;
        t.read(tchrs[i].toObject(),allData->getAllDisciplines());
        teachers.append(t);
    }

    file.close();
}

void FlParser::readStudentStudyProcessData(StudyProcessData* studentData)
{

    if(!openWithValidation(QIODevice::ReadOnly)){
        file.close();
        throw Except("Не вдалося відкрити файл " + file.fileName() + " для зчитування");
    }

    QByteArray data = file.readAll();
    QJsonDocument readDoc(QJsonDocument::fromJson(data));
    if(readDoc.isNull() || !readDoc.isObject()){
        throw Except("Пошкоджений файл" + file.fileName());
    }
    QJsonObject procObg = readDoc.object();
    studentData->readMapsOfStudents(procObg);

    file.close();
}

void FlParser::readTeacherStudyProcessData(StudyProcessData* teacherData)
{
    if(!openWithValidation(QIODevice::ReadOnly)){
        file.close();
        throw Except("Не вдалося відкрити файл " + file.fileName() + " для зчитування");
    }

    QByteArray data = file.readAll();
    QJsonDocument readDoc(QJsonDocument::fromJson(data));
    if(readDoc.isNull() || !readDoc.isObject()){
        throw Except("Пошкоджений файл" + file.fileName());
    }
    QJsonObject procObg = readDoc.object();
    teacherData->readMapsOfTeachers(procObg);

    file.close();
}



void FlParser::readDisciplines(QVector<Discipline>& courses)
{
    if(!openWithValidation(QIODevice::ReadOnly)){
        file.close();
        throw Except("Не вдалося відкрити файл " + file.fileName() + " для зчитування");
    }

    QByteArray data = file.readAll();
    QJsonDocument readDoc(QJsonDocument::fromJson(data));
    if(readDoc.isNull() || !readDoc.isArray()){
        throw Except("Пошкоджений файл " + file.fileName());
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

