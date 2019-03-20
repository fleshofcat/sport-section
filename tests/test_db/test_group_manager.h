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

        Group fullGroup(Group("fullGroupName"));
        fullGroup.trainers << trainersManager.getPerson(2);
        fullGroup.sportsmen << sportsmenManager.getPerson(3);

        Group wrongSportsmanGroup(Group("testGroupName"));
        wrongSportsmanGroup.trainers << trainersManager.getPerson(2);
        wrongSportsmanGroup.sportsmen << sportsmenManager.getPerson(2);


        QVERIFY(grs.addGroup(fullGroup));                   // test full group
        QCOMPARE(grs.addGroup(wrongSportsmanGroup), false); // test group with repitable sportsman
        QVERIFY(grs.addGroup(Group("empty group")));        // test empty group
    }

    void test_updateGroup()
    {
        setDefaultValuesToDb();

        GroupManager grs(groups, trainers, sportsmen);
        PeopleManager trainersManager(trainers);
        PeopleManager sportsmenManager(sportsmen);

        Group groupForUpdate(Group("for update"));
        groupForUpdate.trainers << sportsmenManager.getPerson(1);
        groupForUpdate.sportsmen << sportsmenManager.getPerson(3);

        groupForUpdate.id = grs.getMaxIdFromTable(groups);
        QVERIFY(groupForUpdate.id > 0);

        QVERIFY(grs.updateGroup(groupForUpdate));

        Group updatedGroup = grs.getGroup(groupForUpdate.id);
        QCOMPARE(updatedGroup.getSaveableProperty(), groupForUpdate.getSaveableProperty());
        QCOMPARE(updatedGroup.getSportsmenIds(), groupForUpdate.getSportsmenIds());
        QCOMPARE(updatedGroup.getTrainersIds(), groupForUpdate.getTrainersIds());
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
        Group fraud = groups.at(1);

        QCOMPARE(groups.count(), 2);
        QCOMPARE(swimming.id, 1);
        QCOMPARE(fraud.getInList().first(), "Группа мошеннег");
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


        Person artem;
        artem.setEditableList({"Артем", "Александрович", "Оношко", "12.01.1998", "плавание", "+7xxx"});
        Person oleg;
        oleg.setEditableList({"Олег", "Павлович", "Полушин", "01.01.1995", "мошенник", "+7xxx"});
        Person mihail;
        mihail.setEditableList({"Михаил", "Владимирович", "Черников", "18.09.1997", "макбук", "+7xxx"});

        PeopleManager sportsmenManager(sportsmen);
        sportsmenManager.savePerson(artem);
        artem.id = 1; // now artem's id = 1

        sportsmenManager.savePerson(oleg);
        oleg.id = 2; //  now oleg's id = 2

        sportsmenManager.savePerson(mihail);
        mihail.id = 3; //  now mihail's id = 3


        Person ivan;
        ivan.setEditableList({"Иван", "Владимирович", "Вытовтов", "10.02.1997", "мошенник", "+7xxx"});
        Person vadim;
        vadim.setEditableList({"Вадим", "Александрович", "Сурков", "26.03.1997", "плавание", "+7xxx"});


        PeopleManager trainersManager(trainers);
        trainersManager.savePerson(ivan);
        ivan.id = 1; // now ivan's id = 1

        trainersManager.savePerson(vadim);
        vadim.id = 2; // now vadim's id = 2


        Group gr(Group::getPattern().first());
        gr.trainers << ivan;
        gr.sportsmen << artem;

        Group gr_2("Группа мошеннег");
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





















