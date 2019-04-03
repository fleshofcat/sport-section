#pragma once

#include "people_manager.h"
#include "group_manager.h"
#include "schedule_manager.h"

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
    ScheduleManager scheduleManager;
    ScheduleManager closedScheduleManager;

public:
    explicit DbManager(QObject *parent = nullptr) ;
    explicit DbManager(QString db_path, QObject *parent = nullptr);

    void touchDb(QString db_path);

    // методы для взаимодействия с данными в бд

    bool saveSportsman(Person sportsman);
    bool saveTrainer(Person trainer);
    bool saveGroup(Group group);
    bool saveSchedule(Schedule sch);

    bool hardSaveGroup(Group group);
    bool hardSaveClosedSchedule(Schedule schedule);

    bool removeSportsman(int id);
    bool removeTrainer(int id);
    bool removeGroup(int id);
    bool removeSchedule(int id);
    bool removeClosedSchedule(int id);

    QList<Person> getSportsmen();
    QList<Person> getTrainers();
    Person getSportsman(int id);
    Person getTrainer(int id);
    QList<Group> getGroups();
    Group getGroup(int id);
    QList<Schedule> getSchedules();
    QList<Schedule> getClosedSchedule();

    ~DbManager();
};




















