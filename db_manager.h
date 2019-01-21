#pragma once

#include <QtSql>

#include "people_storage.h"
#include "relation_storage.h"

class DbManager : public QObject
{
    Q_OBJECT

    PeopleStorage people;
    RelationStorage relations;
public:

    explicit DbManager(QString db_path, QObject *parent = nullptr) : QObject (parent)
    {        
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(db_path);

        if (db.open() == false)
        {
            qDebug() << "Error: Database connection is failed";
        }
    }

    bool personAdd(Person pers)
    {
        return people.addPerson(pers);
    }

    bool personRemove(Person pers)
    {
        return people.removePerson(pers);
    }

    bool personReplaceById(Person pers)
    {
        return people.replacePersonById(pers);
    }

    QList<Person> *personGetAll() // TODO try to get without a pointer
    {
        return people.getAllPeople();
    }


    bool relationAdd(Relation rel)
    {
        return relations.addRelation(rel);
    }

    bool relationRemove(Relation rel)
    {
        return relations.removeRelation(rel);
    }

    bool relationReplaceById(Relation rel)
    {
        return relations.replaceRelationById(rel);
    }

    QList<Relation> *relationGetAll() // TODO try without pointer
    {
        return relations.getAllRelations();
    }

    bool relationCheckValid(Relation rel)
    {
        // TODO make in people isExist(id)
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

// TODO replace to DbManager
//void test_isValid()
//{
//    // prepare
//    // get first id
//    PeopleStorage people;
//    people.addPerson(Person("Артем", "Оношко", "12.01.1998", "лох", false));

//    QSqlQuery query("SELECT max(id) FROM people");
//    query.next();
//    int pers_id_1 = query.value(0).toInt();

//    // get second id
//    people.addPerson(Person("Артем", "Оношко", "12.01.1998", "лох", false));

//    query.exec("SELECT max(id) FROM people");
//    query.next();
//    int pers_id_2 = query.value(0).toInt();

//    // create relation storage
//    RelationStorage links;

//    // compare
//    // everything is ok
//    QCOMPARE(links.isValid(
//                 Relation(pers_id_1, pers_id_2)), true);

//    // pers_id_2 is out
//    QCOMPARE(links.isValid(
//                 Relation(pers_id_1, pers_id_2 + 1)), false);

//    // pers_id_1 is out
//    QCOMPARE(links.isValid(
//                 Relation(-1, pers_id_2)), false);
//}





















