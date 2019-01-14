#pragma once

#include <QtSql>
#include <QList>
#include <src/common/person.h>

class DbManager : public QObject
{
    Q_OBJECT
public:
    explicit DbManager(QString path, QObject *parent = nullptr);

    bool addPerson(Person person);
    bool addPerson(QString firstName, QString lastName,
                   QString birthday, QString description);

    bool removePerson(Person person);
    bool removePerson(int id);

    bool editPersonById(Person person); // identification occurs by person.id
    bool editPersonById(int id, QString firstName, QString lastName, // deprecated
                    QString birthday, QString description);

    Person *getPerson(int id);

    QList<Person> *getAllPeople();
};

