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
        PeopleManager trainersManager(trainers);
        PeopleManager sportsmenManager(sportsmen);

        Group gr(Group({"testGroupName", "testSportType"}));
        gr.trainers << sportsmenManager.getPerson(2);
        gr.sportsmen << sportsmenManager.getPerson(2);

        QVERIFY(grs.addGroup(gr));                         // test full group
        QVERIFY(grs.addGroup(Group({"empty", "group"})));  // test empty group
    }

    void test_updateGroup()
    {
        setDefaultValuesToDb();

        GroupManager grs(groups, trainers, sportsmen);
        PeopleManager trainersManager(trainers);
        PeopleManager sportsmenManager(sportsmen);

        Group basicGroup(Group({"testGroupName", "testSportType"}));
        basicGroup.trainers << sportsmenManager.getPerson(1);
        basicGroup.sportsmen << sportsmenManager.getPerson(1);

        basicGroup.id = grs.getMaxIdFromTable(groups);
        QVERIFY(basicGroup.id > 0);

        QVERIFY(grs.updateGroup(basicGroup));

        Group updatedGroup = grs.getGroup(basicGroup.id);
        QCOMPARE(updatedGroup, basicGroup);
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


        Person artem({"Артем", "Александрович", "Оношко", "12.01.1998", "плавание"});
        Person oleg({"Олег", "Павлович", "Полушин", "чч.чч.1995", "мошенник"});

        PeopleManager sportsmenManager(sportsmen);
        sportsmenManager.savePerson(artem);
        artem.id = 1; // now artem's id = 1

        sportsmenManager.savePerson(oleg);
        oleg.id = 2; //  now oleg's id = 2


        Person ivan({"Иван", "Владимирович", "Вытовтов", "10.02.1997", "плавание"});
        Person vadim({"Вадим", "Александрович", "Сурков", "26.03.1997", "плавание"});


        PeopleManager trainersManager(trainers);
        trainersManager.savePerson(ivan);
        ivan.id = 1; // now ivan's id = 1

        trainersManager.savePerson(vadim);
        vadim.id = 2; // now vadim's id = 2


        Group gr(Group::pattern());
        gr.trainers << ivan;
        gr.sportsmen << artem;

        Group gr_2({"Группа мошеннег", "мошенник"});
        gr_2.trainers << ivan;
        gr_2.sportsmen << oleg;

        GroupManager grs(groups, trainers, sportsmen);
        grs.addGroup(gr);
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





















