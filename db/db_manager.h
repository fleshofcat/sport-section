#pragma once

#include "people_manager.h" // файл объекта отвечающего за "людей" в бд
#include "group_manager.h" // файл объекта отвечающего за "расписание" в бд

// класс DbManager/БдМенеджер
class DbManager : public QObject
{
    Q_OBJECT    // обязательный макрос для Qt Framework

    QString groupsTable = "groups";
    QString trainersTable = "trainers";
    QString sportsmenTable = "sportsmen";

    PeopleManager sportsmenManager;
    PeopleManager trainersManager;
    GroupManager groupManager;

public:
    // конструктор, при создании объекта от этого класса выполняется следующий код:
    // здесь создается и подключается база данных по пути
    // заданному в db_path
    // это строковая переменная, значение которой закладывается в объект при его создании
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

    // работа с людьми

    // метод добавления человека в бд
    // принимает объект человека и добавляет с помощью менеджера людей

    bool savePerson(Person pers)
    {
        if (pers.who == Person::Who::TRAINER)
        {
            return trainersManager.savePerson(pers);
        }
        else if (pers.who == Person::Who::SPORTSMAN)
        {
            return sportsmenManager.savePerson(pers);
        }
        return false;
    }

    // метод удаления человека из бд
    // принимает объект человека и удаляет с помощью менеджера людей
    bool removePerson(int id, Person::Who who)
    {
        if (who == Person::Who::TRAINER)
        {
            return trainersManager.removePerson(id);
        }
        else if (who == Person::Who::SPORTSMAN)
        {
            return sportsmenManager.removePerson(id);
        }
        return false;
    }

    // метод возвращает из бд детей в виде спика List
    QList<Person> getSportsmen()
    {
        QList<Person> sportsmen = sportsmenManager.getPeople();

        for (int i = 0; i < sportsmen.count(); i++)
        {
            sportsmen[i].who = Person::Who::SPORTSMAN;
        }

        return sportsmen;
    }


    // метод возвращает из бд тренеров в виде спика List
    QList<Person> getTrainers()
    {
        QList<Person> trainers = trainersManager.getPeople();

        for (int i = 0; i < trainers.count(); i++)
        {
            trainers[i].who = Person::Who::TRAINER;
        }

        return trainers;
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




















