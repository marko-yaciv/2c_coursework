#include "flparser.h"

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

void FlParser::writeData(const QString & data)
{
    file.open(QIODevice::Append);
    file.write(data.toUtf8() + "\n");
    file.close();
}



void FlParser::writeData(const Student &stud)
{
    file.open(QIODevice::Append);
    file.flush();
    auto studInits = stud.getInitials();
    QList<QString> infoToPrint = QList<QString>::fromVector(*studInits);
    infoToPrint.append(stud.getGroup());
    infoToPrint.append(QString::number(stud.getId()));
    infoToPrint.append(stud.getPasword());
    for(auto&i: infoToPrint){
        file.write((i + " ").toUtf8());
    }
    file.write(QString("\n").toUtf8());
    file.close();
}

void FlParser::writeData(const Teacher & teach)
{
    file.open(QIODevice::Append);

    auto teachInits = teach.getInitials();
    QList<QString> infoToPrint = QList<QString>::fromVector(*teachInits);
    infoToPrint.append(teach.getPost());
    infoToPrint.append(QString::number(teach.getStage()));
    infoToPrint.append(QString::number(teach.getId()));
    infoToPrint.append(teach.getPasword());
    for(auto&i: infoToPrint){
        file.write((i + " ").toUtf8());
    }
    file.write(QString("\n").toUtf8());
    file.close();
}

//void FlParser::writeData(const Discipline& discipline)
//{
//    file.open(QIODevice::Append);

//    file.write((discipline.getName() + "\n").toUtf8());

//    file.close();
//}


void FlParser::readData(QList<QString> &data)
{
    file.open(QIODevice::ReadOnly);
    while(!file.atEnd()){
        auto lineData = file.readLine();
        data.append(lineData.remove(lineData.length()-1,1));
    }
    file.close();
}

void FlParser::readData(QVector<Student> &studs)
{
    file.open(QIODevice::ReadOnly);
    while(!file.atEnd()){
        QVector<QByteArray> studInfo = file.readLine().split(' ').toVector();
        studs.append(Student(studInfo.at(0),
                             studInfo.at(1),
                             studInfo.at(2),
                             studInfo.at(3),
                             studInfo.at(5)));
        studs.last().setId(studInfo.at(4).toLong());
    }
    file.close();
}

void FlParser::readData(QVector<Teacher> &teachers)
{
    file.open(QIODevice::ReadOnly);
    while(!file.atEnd()){
        QList<QByteArray> teachInfo = file.readLine().split(' ');
        teachers.append(Teacher(teachInfo.at(0),
                                teachInfo.at(1),
                                teachInfo.at(2),
                                teachInfo.at(3),
                                teachInfo.at(4).toInt(),
                                teachInfo.at(6)));
        teachers.last().setId(teachInfo.at(5).toLong());
    }
    file.close();
}

void FlParser::readData(QVector<Discipline>& disciplines)
{
    file.open(QIODevice::ReadOnly);
    while(!file.atEnd()){
        QString data(file.readLine());
        data.remove(data.length()-1,1);

        disciplines.append(Discipline(data));
    }
    file.close();
}
