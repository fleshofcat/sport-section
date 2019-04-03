#include "db/db_manager.h"


DbManager::DbManager(QObject *parent) : QObject(parent) {}
DbManager::DbManager(QString db_path, QObject *parent)
    : QObject (parent)
{
    touchDb(db_path);
}

void DbManager::touchDb(QString db_path)
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

bool DbManager::saveSportsman(Person sportsman)
{
    return sportsmenManager.savePerson(sportsman);
}


bool DbManager::saveTrainer(Person trainer)
{
    return trainersManager.savePerson(trainer);
}


bool DbManager::removeSportsman(int id)
{
    return sportsmenManager.removePerson(id);
}


bool DbManager::removeTrainer(int id)
{
    return trainersManager.removePerson(id);
}


QList<Person> DbManager::getSportsmen()
{
    return sportsmenManager.getPeople();
}


QList<Person> DbManager::getTrainers()
{
    return trainersManager.getPeople();
}

Person DbManager::getSportsman(int id)
{
    return sportsmenManager.getPerson(id);
}

Person DbManager::getTrainer(int id)
{
    return trainersManager.getPerson(id);
}

bool DbManager::saveGroup(Group group)
{
    return groupManager.saveGroup(group);
}


bool DbManager::removeGroup(int id)
{
    if (!groupManager.removeGroup(id))
    {
        for (auto sch : closedScheduleManager.getSchedules())
        {
            if (sch.getGroupsIds().contains(id))
            {
                closedScheduleManager.removeSchedule(sch.id);
            }
        }
        return groupManager.removeGroup(id);
    }
    return true;
}


QList<Group> DbManager::getGroups()
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

Group DbManager::getGroup(int id)
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

bool DbManager::saveSchedule(Schedule sch)
{
    return scheduleManager.saveSchedule(sch);
}

bool DbManager::removeSchedule(int id)
{
    return scheduleManager.removeSchedule(id);
}

QList<Schedule> DbManager::getSchedules()
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

bool DbManager::removeClosedSchedule(int id)
{
    return closedScheduleManager.removeSchedule(id);
}

QList<Schedule> DbManager::getClosedSchedule()
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

bool DbManager::hardSaveGroup(Group group)
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

bool DbManager::hardSaveClosedSchedule(Schedule schedule)
{
    for (Group group : schedule.groups)
    {
        if (hardSaveGroup(group) == false)
            return false;
    }

    return closedScheduleManager.saveSchedule(schedule);
}

DbManager::~DbManager()
{
    {
        QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
        db.close();
    }
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
}










