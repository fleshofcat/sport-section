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
    QString closedScheduleTable = "closed_schedule";

    PeopleManager sportsmenManager;
    PeopleManager trainersManager;
    GroupManager groupManager;
    DbSchedule scheduleManager;
    DbSchedule closedScheduleManager;

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
                closedScheduleManager.touchManager(closedScheduleTable, groupsTable);
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
        QList<Group> groups = groupManager.getGroups();

        for (int g = 0; g < groups.count(); g++)
        {
            QList<Person> trainersIds = groups[g].trainers;
            QList<Person> sportsmenIds = groups[g].sportsmen;
            groups[g].trainers = {};
            groups[g].sportsmen = {};

            for (Person trainerId : trainersIds)
            {
                groups[g].trainers << trainersManager.getPerson(trainerId.id);
            }

            for (Person sportsmenId : sportsmenIds)
            {
                groups[g].sportsmen << sportsmenManager.getPerson(sportsmenId.id);
            }
        }

        return groups;
    }

    Group getGroup(int id)
    {
        Group group = groupManager.getGroup(id);

        QList<Person> trainersIds = group.trainers;
        QList<Person> sportsmenIds = group.sportsmen;
        group.trainers = {};
        group.sportsmen = {};

        for (Person trainerId : trainersIds)
        {
            group.trainers << trainersManager.getPerson(trainerId.id);
        }

        for (Person sportsmenId : sportsmenIds)
        {
            group.sportsmen << sportsmenManager.getPerson(sportsmenId.id);
        }

        return group;
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
        QList<Schedule> schedules = scheduleManager.getSchedules();

        for (int s = 0; s < schedules.count(); s++)
        {
            QList<Group> groupsIds = schedules[s].groups;
            schedules[s].groups = {};

            for (Group groupId : groupsIds)
            {
                schedules[s].groups << getGroup(groupId.id);
            }
        }

        return schedules;
    }

    bool saveClosedSchedule(Schedule sch)
    {
        return closedScheduleManager.saveSchedule(sch);
    }

    bool removeClosedSchedule(int id)
    {
        return closedScheduleManager.removeSchedule(id);
    }

    QList<Schedule> getClosedSchedule()
    {
        QList<Schedule> closedSchedules = closedScheduleManager.getSchedules();

        for (int s = 0; s < closedSchedules.count(); s++)
        {
            QList<Group> groupsIds = closedSchedules[s].groups;
            closedSchedules[s].groups = {};

            for (Group groupId : groupsIds)
            {
                closedSchedules[s].groups << getGroup(groupId.id);
            }
        }

        return closedSchedules;
    }

    bool hardSaveGroup(Group group)
    {
        for (Person sportsmen : group.sportsmen)
        {
            if(saveSportsman(sportsmen) == false)
                return false;
        }

        for (Person trainer : group.trainers)
        {
            if (saveTrainer(trainer) == false)
            {
                return false;
            }
        }

        return groupManager.saveGroup(group);
    }

    bool hardSaveSchedule(Schedule schedule)
    {
        for (Group group : schedule.groups)
        {
            if (hardSaveGroup(group) == false)
                return false;
        }

        return scheduleManager.saveSchedule(schedule);
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




















