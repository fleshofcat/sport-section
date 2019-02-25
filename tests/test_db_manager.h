#pragma once

#include <QtTest/QtTest>
#include "db/db_manager.h"

class TestDbManager : public QObject
{
    Q_OBJECT

private slots:

    // connection setting in constructor
    // setting down in destructor
    void test_creationDbManager()
    {
        if (QSqlDatabase::database().isValid()) // drop db if exist
        {
            {
                QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
                db.close();
            }
            QSqlDatabase::removeDatabase("qt_sql_default_connection");
        }

        // check that connection is not
        QCOMPARE(QSqlDatabase::database().isValid(), false);

        DbManager * storage = new DbManager(
                    "../record/tests/test_res/sport_people.db");

        // now it is. check it
        QCOMPARE(QSqlDatabase::database().isValid(), true);

        delete storage;

        // check that connection is not again
        QCOMPARE(QSqlDatabase::database().isValid(), false);
    }

};






