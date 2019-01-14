#include "db_manager.h"

DbManager::DbManager(QString path, QObject *parent) : QObject(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (db.open())
        qDebug() << "Database: connection ok";
    else
        qDebug() << "Error: connection with database fail";
}


bool DbManager::addPerson(QString firstName, QString lastName,
                          QString birthday, QString description)
{
    QSqlQuery query;
    query.prepare("INSERT INTO people (first_name, last_name, birthday, description)"
                  " VALUES (:first_name, :last_name, :birthday, :description)");

    query.bindValue(":first_name", firstName);
    query.bindValue(":last_name", lastName);
    query.bindValue(":birthday", birthday);
    query.bindValue(":description", description);

    bool ret = query.exec();

    if (!ret)
        qDebug() << "query.exec() in addPerson failed";

    return ret;
}


bool DbManager::addPerson(Person person)
{
    if (person.isFull() == false)
        return false;

    return addPerson(person.firstName, person.lastName,
              person.birthday, person.description);
}


bool DbManager::removePerson(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM people WHERE id = (:id)");
    query.bindValue(":id", id);

    bool ret = query.exec();

    if (!ret)
        qDebug() << "query.exec() in removePerson failed";

    return ret;
}

// TODO add exist examen to a person
bool DbManager::removePerson(Person person)
{
    return removePerson(person.id);
}


bool DbManager::editPersonById(int id, QString firstName, // TODO
                           QString lastName, QString birthday,
                           QString description)
{
    QSqlQuery query;
    query.prepare("UPDATE people                    "
                  "SET  first_name = (:first_name), "
                  "     last_name = (:last_name),   "
                  "     birthday = (:birthday),     "
                  "     description = (:description)"
                  "WHERE id = (:id)               ");


    query.bindValue(":id", id);

    query.bindValue(":first_name", firstName);
    query.bindValue(":last_name", lastName);
    query.bindValue(":birthday", birthday);
    query.bindValue(":description", description);

    bool ret = query.exec();

    if (!ret)
        qDebug() << "query.exec() in editPersonById failed";

    return ret;
}


bool DbManager::editPersonById(Person person)
{
    return editPersonById(person.id, person.firstName,
                          person.lastName, person.birthday,
                          person.description);
}


Person *DbManager::getPerson(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM people WHERE id = (:id)");
    query.bindValue(":id", id);

    Person *person;

    if (query.exec())
    {
        if (query.next())
        {
            person = new Person();

            person->id = query.value("id").toInt();
            person->firstName = query.value("first_name").toString();
            person->lastName = query.value("last_name").toString();
            person->birthday = query.value("birthday").toString();
            person->description = query.value("description").toString();

            return person;
        }
    }

    qDebug() << "query.exec() in getPerson failed";
    return nullptr;
}


QList<Person> *DbManager::getAllPeople()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM people");

    QList<Person> *peopleList = new QList<Person>();

    if (query.exec())
    {
        while (query.next())
        {

            Person *person = new Person();

            person->id = query.value("id").toInt();
            person->firstName = query.value("first_name").toString();
            person->lastName = query.value("last_name").toString();
            person->birthday = query.value("birthday").toString();
            person->description = query.value("description").toString();

            *peopleList << *person;
        }
        return peopleList;
    }

    qDebug() << "query.exec() in getAllPeople failed";
    return nullptr;
}



















