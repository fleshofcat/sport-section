#pragma once

#include <QtTest/QtTest>
#include "db/schedule_manager.h"
#include "db/group_manager.h"
#include "db/people_manager.h"

class TestScheduleManager : public QObject
{
    Q_OBJECT

    QString db_path = "../record/tests/test_res/sport_people.db";

    QString schedule = "schedule";
    QString groups = "groups";
    QString trainers = "trainers";
    QString sportsmen = "sportsmen";

private slots:
    void initTestCase()
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(db_path);
        QVERIFY(db.open());
    }

    void test_addSchedule()
    {
        setDefaultState();

        QDate currentDate = QDate::currentDate();

        Schedule sch("title", Schedule::Event::TRAINING,
                     currentDate, "climbing");

        GroupManager gm(groups, trainers, sportsmen);
        auto justForAdd = gm.getGroups().first();

        sch.groups << justForAdd;

        ScheduleManager sm(schedule, groups);
        QVERIFY(sm.saveSchedule(sch));

        currentDate = currentDate.addDays(1);

        sch.setDate(currentDate);
        sch.id = 1;
        QVERIFY(sm.saveSchedule(sch));
        QCOMPARE(sm.getSchedules().first().getDate(), currentDate);
    }

    void test_removeSchedule()
    {
        setDefaultState();

        ScheduleManager sh(schedule, groups);
        QVERIFY(sh.removeSchedule(1));
    }

    void test_getSchedules()
    {
        setDefaultState();

        ScheduleManager sh(schedule, groups);
        QList<Schedule> schs = sh.getSchedules();

        QCOMPARE(schs.first().getEventNumber(), Schedule::Event::COMPETITION);
        QCOMPARE(schs.first().sportType, "смотреть");
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
    void setDefaultState()
    {
        dropDbState();

        GroupManager gm(groups, trainers, sportsmen);
        for (auto group : testGroups())
        {
            gm.saveGroup(group);
        }

        Schedule sch("Тестовое расписание",
                         Schedule::Event::COMPETITION,
                         QDate::currentDate().addDays(-1),
                         "смотреть");

        ScheduleManager sm(schedule, groups);
        sm.saveSchedule(sch);
    }

    void dropDbState()
    {
        QSqlQuery query;    
        query.exec("DROP TABLE IF EXISTS " + schedule + "_" + groups);
        query.exec("DROP TABLE IF EXISTS " + schedule);
        query.exec("DROP TABLE IF EXISTS " + groups + "_" + sportsmen);
        query.exec("DROP TABLE IF EXISTS " + groups + "_" + trainers);
        query.exec("DROP TABLE IF EXISTS " + groups);
        query.exec("DROP TABLE IF EXISTS " + sportsmen);
        query.exec("DROP TABLE IF EXISTS " + trainers);

        // for correct work
        // the tables in db need to be exist
        PeopleManager tm(trainers);
        PeopleManager sm(sportsmen);
        GroupManager gm(groups, trainers, sportsmen);
        ScheduleManager(schedule, groups);
    }

    QList<Group> testGroups()
    {
        Group q({"q", "q"});
        Group gr(Group::getPattern());
        Group gr_2({"Группа мошеннег", "мошенник"});

        return {q, gr, gr_2};
    }
};


























