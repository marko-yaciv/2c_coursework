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
    m_group = group;
    m_course = group.at(3).unicode() - 48;
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
    m_fName = fname;
    m_lName  = lname;
    m_fthName = fthname;
}

void Student::setStudyMap(const QMap<Discipline, Teacher> &map)
{
    for(auto i = map.begin(); i != map.end();++i){
        if(m_studyMap.contains(i.key())){
            m_studyMap[i.key()] = i.value();
        }
    }
}

void Student::addDiscipline(const Discipline &discipl)
{
    m_courses.push_back(discipl);
    //m_studyMap.insert(discipl,Teacher());
}

void Student::setGroup(const QString group)
{
    m_group = group;
    m_course = m_group.at(3).unicode() - 48;
}

void Student::addStudyTarget(const Discipline discipline,const Teacher teacher)
{
    if(m_courses.contains(discipline))
        m_studyMap.insert(discipline,teacher);
    else throw Except("This student doesn't have discipline, called \"" + discipline.getName() + "\"");
}

int Student::countDisciplines() const
{
    return m_studyMap.keys().size();
}

const QList<Discipline> &Student::getDisciplines() const
{
    return m_courses;
}

const QMap<Discipline, Teacher> &Student::getStudyMap() const
{
    return m_studyMap;
}

const QString &Student::getGroup() const
{
    return m_group;
}

int Student::getCourse() const
{
    return m_course;
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
}

bool Student::lessThan(const Student &other) const
{
    return m_lName < other.m_lName;
}

Student& Student::operator=(const Student &other)
{
    this->m_fName = other.m_fName;
    this->m_lName = other.m_lName;
    this->m_fthName = other.m_fthName;
    this->m_course = other.m_course;
    this->m_group = other.m_group;
    this->m_id = other.m_id;
    this->m_password = other.m_password;
    this->m_courses = other.m_courses;
    this->m_studyMap = other.m_studyMap;
    return *this;
}

bool Student::operator==(const Student &other) const
{
    return (getFname() == other.getFname()) &&
            (getLname() == other.getLname()) &&
            (getFthName() == other.getFthName()) &&
            (getPasword() == other.getPasword());
    //return this->getId() == other.getId();
}

bool Student::operator<(const Student &other) const
{
    return m_id < other.m_id;
}


