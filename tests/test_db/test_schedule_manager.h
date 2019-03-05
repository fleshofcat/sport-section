#pragma once

#include <QtTest/QtTest>
#include "db/db_manager.h"
#include "db/schedule_manager.h"

class TestScheduleManager : public QObject
{
    Q_OBJECT

    QString db_path = "../record/tests/test_res/test_schedule.db";

    QString schedule = "schedule";
    QString groups = "groups";
    QString trainers = "trainers";
    QString sportsmen = "sportsmen";

//    DbManager *db = new DbManager(db_path, this);

private slots:
    void test_addSchedule()
    {
        setDefaultState();
        DbManager db(db_path);

        Schedule sch(Schedule::Event::TRAINING, "10.02.1997", "climbing");
        sch.groups << Group(1);

        DbSchedule sm(schedule, groups);

        QVERIFY(sm.saveSchedule(sch));

        sch.date = "new date";
        sch.id = 1;
        QVERIFY(sm.saveSchedule(sch));
        QCOMPARE(sm.getSchedules().at(0).date, "new date");
    }

    void test_removeSchedule()
    {
        setDefaultState();
        DbManager db(db_path);

        DbSchedule sh(schedule, groups);
        QVERIFY(sh.removeSchedule(1));
    }

    void test_getSchedules()
    {
        setDefaultState();
        DbManager db(db_path);

        DbSchedule sh(schedule, groups);
        QList<Schedule> schs = sh.getSchedules();

        QCOMPARE(schs.at(0).event, Schedule::Event::COMPETITION);
        QCOMPARE(schs.at(0).sportType, "смотреть");
    }


private:
    void setDefaultState()
    {
        dropDbState();

        DbManager db(db_path);
        for (auto group : testGroups())
        {
            db.saveGroup(group);
        }

        DbSchedule s(schedule, groups);
        s.saveSchedule(Schedule({Schedule::Event::COMPETITION, "пондельник, 18:30", "смотреть"}));
    }

    void dropDbState()
    {
        DbManager d(db_path);

        QSqlQuery query;    
        query.exec("DROP TABLE IF EXISTS " + schedule + "_" + groups);
        query.exec("DROP TABLE IF EXISTS " + schedule);
        query.exec("DROP TABLE IF EXISTS " + groups + "_" + sportsmen);
        query.exec("DROP TABLE IF EXISTS " + groups + "_" + trainers);
        query.exec("DROP TABLE IF EXISTS " + groups);
        query.exec("DROP TABLE IF EXISTS " + sportsmen);
        query.exec("DROP TABLE IF EXISTS " + trainers);
    }

    QList<Group> testGroups()
    {
        Group q({"q", "q"});
        Group gr(Group::pattern());
        Group gr_2({"Группа мошеннег", "мошенник"});

        return {q, gr, gr_2};
    }
};


























