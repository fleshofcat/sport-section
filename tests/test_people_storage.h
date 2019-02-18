#pragma once

#include <QtTest/QtTest>
#include "db/people_manager.h"

class TestPeopleStorage : public QObject
{
    Q_OBJECT

private slots:
/*    void initTestCase()
    {
        // prepare tests to manual check
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("../record/tests/test_res/sport_people.db");
        QVERIFY(db.open());
    }

    void test_savePerson_add()
    {
        // prepare
        PeopleManager people("people");
        Person pers("Артем", "Оношко", "12.01.1998", "лох", false);

        // run test method
        QVERIFY(people.savePerson(pers));

        // get examine data
        QSqlQuery query;
        query.exec("SELECT * FROM people WHERE id = (SELECT MAX(id) FROM people)");
        query.next();

        // compare
        QCOMPARE(pers.firstName, query.value("first_name").toString());
        QCOMPARE(pers.lastName, query.value("last_name").toString());
        QCOMPARE(pers.birthday, query.value("birthday").toString());
        QCOMPARE(pers.sportType, query.value("sport_type").toString());
        QCOMPARE(pers.isTrainer, query.value("is_trainer").toBool());
    }

    void test_removePerson()
    {
        // preparing

        // create person for delete
        PeopleManager people("people");
        Person pers("person", "for", "delete", "in test", false);

        people.savePerson(pers);

        // get last id before removing
        QSqlQuery query("SELECT max(id) FROM people");
        query.next();

        int last_id = query.value(0).toInt();
        pers.id = last_id;

        // run test method
        // is working by pers.id
        QVERIFY(people.removePerson(pers));

        // get new last id
        query.exec("SELECT max(id) FROM people");
        query.next();

        // compare pers.id with last id
        QVERIFY(pers.id != query.value(0).toInt());
    }

    void test_savePerson_update()
    {
        // prepare

        // add basic person
        PeopleManager people("people");
        people.savePerson(Person("record", "for edit", "xx.xx.xxxx", "coach", false));

        // make person for update
        Person pers("edited", "record", "00.00.0000", "coach", true);

        // get new person id
        QSqlQuery query("SELECT max(id) FROM people");
        query.next();
        pers.id = query.value(0).toInt();

        // run test method
        people.savePerson(pers);

        // get examine data
        query.prepare("SELECT * FROM people WHERE id = (:id)");
        query.bindValue(":id", pers.id);
        query.exec();
        query.next();

        // compare
        QCOMPARE(pers.firstName, query.value("first_name").toString());
        QCOMPARE(pers.lastName, query.value("last_name").toString());
        QCOMPARE(pers.birthday, query.value("birthday").toString());
        QCOMPARE(pers.sportType, query.value("sport_type").toString());
        QCOMPARE(pers.isTrainer, query.value("is_trainer").toBool());
    }

    void test_getAllPeople()
    {
        // prepare

        // clean and refill the people table
        QSqlQuery query("DELETE FROM people");

        PeopleManager people("people");

        Person insertPers_1("names", "for get", "at all", "xx.xx.xxxx", true);
        people.savePerson(insertPers_1);

        Person insertPers_2("Артем", "Оношко", "лох", "xx.xx.xxxx", false);
        people.savePerson(insertPers_2);

        // run test method
        QList<Person> *peopleList = people.getAllPeople();

        QVERIFY(comparePersons(insertPers_1, peopleList->at(0)));
        QVERIFY(comparePersons(insertPers_2, peopleList->at(1)));
    }

    void cleanupTestCase()
    {
        {
            QSqlQuery query("DELETE FROM people");
            QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
            db.close();
        }
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

private:
    bool comparePersons(Person pers_1, Person pers_2)
    {
        return     (pers_1.firstName == pers_2.firstName)
                && (pers_1.lastName == pers_2.lastName)
                && (pers_1.birthday == pers_2.birthday)
                && (pers_1.sportType == pers_2.sportType)
                && (pers_1.isTrainer == pers_2.isTrainer);
    }

    */
};


