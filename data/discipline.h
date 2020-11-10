#ifndef DISCIPLINE_H
#define DISCIPLINE_H
#include <QString>
#include <QVector>
#include <QJsonObject>
class Discipline
{
    QString m_name;
public:
    Discipline();
    Discipline(const QString& name);
    ~Discipline();

    void rename(QString& newName);

    QString getName() const;

    void read(const QJsonObject& json);
    void write(QJsonObject& json)const;

    bool operator<(const Discipline& other) const;
    bool operator==(const Discipline&other) const;
    bool operator!=(const Discipline&other) const;

};

#endif // DISCIPLINE_H
