#pragma once

#include <QtTest/QtTest>
#include "db/db_manager.h"

class TestDbManager : public QObject
{
    Q_OBJECT

    QString db_path = "../record/tests/test_res/sport_people.db";
    QString schedule = "schedule";
    QString groups = "groups";
    QString trainers = "trainers";
    QString sportsmen = "sportsmen";

private slots:
    void test_creation()
    {
        // test 1
        QVERIFY(dropDbConnection()); // connection is not

        DbManager * storage = new DbManager(db_path);

        // now it is. check it
        QCOMPARE(QSqlDatabase::database().isValid(), true);

        delete storage;

        // check that connection is not again
        QCOMPARE(QSqlDatabase::database().isValid(), false);


        // test 2
        {
            DbManager db_1;
            db_1.touchDb(db_path);

            // now it is, check
            QCOMPARE(QSqlDatabase::database().isValid(), true);
        }

        // check that connection is not again
        QCOMPARE(QSqlDatabase::database().isValid(), false);
    }


    void test_removingPerson()
    {
        setDefaultState();

        DbManager db(db_path);

        QCOMPARE(db.removeSportsman(1), false);
    }


private:
    bool dropDbConnection()
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
        return !(QSqlDatabase::database().isValid());
    }

    void setDefaultState()
    {
        dropDbData();

        DbManager db(db_path);

        db.saveSportsman(Person({"Артем", "Эдуардович", "Оношко", "12.01.1998", "плавание"}));
        db.saveSportsman(Person({"Олег", "Павлович", "Полушин", "чч.чч.1995", "мошенник"}));

        db.saveTrainer(Person({"Иван", "Владимирович", "Вытовтов", "10.02.1997", "плавание"}));

        Group gr(Group::getPattern());
        gr.trainers_ids << 1;
        gr.sportsmen_ids << 1;
        db.saveGroup(gr);

        Group gr_2({"Группа мошеннег", "мошенник"});
        gr_2.trainers_ids << 1;
        gr_2.sportsmen_ids << 2;
        db.saveGroup(gr_2);
    }

    void dropDbData()
    {
        DbManager db(db_path);

        QSqlQuery query;
        query.exec("DROP TABLE IF EXISTS " + schedule + "_" + groups);
        query.exec("DROP TABLE IF EXISTS " + schedule);
        query.exec("DROP TABLE IF EXISTS " + groups + "_" + sportsmen);
        query.exec("DROP TABLE IF EXISTS " + groups + "_" + trainers);
        query.exec("DROP TABLE IF EXISTS " + groups);
        query.exec("DROP TABLE IF EXISTS " + sportsmen);
        query.exec("DROP TABLE IF EXISTS " + trainers);
    }
};






