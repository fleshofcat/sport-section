#pragma once

#include <QtSql> // для работы с бд

#include "common/schedule.h"
#include "db/group_people_relations.h"

class DbSchedule : public QObject
{
    Q_OBJECT

    QString scheduleTable;
    RelationsInDb refsToGroups;

public:
    DbSchedule(QObject *parent = nullptr)
        : QObject(parent) {}


    DbSchedule(QString scheduleTable,
                    QString groupTable,
                 QObject *parent = nullptr)
        : QObject(parent)
    {
        touchManager(scheduleTable, groupTable);
    }


    void touchManager(QString scheduleTable,
                      QString groupTable)
    {
        touchTable(scheduleTable);
        refsToGroups.touchManager(scheduleTable, groupTable);
    }


    bool saveSchedule(Schedule sch)
    {
        if (sch.id < 1)
            return addSchedule(sch);
        else
            return updateSchedule(sch);
    }

    bool removeSchedule(int id)
    {
        if(refsToGroups.removeMainLinks(id))
        {
            QSqlQuery query;
            query.prepare(QString("DELETE FROM %1 WHERE id = (:id)")
                          .arg(scheduleTable));
            query.addBindValue(id);

            return query.exec();
        }

        return false;
    }

    QList<Schedule> getSchedules()
    {
        QSqlQuery query("SELECT * FROM " + scheduleTable);

        QList<Schedule> schedules;

        if (!query.lastError().isValid())
        {
            while (query.next())
            {
                QList<QString> scheduleInList;

                for (int i = 0; i < Schedule::getPattern().count(); i++)
                {
                    scheduleInList << query.record().value(i + 1).toString();
                }

                Schedule sch(scheduleInList);
                sch.id = query.record().value("id").toInt();
                sch.group_ids = refsToGroups.getLinks(sch.id);

                schedules << sch;
            }
        }
        else
            qWarning() << "ScheduleManager: " << query.lastError().text();

        return schedules;
    }

private:
    bool addSchedule(Schedule sch)
    {
        int maxScheduleId = getMaxIdFromTable(scheduleTable);


        if (maxScheduleId >= 0)
        {
            sch.id = maxScheduleId + 1;

            QSqlQuery query;
            query.prepare(QString("INSERT INTO %1 "
                                  " (id, event, date, sport_type) "
                                  " VALUES  (?, ?, ?, ?) ")
                          .arg(scheduleTable));

            query.addBindValue(sch.id);
            bindValueList(query, sch.getInList());

            if (query.exec())
            {
                if (refsToGroups.updateLinks(sch.id, sch.group_ids))
                {
                    return true;
                }
            } else
                qWarning() << query.lastError().text();
        } else
            qWarning() << "addSchedule failed to get maxId";

        return false;
    }


    bool updateSchedule(Schedule sch)
    {
        QSqlQuery query;
        query.prepare(" UPDATE " + scheduleTable + " SET   \n" +
                      "     event = (:event),              \n"
                      "     date = (:date),                \n"
                      "     sport_type = (:sport_type)     \n"
                      " WHERE id = (:id)                   \n");


        query.bindValue(":id", sch.id);
        bindValueList(query, sch.getInList());

        if (query.exec())
        {
            if (refsToGroups.updateLinks(sch.id, sch.group_ids))
            {
                return true;
            }

        } else
            qWarning() << "ScheduleManager::updateSchedule" << query.lastError().text();

        return false;
    }

    bool touchTable(QString schedule)
    {
        this->scheduleTable = schedule;

        QSqlQuery query("SELECT name FROM sqlite_master"
                        " WHERE name='" + schedule + "'");

        if (query.next() == false)
        {
            query.exec("CREATE TABLE IF NOT EXISTS " + schedule + " ( \n" +
                       " id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,   \n"
                       " event TEXT NOT NULL,                           \n"
                       " date TEXT NOT NULL,                            \n"
                       " sport_type TEXT NOT NULL                       \n"
                       ")                                                 ");

            if (!query.lastError().isValid())
                qDebug() << "Creating " + schedule + " table inside database";
            else
                return false;
        }
        return true;
    }

    static int getMaxIdFromTable(QString table) // возвращает -1 при неудаче
    {
        QSqlQuery query("SELECT MAX(id) FROM " + table);

        if (query.lastError().isValid() == false)
        {
            if (query.next())
            {
                return query.record().value(0).toInt();
            }
        }
        else
        {
            qWarning() << query.lastError().text();
        }
        return -1;
    }

    static void bindValueList(QSqlQuery &query, QList<QString> list)
    {
        for (QString field : list)
        {
            query.addBindValue(field);
        }
    }
};



