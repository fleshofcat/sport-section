#pragma once

#include <QtSql>

#include "people_storage.h"
#include "relation_storage.h"

class DbManager : public QObject
{
    Q_OBJECT

    PeopleStorage *people;
    RelationStorage *relations;
public:

    explicit DbManager(QString db_path, QObject *parent = nullptr)
        : QObject (parent)
    {        
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(db_path);

        if (db.open() == false)
        {
            qDebug() << "Error: Database connection is failed";
            return;
        }
        people = new PeopleStorage(this);
        relations = new RelationStorage(this);
    }

    // people
    bool addPerson(Person pers)
    {
        return people->addPerson(pers);
    }

    bool removePerson(Person pers)
    {
        if (isBelongToSomeRecord(pers, *this->getRecords()) == false)
        {
            return people->removePerson(pers);
        }
        return false;
    }

    bool replacePersonById(Person pers)
    {
        return people->replacePersonById(pers);
    }

    QList<Person> *getChildren()
    {
        QList<Person> peopleList = *people->getAllPeople();
        QList<Person> *children = new QList<Person>;

        for (Person pers : peopleList)
        {
            if (pers.isTrainer == false)
            {
                *children << pers;
            }
        }
        return children;
    }

    QList<Person> *getTrainers()
    {
        QList<Person> peopleList = *people->getAllPeople();
        QList<Person> *trainers = new QList<Person>;

        for (Person pers : peopleList)
        {
            if (pers.isTrainer == true)
            {
                *trainers << pers;
            }
        }
        return trainers;
    }

    QList<Person> *getPeople()
    {
        return people->getAllPeople();
    }


    // records
    bool addRecord(Record record)
    {
        if (isRecordValid(record, *this->people->getAllPeople()))
        {
            return relations->addRelation(record);
        }
        return false;
    }

    bool removeRecord(Record record)
    {
        return relations->removeRelation(record);
    }

    bool replaceRecordById(Record record)
    {
        if (isRecordValid(record, *this->people->getAllPeople()))
        {
            return relations->replaceRelationById(record);
        }
        return false;
    }

    QList<Record> *getRecords()
    {
        return relations->getAllRelations();
    }

    ~DbManager()
    {
        {
            QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
            db.close();
        }
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

private:
    bool isPersonExist(int pers_id, QList<Person> people)
    {
        for (Person pers : people)
        {
            if (pers.id == pers_id)
            {
                return true;
            }
        }
        return false;
    }

    bool isBelongToSomeRecord(Person pers, QList<Record> records)
    {
        for (Record record : records)
        {
            if (record.child_id == pers.id
                    || record.trainer_id == pers.id)
            {
                return true;
            }
        }
        return false;
    }

    bool isRecordValid(Record record, QList<Person> people)
    {
        if (isPersonExist(record.child_id, people)
                && isPersonExist(record.trainer_id, people))
        {
            return true;
        }
        return false;
    }


};




















