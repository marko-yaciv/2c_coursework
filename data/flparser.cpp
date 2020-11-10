#include "flparser.h"

extern QVector<Teacher> allTeachers;
extern QVector<Student> allStudents;


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
    if(!file.open(QIODevice::WriteOnly)){
        qWarning("Couldn't open save file");
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
    if(!file.open(QIODevice::WriteOnly)){
        qWarning("Couldn't open save file");
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

void FlParser::writeDiscipl(const QList<QString> discipls)
{
    if(!file.open(QIODevice::WriteOnly)){
        qWarning("Couldn't open save file");
        return;
    }
    QJsonArray arr;
    for(auto &i:discipls){
        QJsonObject obj;
        Discipline discipl(i);
        discipl.write(obj);
        arr.append(obj);
    }
    file.write(QJsonDocument(arr).toJson());
    file.close();
}

void FlParser::readStudents(QVector<Student> &students)
{
    if(!file.open(QIODevice::ReadOnly)){
        qWarning("Couldn't open save file");
        return;
    }
    if(file.atEnd()){
        file.close();
        qWarning("file is empty");
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
        students.append(s);
    }

    file.close();
}

void FlParser::readTeachers(QVector<Teacher> &teachers)
{
    if(!file.open(QIODevice::ReadOnly)){
        qWarning("Couldn't open save file");
        return;
    }
    if(file.atEnd()){
        file.close();
        qWarning("file is empty");
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

void FlParser::readDisciplines(QVector<Discipline>& courses)
{
    if(!file.open(QIODevice::ReadOnly)){
        qWarning("Couldn't open save file");
        return;
    }
    if(file.atEnd()){
        file.close();
        qWarning("file is enmty");
        return;
    }
    QByteArray data = file.readAll();
    QJsonDocument readDoc(QJsonDocument::fromJson(data));
    if(readDoc.isArray()){
        QJsonArray discipls = readDoc.array();
        for(int i  = 0; i < discipls.size(); ++i){
            Discipline d;
            d.read(discipls[i].toObject());
            courses.append(d);
        }
    }
    file.close();
}


//void FlParser::writeData(const QString & data)
//{
//    file.open(QIODevice::Append);
//    file.write(data.toUtf8() + "\n");
//    file.close();
//}

//void FlParser::writeStudentInfo(const Student &stud)
//{
//    file.open(QIODevice::Append);
//    auto studInits = stud.getInitials();
//    QList<QString> infoToPrint = QList<QString>::fromVector(*studInits);
//    infoToPrint.append(stud.getGroup());
//    infoToPrint.append(QString::number(stud.getId()));
//    infoToPrint.append(stud.getPasword());
//    //int endOfInits = 6;

//    //QList<QString> mapMemberStr;

////    const QMap<Discipline,Teacher> studyMap = stud.getStudyMap();
////    const QVector<QString>* teacherInits;
////    for(auto i = studyMap.begin(); i != studyMap.end(); ++i){
////        teacherInits = i.value().getInitials();
////        mapMemberStr.append(i.key().getName() + " " +
////                            teacherInits->at(0) + "#" +
////                            teacherInits->at(1) + "#" +
////                            teacherInits->at(2) + "#" +
////                            i.value().getPasword() +  "/");

////    }
////    infoToPrint.append(mapMemberStr);

////    int j = 0;
////    for(auto&i: infoToPrint){
////        file.write((i + " ").toUtf8());
////        j++;
////        if(j == endOfInits) file.write("|");
////    }
////    file.write(QString("\n").toUtf8());
//    for(auto&i: infoToPrint){
//        file.write((i + " ").toUtf8());
//    }
//    file.write(QString("\n").toUtf8());
//    file.close();
//}

//void FlParser::writeTeacherInfo(const Teacher & teach)
//{
//    file.open(QIODevice::Append);

//    auto teachInits = teach.getInitials();
//    QList<QString> infoToPrint = QList<QString>::fromVector(*teachInits);
//    infoToPrint.append(teach.getPost());
//    infoToPrint.append(QString::number(teach.getStage()));
//    infoToPrint.append(QString::number(teach.getId()));
//    infoToPrint.append(teach.getPasword());
////    int endOfInits = 7;
////    QList<QString> mapMemberStr;

////    const QMultiMap<Discipline,Student> studentMap = teach.getVisitors();
////    const QVector<QString>* studentInits;
////    auto keys = studentMap.uniqueKeys();
////    for(auto i = keys.begin(); i != keys.end(); ++i){
////        auto values = studentMap.values(*i);
////        for(auto j = values.begin();j!=values.end();++j){
////            studentInits = j->getInitials();
////            mapMemberStr.append(i->getName() + " " +
////                                studentInits->at(0) + "#" +
////                                studentInits->at(1) + "#" +
////                                studentInits->at(2) + "#" +
////                                j->getPasword() + "/");
////        }
////    }
////    infoToPrint.append(mapMemberStr);

////    int j = 0;
////    for(auto&i: infoToPrint){
////        file.write((i + " ").toUtf8());
////        j++;
////        if(j == endOfInits) file.write("|");
////    }
////    file.write(QString("\n").toUtf8());
//    for(auto&i: infoToPrint){
//        file.write((i + " ").toUtf8());
//    }
//    file.write(QString("\n").toUtf8());
//    file.close();
//}




//void FlParser::readStudents(QVector<Student> &studs)
//{
//    file.open(QIODevice::ReadOnly);
//    while(!file.atEnd()){
//        QVector<QByteArray> studInfo = file.readLine().simplified().split('|').toVector();

//        QVector<QByteArray> inits = studInfo.at(0).split(' ').toVector();
//        studs.append(Student(inits.at(0),
//                             inits.at(1),
//                             inits.at(2),
//                             inits.at(3),
//                             inits.at(5)));
//        studs.last().setId(inits.at(4).toLong());

//        if(studInfo.size() > 1 && studInfo.last().size() > 5){
//            inits = studInfo.at(1).split('/').toVector();
//            for(auto &i : inits){
//                QVector<QByteArray> keyValue = i.split(' ').toVector();
//                QString disciplineName(keyValue.first());

//                QList<QByteArray> teacherInfo(keyValue.last().split('#'));
//                Teacher neededTeacher(teacherInfo.at(0),
//                                      teacherInfo.at(1),
//                                      teacherInfo.at(2),
//                                      teacherInfo.at(3));

//                Discipline neededDiscipl(disciplineName);
//                studs.last().addStudyTarget(neededDiscipl,
//                                            *std::find(allTeachers.begin(),allTeachers.end(),neededTeacher));
//            }
//        }
//    }
//    file.close();
//}






//void FlParser::readTeachers(QVector<Teacher> &teachers)
//{
//    file.open(QIODevice::ReadOnly);
//    while(!file.atEnd()){
//        QList<QByteArray> teachInfo = file.readLine().simplified().split('|');
//        bool hasVisitors = true;
//        if(teachInfo.size() == 2 && teachInfo.size() < 2)
//            hasVisitors = false;
//        QList<QByteArray> inits = teachInfo.at(0).split(' ');
//        teachers.append(Teacher(inits.at(0),
//                                inits.at(1),
//                                inits.at(2),
//                                inits.at(3),
//                                inits.at(4).toInt(),
//                                inits.at(6)));
//        teachers.last().setId(inits.at(5).toLong());
//        if(hasVisitors){
//            inits = teachInfo.at(1).split('/');
//            for(auto &i : inits){
//                QList<QByteArray> keyValue = i.split(' ');
//                QString courseName(keyValue.first());

//                QList<QByteArray> studentInfo(keyValue.last().split('#'));
//                Student neededStudent(studentInfo.at(0),
//                                      studentInfo.at(1),
//                                      studentInfo.at(2),
//                                      studentInfo.at(3));
//                Discipline neededDiscipl(courseName);
//                teachers.last().updateDisciplineWithStuds(neededDiscipl,
//                                                          *std::find(allStudents.begin(),allStudents.end(),neededStudent));

//            }
//        }
//    }
//    file.close();
//}


//void FlParser::readDisciplines(QVector<Discipline>& disciplines)
//{
//    file.open(QIODevice::ReadOnly);
//    while(!file.atEnd()){
//        QString data(file.readLine().simplified());

//        disciplines.append(Discipline(data));
//    }
//    file.close();
//}
