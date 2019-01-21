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

    explicit DbManager(QString db_path, QObject *parent = nullptr) : QObject (parent)
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
        return people->removePerson(pers);
    }


    bool replacePersonById(Person pers)
    {
        return people->replacePersonById(pers);
    }


    QList<Person> *getAllPersons()
    {
        return people->getAllPeople();
    }


    // relations
    bool addRelation(Relation rel)
    {
        return relations->addRelation(rel);
    }


    bool removeRelation(Relation rel)
    {
        return relations->removeRelation(rel);
    }


    bool replaceRelationById(Relation rel)
    {
        return relations->replaceRelationById(rel);
    }


    QList<Relation> *getAllRelations()
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
};




















