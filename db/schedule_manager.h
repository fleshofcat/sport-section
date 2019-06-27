#pragma once

#include <QtSql>

#include "common/schedule.h"
#include "db/relations_in_db.h"

class ScheduleManager : public QObject
{
    Q_OBJECT

    QString scheduleTable;
    RelationsInDb refsToGroups;

public:
    ScheduleManager(QObject *parent = nullptr);
    ScheduleManager(QString scheduleTable,
                    QString groupTable,
                    QObject *parent = nullptr);

    void touchManager(QString scheduleTable,
                      QString groupTable);

    bool saveSchedule(Schedule sch);
    bool removeSchedule(int id);
    QList<Schedule> getSchedules();

private:
    bool touchTable(QString schedule);
    bool addSchedule(Schedule sch);
    bool updateSchedule(Schedule sch);
    bool isRecordExist(int id);
    static int getMaxIdFromTable(QString table); 
    static void bindValueList(QSqlQuery &query, QList<QString> list);
};



