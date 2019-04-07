#pragma once

#include <QtTest/QtTest>
#include "db/db_manager.h"

class TestDbManager : public QObject
{
    Q_OBJECT

    QString db_path = "../sport-section/tests/test_res/sport_people.db";
    QString schedule = "schedule";
    QString groups = "groups";
    QString trainers = "trainers";
    QString sportsmen = "sportsmen";

private slots:
    void test_creation()
    {
        // test 1 // create by pointer
        QVERIFY(dropDbConnection()); // connection is not

        DbManager * storage = new DbManager(db_path);

        // now it is. check it
        QCOMPARE(QSqlDatabase::database().isValid(), true);

        delete storage;

        // check that connection is not again
        QCOMPARE(QSqlDatabase::database().isValid(), false);


        // test 2 // create by object
        {
            DbManager db_1;
            db_1.touchDb(db_path);

            // now it is, check
            QCOMPARE(QSqlDatabase::database().isValid(), true);
        }

        // check that connection is not again
        QCOMPARE(QSqlDatabase::database().isValid(), false);
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
/*
    void setDefaultState()
    {
        dropDbData();

        DbManager db(db_path);

        Person artem({"Артем", "Александрович", "Оношко", "12.01.1998", "плавание"});
        Person oleg({"Олег", "Павлович", "Полушин", "чч.чч.1995", "мошенник"});

        db.saveSportsman(artem);
        artem.id = 1; // now artem's id = 1
        db.saveSportsman(oleg);
        oleg.id = 2; // now oleg's id = 1

        Person ivan({"Иван", "Владимирович", "Вытовтов", "10.02.1997", "плавание"});
        Person vadim({"Вадим", "Александрович", "Сурков", "26.03.1997", "плавание"});

        db.saveTrainer(ivan);
        ivan.id = 2; // now ivan's id = 1
        db.saveTrainer(vadim);
        vadim.id = 2; // now vadim's id = 1

        Group gr(Group::pattern());
        gr.trainers << ivan;
        gr.sportsmen << artem;
        db.saveGroup(gr);

        Group gr_2({"Группа мошеннег", "мошенник"});
        gr_2.trainers << ivan;
        gr_2.sportsmen << oleg;
        db.saveGroup(gr_2);
    }
*/

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







