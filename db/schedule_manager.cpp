#include "db/schedule_manager.h"


ScheduleManager::ScheduleManager(QObject *parent)
    : QObject(parent) {}


ScheduleManager::ScheduleManager(QString scheduleTable,
           QString groupTable,
             QObject *parent)
    : QObject(parent)
{
    touchManager(scheduleTable, groupTable);
}


void ScheduleManager::touchManager(QString scheduleTable,
                  QString groupTable)
{
    touchTable(scheduleTable);
    refsToGroups.touchManager(scheduleTable, groupTable);
}


bool ScheduleManager::saveSchedule(Schedule sch)
{
    if (isRecordExist(sch.id))
        return updateSchedule(sch);
    else
        return addSchedule(sch);
}

bool ScheduleManager::removeSchedule(int id)
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

QList<Schedule> ScheduleManager::getSchedules()
{
    QSqlQuery query("SELECT * FROM " + scheduleTable);

    QList<Schedule> schedules;

    if (!query.lastError().isValid())
    {
        while (query.next())
        {
            QList<QString> scheduleInList;

            for (int i = 0; i < Schedule::getSaveblePattern().count(); i++)
            {
                scheduleInList << query.record().value(i + 1).toString();
            }

            Schedule sch;
            sch.setSavableProperty(scheduleInList);
            sch.id = query.record().value("id").toInt();

            for (int id : refsToGroups.getLinks(sch.id))
            {
                sch.groups << Group(id);
            }

            schedules << sch;
        }
    }
    else
        qWarning() << "ScheduleManager: " << query.lastError().text();

    return schedules;
}


bool ScheduleManager::addSchedule(Schedule sch)
{
    int maxScheduleId = getMaxIdFromTable(scheduleTable);


    if (maxScheduleId >= 0)
    {
        sch.id = maxScheduleId + 1;

        QSqlQuery query;
        query.prepare(QString("INSERT INTO %1 "
                              " (id, title, event, date) "
                              " VALUES  (?, ?, ?, ?) ")
                      .arg(scheduleTable));

        query.addBindValue(sch.id);
        bindValueList(query, sch.getSavebleProperty());

        if (query.exec())
        {
            if (refsToGroups.updateLinks(sch.id, sch.getGroupsIds()))
            {
                return true;
            }
        } else
            qWarning() << query.lastError().text();
    } else
        qWarning() << "addSchedule failed to get maxId";

    return false;
}


bool ScheduleManager::updateSchedule(Schedule sch)
{
    QSqlQuery query;
    query.prepare(" UPDATE " + scheduleTable + " SET   \n" +
                  "     title = (:title),              \n"
                  "     event = (:event),              \n"
                  "     date = (:date)                 \n"
                  " WHERE id = (:id)                   \n");


    query.bindValue(":id", sch.id);
    bindValueList(query, sch.getSavebleProperty());

    if (query.exec())
    {
        if (refsToGroups.updateLinks(sch.id, sch.getGroupsIds()))
        {
            return true;
        }

    } else
        qWarning() << "ScheduleManager::updateSchedule" << query.lastError().text();

    return false;
}

bool ScheduleManager::touchTable(QString schedule)
{
    this->scheduleTable = schedule;

    QSqlQuery query("SELECT name FROM sqlite_master"
                    " WHERE name='" + schedule + "'");

    if (query.next() == false)
    {
        query.exec("CREATE TABLE IF NOT EXISTS " + schedule + " ( \n" +
                   " id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,   \n"
                   " title TEXT NOT NULL,                           \n"
                   " event TEXT NOT NULL,                           \n"
                   " date TEXT NOT NULL                             \n"
                   ")                                                 ");

        if (!query.lastError().isValid())
            qDebug() << "Creating " + schedule + " table inside database";
        else
            return false;
    }
    return true;
}

bool ScheduleManager::isRecordExist(int id)
{
    if (id == 0)
        return false;

    QSqlQuery query(QString("SELECT id FROM %1 "
                       " WHERE id = %2 ")
                    .arg(scheduleTable)
                    .arg(QString::number(id)));

    query.next();

    if (query.record().value(0).toInt() == id)
        return true;

    return false;
}

int ScheduleManager::getMaxIdFromTable(QString table)
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

void ScheduleManager::bindValueList(QSqlQuery &query, QList<QString> list)
{
    for (QString field : list)
    {
        query.addBindValue(field);
    }
}

































