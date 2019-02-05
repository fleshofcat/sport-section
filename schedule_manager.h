#pragma once

#include <QtSql>

#include "common_objects.h"

class ScheduleManager : public QObject
{
    Q_OBJECT
    QString tableName = "relations";

public:
    explicit ScheduleManager(QObject *parent = nullptr)
        : QObject(parent)
    {               // create relation table in db if not exist // TODO
        QSqlQuery query("SELECT name FROM sqlite_master"
                        " WHERE name='" + tableName + "'");

        query.next();
        if (query.value(0).toString() != tableName)
        {
            query.exec("CREATE TABLE IF NOT EXISTS " + tableName + "        "
                       " ( id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,   "
                       " trainer_id INTEGER NOT NULL,                       "
                       " child_id INTEGER NOT NULL)                         ");

            qDebug() << "Creating " + tableName + " table inside database";
        }
    }


    bool addSchedule(Schedule sched)
    {
        if (sched.isFull() == false)
            return false;

        QSqlQuery query;
        query.prepare("INSERT INTO " + tableName +
                      "         (trainer_id, child_id)"
                      " VALUES  (:trainer_id, :child_id)");

        query.bindValue(":trainer_id", sched.trainer_id);
        query.bindValue(":child_id", sched.child_id);

        bool ret = query.exec();

        if (!ret)
            qDebug() << "query.exec() in addSchedule failed";

        return ret;
    }


    bool removeSchedule(Schedule sched)
    {
        QSqlQuery query;

        query.prepare("DELETE FROM " + tableName + " WHERE id = (:id)");
        query.bindValue(":id", sched.id);

        bool ret = query.exec();

        if (!ret)
            qDebug() << "query.exec() in removeSchedule failed";

        return ret;
    }


    bool replaceScheduleById(Schedule sched)
    {
        QSqlQuery query;
        query.prepare(" UPDATE " + tableName + " SET    "
                      "     trainer_id = (:trainer_id), "
                      "     child_id = (:child_id)      "
                      " WHERE id = (:id)                ");


        query.bindValue(":id", sched.id);
        query.bindValue(":trainer_id", sched.trainer_id);
        query.bindValue(":child_id", sched.child_id);

        bool ret = query.exec();

        if (!ret)
            qDebug() << "query.exec() in replaceScheduleById failed";

        return ret;
    }


    QList<Schedule> *getAllSchedules()
    {
        QSqlQuery query;
        query.prepare("SELECT * FROM " + tableName);

        QList<Schedule> *scheduleList = new QList<Schedule>;

        if (query.exec())
        {
            while (query.next())
            {

                Schedule *sched = new Schedule();

                sched->id = query.value("id").toInt();
                sched->trainer_id = query.value("trainer_id").toInt();
                sched->child_id = query.value("child_id").toInt();

                *scheduleList << *sched;
            }
            return scheduleList;
        }

        qDebug() << "query.exec() in getAllSchedules failed";
        return nullptr;
    }
};





