#pragma once

#include <QtSql>
#include <QList>

#include "common_objects.h"

class PeopleManager : public QObject
{
    Q_OBJECT
    QString tableName = "people";

public:
    explicit PeopleManager(QObject *parent = nullptr)
        : QObject(parent)
    {                   // create people table in db if not exist
            QSqlQuery query("SELECT name FROM sqlite_master"
                            " WHERE name='" + tableName + "'");

            query.next();
            if (query.value(0).toString() != tableName)
            {
                query.exec("CREATE TABLE IF NOT EXISTS people (                 "
                           " id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,     "
                           " first_name TEXT NOT NULL,                          "
                           " last_name TEXT NOT NULL,                           "
                           " birthday TEXT NOT NULL,                            "
                           " sport_type TEXT NOT NULL,                          "
                           " is_trainer INTEGER )                               ");

                qDebug() << "Creating people table inside database";
            }
    }


    bool addPerson(Person person)
    {
        if (person.isFull() == false)
            return false;

        QSqlQuery query;
        query.prepare("INSERT INTO " + tableName + "                            "
        "        (first_name, last_name, birthday, sport_type, is_trainer)      "
        " VALUES (:first_name, :last_name, :birthday, :sport_type, :is_trainer) ");

        query.bindValue(":first_name", person.firstName);
        query.bindValue(":last_name", person.lastName);
        query.bindValue(":birthday", person.birthday);
        query.bindValue(":sport_type", person.sportType);
        query.bindValue(":is_trainer", person.isTrainer);

        bool ret = query.exec();

        if (!ret)
            qDebug() << "query.exec() in add person failed";

        return ret;
    }


    bool removePerson(Person person)
    {
        QSqlQuery query;

        query.prepare("DELETE FROM " + tableName + " WHERE id = (:id)");
        query.bindValue(":id", person.id);

        bool ret = query.exec();

        if (!ret)
            qDebug() << "query.exec() in remove person failed";

        return ret;
    }


    bool replacePersonById(Person person)
    {
        QSqlQuery query;
        query.prepare(" UPDATE " + tableName + " SET    "
                      "     first_name = (:first_name), "
                      "     last_name = (:last_name),   "
                      "     birthday = (:birthday),     "
                      "     sport_type = (:sport_type), "
                      "     is_trainer = (:is_trainer)  "
                      " WHERE id = (:id)                ");


        query.bindValue(":id", person.id);
        query.bindValue(":first_name", person.firstName);
        query.bindValue(":last_name", person.lastName);
        query.bindValue(":birthday", person.birthday);
        query.bindValue(":sport_type", person.sportType);
        query.bindValue(":is_trainer", person.isTrainer);

        bool ret = query.exec();

        if (!ret)
            qDebug() << "query.exec() in replace person failed";

        return ret;
    }


    Person getPerson(int id)
    {
        QSqlTableModel sqlModel;
        sqlModel.setTable("people");
        sqlModel.setFilter("id = " + QString::number(id));
        sqlModel.select();

        Person pers;

        pers.id = sqlModel.record(0).value("id").toInt();
        pers.firstName = sqlModel.record(0).value("first_name").toString();
        pers.lastName = sqlModel.record(0).value("last_name").toString();
        pers.birthday = sqlModel.record(0).value("birthday").toString();
        pers.sportType = sqlModel.record(0).value("sport_type").toString();
        pers.isTrainer = sqlModel.record(0).value("is_trainer").toBool();

        return pers;
    }


    QList<Person> *getAllPeople()
    {
        QSqlQuery query;
        query.prepare("SELECT * FROM " + tableName);

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
                person->sportType = query.value("sport_type").toString();
                person->isTrainer = query.value("is_trainer").toBool();

                *peopleList << *person;
            }
            return peopleList;
        }

        qDebug() << "query.exec() in getAllPeople failed";
        return nullptr;
    }
};






