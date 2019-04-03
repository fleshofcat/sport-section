#pragma once

#include <QtSql>

#include "common/person.h"

class PeopleManager : public QObject
{
    Q_OBJECT

    QString tableName;

public:
    explicit PeopleManager(QObject *parent = nullptr);
    explicit PeopleManager(QString tableName,
                           QObject *parent = nullptr);

    void touchManager(QString table);

    bool savePerson(Person pers);
    bool removePerson(int id);

    QList<Person> getPeople();
    Person getPerson(int id);

private:
    bool addPerson(Person person);
    bool updatePerson(Person person);
};






