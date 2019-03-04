#pragma once

#include <QtTest/QtTest>
#include <QtSql>

#include "db/group_manager.h"
#include "db/people_manager.h"

class TestGroupManager : public QObject
{
    Q_OBJECT

    QString groups = "groups";
    QString trainers = "trainers";
    QString sportsmen = "sportsmen";

private slots:
    void initTestCase()
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("../record/tests/test_res/sport_people.db");
        QVERIFY(db.open());
    }


    void test_addGroup()
    {
        setDefaultValuesToDb();

        GroupManager grs(groups, trainers, sportsmen);

        Group gr(Group({"testGroupName", "testSportType"}));
        gr.trainers_ids << 1;
        gr.sportsmen_ids << 1;

        QVERIFY(grs.addGroup(gr));
        QVERIFY(grs.addGroup(Group({"empty", "group"})));
    }

    void test_updateGroup()
    {
        setDefaultValuesToDb();

        GroupManager grs(groups, trainers, sportsmen);

        Group gr(Group({"testGroupName", "testSportType"}));
        gr.trainers_ids << 1;
        gr.sportsmen_ids << 1;

        gr.id = grs.getMaxIdFromTable(groups);
        QVERIFY(gr.id > 0);

        QVERIFY(grs.updateGroup(gr));
    }

    void test_removeGroup()
    {
        setDefaultValuesToDb();
        GroupManager grs(groups, trainers, sportsmen);

        int group_id = grs.getMaxIdFromTable(groups);
        QVERIFY(group_id > 0);

        QVERIFY(grs.removeGroup(group_id));

        QCOMPARE(grs.getMaxIdFromTable(groups), 1);
    }


    void test_getGroups()
    {
        setDefaultValuesToDb();

        GroupManager grs(groups, trainers, sportsmen);

        auto groups = grs.getGroups();

        Group swimming = groups.at(0);
        Group froad = groups.at(1);

        QCOMPARE(groups.count(), 2);
        QCOMPARE(swimming.id, 1);
        QCOMPARE(froad.getInList().at(1), "мошенник");
    }


    void cleanUpTestCase()
    {
        {
            QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
            db.close();
        }
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }


private:
   void setDefaultValuesToDb()
    {
       // delete order is important
       dropDbState();

       PeopleManager sportsmenManager(sportsmen);

       sportsmenManager.savePerson(
            Person({"Артем", "Эдуардович", "Оношко", "12.01.1998", "плавание"}));

       sportsmenManager.savePerson(
            Person({"Олег", "Павлович", "Полушин", "чч.чч.1995", "мошенник"}));

       PeopleManager trainersManager(trainers);
       trainersManager.savePerson(
            Person({"Иван", "Владимирович", "Вытовтов", "10.02.1997", "плавание"}));


       GroupManager grs(groups, trainers, sportsmen);

       Group gr(Group::pattern());
       gr.trainers_ids << 1;
       gr.sportsmen_ids << 1;
       grs.addGroup(gr);

       Group gr_2({"Группа мошеннег", "мошенник"});
       gr_2.trainers_ids << 1;
       gr_2.sportsmen_ids << 2;
       grs.addGroup(gr_2);

    }

   void dropDbState()
   {
       QSqlQuery query;
       query.exec("DROP TABLE IF EXISTS " + groups + "_" + sportsmen);
       query.exec("DROP TABLE IF EXISTS " + groups + "_" + trainers);
       query.exec("DROP TABLE IF EXISTS " + groups);
       query.exec("DROP TABLE IF EXISTS " + sportsmen);
       query.exec("DROP TABLE IF EXISTS " + trainers);
   }
};





















