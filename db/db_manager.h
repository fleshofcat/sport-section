#pragma once

#include "people_manager.h" // файл объекта отвечающего за "людей" в бд
#include "group_manager.h" // файл объекта отвечающего за "расписание" в бд

// класс DbManager/БдМенеджер
class DbManager : public QObject
{
    Q_OBJECT

    QString groupsTable = "groups";
    QString trainersTable = "trainers";
    QString sportsmenTable = "sportsmen";

    PeopleManager sportsmenManager;
    PeopleManager trainersManager;
    GroupManager groupManager;

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


    ~DbManager()
    {
        {
            QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
            db.close();
        }
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

};




















