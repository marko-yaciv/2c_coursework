#include "studyprocessdata.h"

extern QVector<Teacher> allTeachers;
extern QVector<Student> allStudents;
extern QVector<QVector<Discipline>> allDisciplines;
StudyProcessData::StudyProcessData()
{

}

StudyProcessData::~StudyProcessData()
{

}

void StudyProcessData::updateMapForStudent(const Student &mapOwner, const QMap<Discipline, Teacher> map)
{
    if(m_studentsStudyMaps.contains(mapOwner)){
        m_studentsStudyMaps[mapOwner] = map;
    }else{
        m_studentsStudyMaps.insert(mapOwner,map);
    }
}

void StudyProcessData::updateMapForTeacher(const Teacher &mapOwner, const QMultiMap<Discipline, Student> map)
{
    if(m_teachersCourseMaps.contains(mapOwner)){
        m_teachersCourseMaps[mapOwner] = map;
    }else{
        m_teachersCourseMaps.insert(mapOwner,map);
    }
}

const QMap<Discipline, Teacher> StudyProcessData::getMapOfStudent(const Student &mapOwner) const noexcept(false)
{
    if(m_studentsStudyMaps.contains(mapOwner)){
        return m_studentsStudyMaps[mapOwner];
    }else{
        throw nullptr;
    }
}

const QMultiMap<Discipline, Student> StudyProcessData::getMapOfTeacher(const Teacher &mapOwner) const noexcept(false)
{
    if(m_teachersCourseMaps.contains(mapOwner)){
        return m_teachersCourseMaps[mapOwner];
    }else{
        throw nullptr;
    }
}

void StudyProcessData::writeMapsForStudents(QJsonObject &owners) const
{
    for(auto i = m_studentsStudyMaps.begin();i != m_studentsStudyMaps.end();++i){
        //QJsonArray ownerMap;
        QJsonObject studyTarget;
        for(auto j = i.value().begin();j != i.value().end();++j){

            studyTarget[j.key().getName()] = (int)j.value().m_id;
          //  ownerMap.append(studyTarget);
        }
        owners[QString::number(i.key().getId())] =  studyTarget;
    }
}

void StudyProcessData::writeMapsForTeachers(QJsonObject &owners) const
{
    for(auto i = m_teachersCourseMaps.begin(); i != m_teachersCourseMaps.end(); ++i){
        //QJsonArray ownerMap;
        QJsonObject studyTarget;
        for(auto j = i.value().begin(); j != i.value().end(); ++j){

            studyTarget[j.key().getName()] = (int)j.value().m_id;
            //ownerMap.append(studyTarget);
        }
        owners[QString::number(i.key().m_id)] =  studyTarget;
    }
}

void StudyProcessData::readMapsOfStudents(const QJsonObject &owners)
{
    m_studentsStudyMaps.clear();
    for(auto i = allStudents.begin(); i != allStudents.end(); ++i)
    {
        QString ownerId = QString::number(i->m_id);

        if(owners.contains(ownerId) && owners[ownerId].isObject())
        {
            QJsonObject ownerMap = owners[ownerId].toObject();
            QMap<Discipline,Teacher> studyMap;

            //loop for allDisciplines of student <i>
            for(auto k = allDisciplines[i->m_course-1].begin(); k != allDisciplines[i->m_course-1].end(); ++k)
            {
                //check if we have a key in json with needed discipline
                if(ownerMap.contains(k->getName()))
                {
                    int teacherId = ownerMap[k->getName()].toInt();
                    //finding teacher in allteachers with needed ID <teacherId> and making appropriate map
                    for(auto&i:allTeachers)
                    {
                        if(i.m_id == teacherId)
                        {
                            studyMap.insert(*k,i);
                            break;
                        }
                    }
                }
            }
            i->m_studyMap = studyMap;
            m_studentsStudyMaps.insert(*i,studyMap);
        }
    }

}

void StudyProcessData::readMapsOfTeachers(const QJsonObject &owners)
{
    for(auto i = allTeachers.begin(); i != allTeachers.end(); ++i)
    {
        QString ownerId = QString::number(i->m_id);

        if(owners.contains(ownerId) && owners[ownerId].isObject())
        {
            QJsonObject ownerMap = owners[ownerId].toObject();
            QMultiMap<Discipline,Student> courseMap;

            //loop for teacher's courses
            for(auto course = i->m_courses.begin(); course != i->m_courses.end(); ++course)
            {
                //check if we have a key in json with needed discipline
                if(ownerMap.contains(course->getName()))
                {
                    int studentId = ownerMap[course->getName()].toInt();

                    //finding student in allStudents with needed ID and making appropriate map
                    for(auto &student : allStudents)
                    {
                        if(student.m_id == studentId)
                        {
                            courseMap.insert(*course,student);
                            break;
                        }
                    }
                }
            }
            i->m_courseVisitors = courseMap;
            m_teachersCourseMaps.insert(*i,courseMap);
        }
    }
}


