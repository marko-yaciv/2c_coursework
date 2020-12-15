#include "teacher.h"
#include "discipline.h"
#include "student.h"

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
}

Teacher::Teacher(const QString &fname, const QString &lname, const QString &fthname,
                 const QString &password):
                 Account(password), Human(fname, lname,fthname)
{

}

Teacher::~Teacher()
{

}

//----setters-----
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

void Teacher::addDiscipline(const Discipline& discipline)
{
    if(!m_courses.contains(discipline)){
        m_courses.append(discipline);
        ++m_populatity;
    }
}

void Teacher::addCourseTarget(const Discipline discipline,const Student stud)
{
        if(!m_courseVisitors.contains(discipline,stud)){
            m_courseVisitors.insert(discipline,stud);
            ++m_populatity;
        }
}



void Teacher::removeDiscipline(const Discipline &discipline)
{
        m_courses.removeOne(discipline);
        --m_populatity;
}

void Teacher::removeCourseTarget(const Discipline& discipline,const Student& stud)
{
    if(m_courseVisitors.contains(discipline,stud)){
       m_courseVisitors.remove(discipline,stud);
       m_courses.removeOne(discipline);
        --m_populatity;
    }
    else {
        throw Except("");
    }
}

//-----getters-----

//std::unique_ptr<QList<Student>> Teacher::getCourseVistors(const Discipline &dis) const
//{
//    return std::unique_ptr<QList<Student>>(new QList<Student>(m_courseVisitors.values(dis)));
//}

QMultiMap<Discipline, Student>& Teacher::getCourseMap()
{
    return m_courseVisitors;
}

const QList<Discipline> &Teacher::getDisciplines() const
{
    return m_courses;
}

const QString &Teacher::getPost() const
{
    return m_post;
}

int Teacher::getPopularity() const
{
    return m_populatity;
}

int Teacher::getStage() const
{
    return m_stage;
}



bool Teacher::hasDiscipline(const Discipline &discipl) const
{
    return m_courses.contains(discipl);
}


//----wrire & read functions---

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
    QJsonArray courses;
    for(auto&i:m_courses){
        courses.append(i.getName());
    }
    json["courses"] = courses;

}

void Teacher::read(const QJsonObject &json, const QVector<QVector<Discipline>>& allDisciplines)
{
    if(json.contains("fname") && json["fname"].isString())
        m_fName = json["fname"].toString();

    if(json.contains("lname") && json["lname"].isString())
        m_lName = json["lname"].toString();

    if(json.contains("fthname")&& json["fthname"].isString())
        m_fthName = json["fthname"].toString();
;
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

    if(json.contains("courses") && json["courses"].isArray()){
        QJsonArray courses = json["courses"].toArray();
        fetchDisciplines(courses,allDisciplines);
    }
}


void Teacher::fetchDisciplines(const QJsonArray &courses, const QVector<QVector<Discipline>>& allDisciplines)
{
    for(const auto &i : courses){
        for(auto &j : allDisciplines){
            for(auto &discipline : j){
                if(discipline.getName() == i.toString()){
                    m_courses.append(discipline);
                    if(m_courses.size() == courses.size()) return;
                }
            }
        }
    }
}


//----overloaded operators and compare functions--

bool Teacher::compareByPost(const Teacher &other, const QMap<short,QString>& postNames) const
{
   return postNames.key(m_post) > postNames.key(other.m_post);
}

bool Teacher::operator==(const Teacher &other) const
{
    return (this->getFname() == other.getFname()) &&
            (this->getLname() == other.getLname()) &&
            (this->getFthName() == other.getFthName()) &&
            (this->getPasword() == other.getPasword());

}

bool Teacher::operator!=(const Teacher &other) const
{
    return !(*this == other);
}

bool Teacher::operator>(const Teacher &other) const
{
    return m_lName > other.m_lName;
}

bool Teacher::operator<(const Teacher &other) const
{
    return m_lName < other.m_lName;
}
