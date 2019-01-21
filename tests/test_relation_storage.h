#pragma once

#include <QtTest/QtTest>
#include <QtSql>

#include "relation_storage.h"

class TestRelationStorage : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("../record/tests/test_res/sport_people.db");
        QVERIFY(db.open());
    }

    void test_isValid()
    {
        // TODO!
        // 1 rename editPerson(int, Person) to replacePersonById(Person)
        // 2 mv database init and close inside PeopleStorage (sacriface spid for crossplatform)
        // 3 (overload)?
        // to google normal directory map in qt creator
    }


    void cleanupTestCase()
    {
        {
            QSqlQuery query("DELETE FROM people");
            QSqlDatabase db = QSqlDatabase::database("QSQLITE");
            db.close();
        }
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }
};
