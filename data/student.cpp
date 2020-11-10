#include "student.h"
#include "discipline.h"
#include "teacher.h"
Student::Student():Account("None"), Human()
{
    this->m_course = 0;
    this->m_group = "None";
}

Student::Student(const QString& fname, const QString& lname, const QString& fthname,
                 const QString& group, const QString& password):
                 Account(password),Human(fname, lname,fthname)
{
    this->m_group = group;
    this->m_course = group.at(3).unicode() - 48;
    //this->m_studyMap = new QMap<Discipline, Teacher>;
}

Student::Student(const QString &fname, const QString &lname, const QString &fthname,
                 const QString &password):
                Account(password),Human(fname, lname,fthname)
{

}

Student::~Student()
{
    //delete this->m_studyMap;
}

void Student::setInitials(const QString fname, const QString lname, const QString fthname)
{
    this->m_fName = fname;
    this->m_lName  = lname;
    this->m_fthName = fthname;
}

void Student::setStudyMap(const QMap<Discipline, Teacher> &map)
{
    for(auto i = map.begin(); i != map.end();++i){
        if(this->m_studyMap.contains(i.key())){
            m_studyMap[i.key()] = i.value();
        }
    }
}

void Student::addDiscipline(const Discipline &discipl)
{
    this->m_studyMap.insert(discipl, Teacher());
}

void Student::setGroup(const QString group)
{
    this->m_group = group;
    this->m_course = m_group.at(3).unicode() - 48;
}

void Student::addStudyTarget(const Discipline &discipline,const Teacher &teacher)
{
    if(m_studyMap.contains(discipline)){
        this->m_studyMap[discipline] = teacher;
    }else{
        this->m_studyMap.insert(discipline,teacher);
    }
}

int Student::countDisciplines() const
{
    return m_studyMap.keys().size();
}

const QMap<Discipline, Teacher> &Student::getStudyMap() const
{
    return this->m_studyMap;
}

const QVector<QString>* Student::getInitials() const
{
    return new QVector<QString>({m_fName, m_lName, m_fthName});
}

const QString &Student::getGroup() const
{
    return this->m_group;
}

int Student::getCourse() const
{
    return this->m_course;
}

//const QList<Discipline>* Student::getDisciplines() const
//{
//    return new QList<Discipline>(this->m_studyMap.keys());
//}

//const QList<Teacher>* Student::getTeachers() const
//{
//    return new QList<Teacher>(this->m_studyMap.values());
//}


const Teacher* Student::getTeacher(const Discipline& discipl) const
{
    if(m_studyMap.contains(discipl)){
        return new Teacher(m_studyMap[discipl]);
    }
    throw 0;
}

const Discipline* Student::getDiscipline(const Teacher& teach) const
{
    for(auto &i : this->m_studyMap){
        if(i == teach){
            return new Discipline(this->m_studyMap.key(teach));
        }
    }
    throw 0;
}

void Student::write(QJsonObject &json) const
{
    json["fname"] = m_fName;
    json["lname"] = m_lName;
    json["fthname"] = m_fthName;
    json["course"] = m_course;
    json["group"] = m_group;
    json["password"] = m_password;
    json["id"] = (int)m_id;
    QJsonArray courseMap;
    for(auto i = m_studyMap.begin(); i != m_studyMap.end(); ++i){
        if(i.value().getPost() == "None") continue;
        QJsonArray keyVal;

        QJsonObject key;
        QJsonObject val;

        i.key().write(key);
        i.value().write(val);

        keyVal.append(key);
        keyVal.append(val);

        courseMap.append(keyVal);
    }
    json["studymap"] = courseMap;
}

void Student::read(const QJsonObject json)
{
    if(json.contains("fname")&& json["fname"].isString())
        m_fName = json["fname"].toString();
    if(json.contains("lname")&& json["lname"].isString())
        m_lName = json["lname"].toString();
    if(json.contains("fthname")&& json["fthname"].isString())
        m_fthName = json["fthname"].toString();

    if(json.contains("group")&& json["group"].isString())
        m_group = json["group"].toString();
    if(json.contains("course")&& json["course"].isDouble())
        m_course = json["course"].toInt();

    if(json.contains("password")&& json["password"].isString())
        m_password = json["password"].toString();
    if(json.contains("id")&& json["id"].isDouble())
        m_id = json["id"].toInt();

    if(json.contains("studymap")&& json["studymap"].isArray()){
        QJsonArray studymap = json["studymap"].toArray();
        m_studyMap.clear();
        for(int i = 0; i < studymap.size(); ++i){
            const QJsonArray keyVal = studymap[i].toArray();

            Discipline dis;
            Teacher teach;
            dis.read(keyVal.first().toObject());
            teach.read(keyVal.last().toObject());

            m_studyMap.insert(dis,teach);
        }
    }


}

void Student::operator=(const Student &other)
{
    this->m_fName = other.m_fName;
    this->m_lName = other.m_lName;
    this->m_fthName = other.m_fthName;
    this->m_course = other.m_course;
    this->m_group = other.m_group;
    this->m_id = other.m_id;
    this->m_password = other.m_password;
    this->m_studyMap = other.m_studyMap;
}

bool Student::operator==(const Student &other) const
{
    return (*this->getInitials() == *other.getInitials()) &&
            (this->getPasword()==other.getPasword());
    //return this->getId() == other.getId();
}
