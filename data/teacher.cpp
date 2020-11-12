#include "teacher.h"
#include "discipline.h"
#include "student.h"
extern QMap<short,QString> postNames;
Teacher::Teacher():Account("None"), Human("None","None","None")
{
    this->m_post = "None";
    this->m_stage = 0;
    this->m_populatity = 0;
}

Teacher::Teacher(const QString& fname, const QString& lname, const QString& fthname,
                 const QString& post, const int stage, const QString& password):
                 Account(password), Human(fname, lname,fthname)
{
    this->m_post = post;
    this->m_stage = stage;
    this->m_populatity = 0;
    //this->m_courseStudents = new QMultiMap<Discipline,Student>;
}

Teacher::Teacher(const QString &fname, const QString &lname, const QString &fthname,
                 const QString &password):
                 Account(password), Human(fname, lname,fthname)
{

}

Teacher::~Teacher()
{
    //delete this->m_courseStudents;
}

void Teacher::setInitials(const QString& fname, const QString& lname, const QString& fthname)
{
    this->m_fName = fname;
    this->m_lName = lname;
    this->m_fthName = fthname;
}

void Teacher::setPost(const QString& post)
{
    this->m_post = post;
}

void Teacher::setStage(const int stage)
{
    this->m_stage = stage;
}

void Teacher::setCourseVisitors(QMultiMap<Discipline, Student> map)
{
    this->m_courseVisitors = map;
}

void Teacher::updateDisciplineWithStuds(Discipline &discipline,Student &stud)
{
    this->m_courseVisitors.insert(discipline, stud);
    ++m_populatity;
}


void Teacher::setDiscipline(const Discipline& discipline)
{
    this->m_courseVisitors.insert(discipline, Student());
    ++m_populatity;
}

bool Teacher::hasDiscipline(const Discipline &discipl) const
{
    return this->m_courseVisitors.contains(discipl);
}

//int Teacher::addStudent(const Student& newStudent)
//{
//    this->m_students.append(newStudent);
//    return m_students.size();
//}

const QVector<QString> *Teacher::getInitials() const
{
    return new QVector<QString>({m_lName, m_fName, m_fthName});
}

const QString &Teacher::getPost() const
{
    return this->m_post;
}

int Teacher::getStage() const
{
    return this->m_stage;
}

int Teacher::getPopularity() const
{
    return m_populatity;
}

const QList<Discipline>* Teacher::getDissciplines() const
{
    return new QList<Discipline>(this->m_courseVisitors.keys());
}
const QList<Student>* Teacher::getSudents() const
{
    return new QList<Student>(this->m_courseVisitors.values());
}

const QList<Student>* Teacher::getSudents(const Discipline &discipl) const
{
    return new QList<Student>(this->m_courseVisitors.values(discipl));
}

const QMultiMap<Discipline, Student>& Teacher::getVisitors() const
{
    return this->m_courseVisitors;
}

bool Teacher::operator==(const Teacher &other) const
{
    return (*this->getInitials() == *other.getInitials()) &&
            (this->getPasword() == other.getPasword());
    //return this->getId() == other.getId();
}

bool Teacher::operator!=(const Teacher &other) const
{
    return (*this->getInitials() != *other.getInitials()) &&
            (this->getPasword() != other.getPasword());
}

bool Teacher::operator>(const Teacher &other) const
{
    return postNames.key(m_post) > postNames.key(other.m_post);
}

bool Teacher::operator<(const Teacher &other) const
{
    return m_lName < other.m_lName;
}

void Teacher::write(QJsonObject &json) const
{
    json["fname"] = m_fName;
    json["lname"] = m_lName;
    json["fthname"] = m_fthName;
    json["post"] = m_post;
    json["stage"] = m_stage;
    json["popularity"] = m_populatity;
    json["password"] = m_password;
    json["id"] = (int)m_id;

    QJsonArray courseMap;
    for(auto i = m_courseVisitors.begin(); i != m_courseVisitors.end(); ++i){
        //if(i.value().getGroup() == "None") continue;
        QJsonArray keyVal;
        QJsonObject key, val;

        i.key().write(key);
        i.value().write(val);

        keyVal.append(key);
        keyVal.append(val);

        courseMap.append(keyVal);
    }
    json["coursemap"] = courseMap;
}

void Teacher::read(const QJsonObject &json)
{
    if(json.contains("fname") && json["fname"].isString())
        m_fName = json["fname"].toString();
    if(json.contains("lname") && json["lname"].isString())
        m_lName = json["lname"].toString();
    if(json.contains("fthname")&& json["fthname"].isString())
        m_fthName = json["fthname"].toString();

    if(json.contains("post") && json["post"].isString())
        m_post = json["post"].toString();
    if(json.contains("stage") && json["stage"].isDouble())
        m_stage = json["stage"].toInt();
    if(json.contains("popularity")&& json["popularity"].isDouble())
        m_populatity = json["popularity"].toInt();

    if(json.contains("password") && json["password"].isString())
        m_password = json["password"].toString();
    if(json.contains("id")&& json["id"].isDouble())
        m_id = json["id"].toInt();

    if(json.contains("coursemap") && json["coursemap"].isArray()){
        QJsonArray courseMap = json["coursemap"].toArray();
        m_courseVisitors.clear();
        for(int i  = 0; i < courseMap.size(); ++i){
            QJsonArray keyVal = courseMap[i].toArray();

            Discipline dis;
            Student stud;

            dis.read(keyVal.first().toObject());
            stud.read(keyVal.last().toObject());

            m_courseVisitors.insert(dis,stud);
        }
    }

}
