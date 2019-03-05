#pragma once

#include "people_manager.h" // файл объекта отвечающего за "людей" в бд
#include "group_manager.h" // файл объекта отвечающего за "расписание" в бд
#include "schedule_manager.h"

// класс DbManager/БдМенеджер
class DbManager : public QObject
{
    Q_OBJECT

    QString groupsTable = "groups";
    QString trainersTable = "trainers";
    QString sportsmenTable = "sportsmen";
    QString scheduleTable = "schedule";

    PeopleManager sportsmenManager;
    PeopleManager trainersManager;
    GroupManager groupManager;
    DbSchedule scheduleManager;

public:
    DbManager(QObject *parent = nullptr) : QObject(parent) {}

    DbManager(QString db_path, QObject *parent = nullptr)
        : QObject (parent)
    {
        touchDb(db_path);
    }

    void touchDb(QString db_path)
    {
        if (!QSqlDatabase::database().isOpen())
        {
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName(db_path);

            if (db.open())
            {
                QSqlQuery query("PRAGMA foreign_keys=on");

                sportsmenManager.touchManager(sportsmenTable);
                trainersManager.touchManager(trainersTable);
                groupManager.touchManager(groupsTable, trainersTable, sportsmenTable);
                scheduleManager.touchManager(scheduleTable, groupsTable);
            }
            else
            {
                qWarning() << "db connection wasn't opened. " << db.lastError().text();
            }
        }
    }

    // методы для взаимодействия с бд

    bool saveSportsman(Person sportsman)
    {
        return sportsmenManager.savePerson(sportsman);
    }


    bool saveTrainer(Person trainer)
    {
        return trainersManager.savePerson(trainer);
    }


    bool removeSportsman(int id)
    {
        return sportsmenManager.removePerson(id);
    }


    bool removeTrainer(int id)
    {
        return trainersManager.removePerson(id);
    }


    QList<Person> getSportsmen()
    {
        return sportsmenManager.getPeople();
    }


    QList<Person> getTrainers()
    {
        return trainersManager.getPeople();
    }

    Person getSportsman(int id)
    {
        return sportsmenManager.getPerson(id);
    }

    Person getTrainer(int id)
    {
        return trainersManager.getPerson(id);
    }

    bool saveGroup(Group group)
    {
        return groupManager.saveGroup(group);
    }


    bool removeGroup(int id)
    {
        return groupManager.removeGroup(id);
    }


    QList<Group> getGroups()
    {
        return groupManager.getGroups();
    }

    bool saveSchedule(Schedule sch)
    {
        return scheduleManager.saveSchedule(sch);
    }

    bool removeSchedule(int id)
    {
        return scheduleManager.removeSchedule(id);
    }

    QList<Schedule> getSchedules()
    {
        return scheduleManager.getSchedules();
    }

    ~DbManager()
    {
        {
            QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
            db.close();
        }
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

};




















