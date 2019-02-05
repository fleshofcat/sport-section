#pragma once

#include <QtTest/QtTest>
#include <QtSql>

#include "schedule_manager.h"

class TestScheduleStorage : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("../record/tests/test_res/sport_people.db");
        QVERIFY(db.open());
    }

    void test_addSchedule()
    {
        // prepare
        ScheduleManager schedules;
        Schedule sched(1, 2); // schedule don't care about validation

        // run test method
        QVERIFY(schedules.addSchedule(sched));

        // get examine data
        QSqlQuery query;
        query.exec("SELECT * FROM " + tableName + " WHERE "
                   "id = (SELECT MAX(id) FROM " + tableName + ")");
        query.next();

        // compare
        QCOMPARE(sched.trainer_id, query.value("trainer_id").toInt());
        QCOMPARE(sched.child_id, query.value("child_id").toInt());
    }

    void test_removeSchedule()
    {
        // preparing

        // create schedule for delete
        ScheduleManager schedules;
        Schedule sched(1, 2);

        schedules.addSchedule(sched);

        // get id
        QSqlQuery query("SELECT max(id) FROM " + tableName);
        query.next();

        int last_id = query.value(0).toInt();
        qDebug() << "max id = " << last_id;

        sched.id = last_id;

        // examine

        // removeSchedule() is deleting by sched.id
        QVERIFY(schedules.removeSchedule(sched));

        query.exec("SELECT max(id) FROM " + tableName);
        query.next();

        // compare sched.id with last id
        QVERIFY(sched.id != query.value(0).toInt());
    }

    void test_replaceScheduleById()
    {
        // prepare

        // add basic schedule
        ScheduleManager schedules;
        schedules.addSchedule(Schedule(1, 2));

        // make schedule for update
        Schedule sched(3, 4);

        // get new schedule id
        QSqlQuery query("SELECT max(id) FROM " + tableName);
        query.next();
        sched.id = query.value(0).toInt();

        // run test method
        schedules.replaceScheduleById(sched);

        // get examine data
        query.prepare("SELECT * FROM " + tableName + " WHERE id = (:id)");
        query.bindValue(":id", sched.id);
        query.exec();
        query.next();

        // compare
        QCOMPARE(sched.trainer_id, query.value("trainer_id").toInt());
        QCOMPARE(sched.child_id, query.value("child_id").toInt());
    }

    void test_getAllSchedule()
    {
        // prepare

        // clean and refill the schedule table
        QSqlQuery query("DELETE FROM " + tableName);

        ScheduleManager schedules;

        Schedule insertSched_1(1, 2);
        schedules.addSchedule(insertSched_1);

        Schedule insertSched_2(3, 4);
        schedules.addSchedule(insertSched_2);

        // run test method
        QList<Schedule> *scheduleList = schedules.getAllSchedules();

        QVERIFY(compareSchedule(insertSched_1, scheduleList->at(0)));
        QVERIFY(compareSchedule(insertSched_2, scheduleList->at(1)));
    }

    void cleanupTestCase()
    {
        {
            QSqlQuery query("DELETE FROM " + tableName);
            QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
            db.close();
        }
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

private:
    QString tableName = "relations";

    bool compareSchedule(Schedule sched_1, Schedule sched_2)
    {
        return     (sched_1.trainer_id == sched_2.trainer_id)
                && (sched_1.child_id == sched_2.child_id);
    }};



