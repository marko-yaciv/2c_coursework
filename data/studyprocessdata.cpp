#include "studyprocessdata.h"
StudyProcessData* StudyProcessData::allData = 0;
const int StudyProcessData::COURSES = 4;

StudyProcessData::StudyProcessData()
{
    m_allDisciplines.resize(COURSES);
}

StudyProcessData::StudyProcessData(const StudyProcessData &)
{

}

StudyProcessData &StudyProcessData::operator=(StudyProcessData &)
{
    return *this;
}

StudyProcessData::~StudyProcessData()
{

}

StudyProcessData *StudyProcessData::getInstance()
{
    if(!allData)
        allData = new StudyProcessData();
    return allData;
}

void StudyProcessData::updateMapForStudent(const Student &mapOwner, const QMap<Discipline, Teacher>& map)
{
        m_studentsStudyMaps.insert(mapOwner,map);
}

void StudyProcessData::updateMapForTeacher(const Teacher &mapOwner, const QMultiMap<Discipline, Student>& map)
{
    m_teachersCourseMaps.insert(mapOwner,map);
}

QList<Student>& StudyProcessData::getAllStudents()
{
    return m_allStudents;
}

QList<Teacher>& StudyProcessData::getAllTeachers()
{
    return m_allTeachers;
}

QVector<QVector<Discipline>>& StudyProcessData::getAllDisciplines()
{
    return m_allDisciplines;
}

QMap<short, QString> &StudyProcessData::getPostNames()
{
    return m_postNames;
}

QList<QString> &StudyProcessData::getGroups()
{
    return m_groups;
}

void StudyProcessData::setAllDisciplines(const QVector<QVector<Discipline> >& disciplines)
{
    m_allDisciplines = disciplines;
}

void StudyProcessData::setPostNames(const QMap<short, QString>& names)
{
    m_postNames = names;
}

void StudyProcessData::setGroups(const QList<QString>& groups)
{
    m_groups = groups;
}

//---------writing functions---------

void StudyProcessData::writeMapsForStudents(QJsonObject &owners) const
{
    for(auto i = m_studentsStudyMaps.begin();i != m_studentsStudyMaps.end();++i)
    {
        QJsonObject studyTarget;
        for(auto j = i.value().begin();j != i.value().end();++j)
        {
            studyTarget[j.key().getName()] = (int)j.value().m_id;
        }
        owners[QString::number(i.key().getId())] =  studyTarget;
    }
}

void StudyProcessData::writeMapsForTeachers(QJsonObject &owners) const
{
    for(auto i = m_teachersCourseMaps.begin(); i != m_teachersCourseMaps.end(); ++i)
    {
        QJsonObject studyTarget;
        auto teacherCourses = i.value().keys();

        for(auto j = teacherCourses.begin(); j != teacherCourses.end(); ++j)
        {
            QJsonArray ownerMap;
            auto students = i.value().values(*j);

            for(auto &student : students)
                ownerMap.append((int)student.m_id);
            studyTarget[j->getName()] = ownerMap;
        }
        owners[QString::number(i.key().m_id)] =  studyTarget;
    }
}


//------reading functions-----

void StudyProcessData::readMapsOfStudents(const QJsonObject &owners)
{
    m_studentsStudyMaps.clear();


    for(auto& student : m_allStudents)
    {
        QString ownerId = QString::number(student.m_id);

        if(owners.contains(ownerId) && owners[ownerId].isObject())
        {
            QJsonObject ownerMap = owners[ownerId].toObject();

            fetchTeachersWithDisciplines(student,ownerMap);

            m_studentsStudyMaps.insert(student,student.m_studyMap);
        }
    }
}
void StudyProcessData::fetchTeachersWithDisciplines(Student& student, QJsonObject& ownerMap)
{
    for(auto disciplOfCourse = m_allDisciplines[student.m_course-1].begin();
             disciplOfCourse != m_allDisciplines[student.m_course-1].end(); ++disciplOfCourse)
    {
        if(ownerMap.contains(disciplOfCourse->getName()))
        {
            int teacherId = ownerMap[disciplOfCourse->getName()].toInt();

            //finding teacher in allteachers with needed ID <teacherId> and making appropriate map
            for(auto& teacher : m_allTeachers)
            {
                if(teacher.m_id == teacherId)
                {
                    student.m_studyMap.insert(*disciplOfCourse,teacher);
                    break;
                }
            }
        }
    }
}


void StudyProcessData::readMapsOfTeachers(const QJsonObject &owners)
{
    m_teachersCourseMaps.clear();

    for(auto& teacher : m_allTeachers)
    {
        QString ownerId = QString::number(teacher.m_id);

        if(owners.contains(ownerId) && owners[ownerId].isObject())
        {
            QJsonObject ownerMap = owners[ownerId].toObject();

            fetchStudentsWithDisciplines(teacher,ownerMap);

            m_teachersCourseMaps.insert(teacher,teacher.m_courseVisitors);
        }
    }
}
void StudyProcessData::fetchStudentsWithDisciplines(Teacher& teacherItr, QJsonObject& ownerMap)
{
    for(auto course = teacherItr.m_courses.begin(); course != teacherItr.m_courses.end(); ++course)
    {
        if(ownerMap.contains(course->getName()))
        {
            auto studentIds = ownerMap[course->getName()].toArray();
            for(auto id : studentIds)
            {
                //finding student in allStudents with needed ID and making appropriate map
                for(auto &student : m_allStudents)
                {
                    if(student.m_id == id.toInt())
                    {
                        teacherItr.m_courseVisitors.insert(*course,student);
                        break;
                    }
                }
            }

        }
    }
}


