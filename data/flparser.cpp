#include "flparser.h"

extern QVector<Teacher> allTeachers;
extern QVector<Student> allStudents;
extern QVector<QVector<Discipline>> allDisciplines;

FlParser::FlParser(const QString& flname)
{
    this->filename = flname;
    this->file.setFileName(flname);
}

void FlParser::changeFilename(const QString& flname)
{
    this->filename = flname;
    this->file.setFileName(flname);
}

bool FlParser::openWithValidation(const QIODevice::OpenModeFlag& openMode)
{
    if(!file.open(openMode)){
        qWarning("Couldn't open save file");
        return false;
    }
    if(file.atEnd() && openMode == QIODevice::ReadOnly){
        qWarning("File is empty");
        return false;
    }
    return true;
}

void FlParser::readData(QList<QString> &data)
{
    file.open(QIODevice::ReadOnly);
    while(!file.atEnd()){
        auto lineData = file.readLine();
        data.append(lineData.remove(lineData.length()-1,1));
    }
    file.close();
}

void FlParser::writeStudents(const QVector<Student> &students)
{
    file.setFileName(STUDENTS_FILE);

    if(!openWithValidation(QIODevice::WriteOnly)){
        file.close();
        return;
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
        return;
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
        return;
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
        return;
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
        return;
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
void FlParser::readStudents(QVector<Student> &students)
{
    file.setFileName(STUDENTS_FILE);

    if(!openWithValidation(QIODevice::ReadOnly)){
        file.close();
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument readDoc(QJsonDocument::fromJson(data));
    if(readDoc.isNull() || !readDoc.isArray()){
        qWarning("Incorrect json!!");
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
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument readDoc(QJsonDocument::fromJson(data));
    if(readDoc.isNull() || !readDoc.isArray()){
        qWarning("Incorrect json!!");
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
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument readDoc(QJsonDocument::fromJson(data));
    if(readDoc.isNull() || !readDoc.isObject()){
        qWarning("Incorrect json!!");
    }
    QJsonObject procObg = readDoc.object();
    procData.readMapsOfStudents(procObg);
    file.close();

    file.setFileName(TEACHER_STUDY_PROCESS_FILE);
    if(!openWithValidation(QIODevice::ReadOnly)){
        file.close();
        return;
    }

    data = file.readAll();
    readDoc = QJsonDocument::fromJson(data);
    if(readDoc.isNull() || !readDoc.isObject()){
        qWarning("Incorrect json!!");
    }
    procObg = readDoc.object();
    procData.readMapsOfTeachers(procObg);
    file.close();
}



void FlParser::readDisciplines(QVector<Discipline>& courses)
{
    if(!openWithValidation(QIODevice::ReadOnly)){
        file.close();
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument readDoc(QJsonDocument::fromJson(data));
    if(readDoc.isArray()){
        QJsonArray discipls = readDoc.array();
        for(int i  = 0; i < discipls.size(); ++i){
            Discipline d;
            d.read(discipls[i].toObject());
            if(d.isValid()){
                courses.append(d);
            }
        }
    }

    file.close();
}

