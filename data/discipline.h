#ifndef DISCIPLINE_H
#define DISCIPLINE_H
#include <QString>
#include <QVector>
class Discipline
{
    QString m_name;
public:
    Discipline();
    Discipline(QString& name);
    ~Discipline();

    void rename(QString& newName);

    QString getName() const;

    bool operator<(const Discipline& other) const;
};

#endif // DISCIPLINE_H
