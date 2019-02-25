#pragma once

#include <QtSql> // библиотека для работы с бд

#include "people_manager.h" // файл объекта отвечающего за "людей" в бд
#include "group_manager.h" // файл объекта отвечающего за "расписание" в бд

// класс DbManager/БдМенеджер
class DbManager : public QObject
{
    Q_OBJECT    // обязательный макрос для Qt Framework

    QString groupsTable = "groups";
    QString trainersTable = "trainers";
    QString sportsmenTable = "sportsmen";

    PeopleManager *sportsmenManager;
    PeopleManager *trainersManager;
    GroupManager *groupManager;

public:
    // конструктор, при создании объекта от этого класса выполняется следующий код:
    // здесь создается и подключается база данных по пути
    // заданному в db_path
    // это строковая переменная, значение которой закладывается в объект при его создании
    explicit DbManager(QString db_path, QObject *parent = nullptr)
        : QObject (parent)
    {        
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(db_path);

        if (db.open() == false) // проверка что бд открылась успешно
        {
            qDebug() << "Error: Database connection is failed";
            return;
        }

        sportsmenManager = new PeopleManager(sportsmenTable, this);
        trainersManager =  new PeopleManager(trainersTable, this);
        groupManager = new GroupManager(groupsTable, trainersTable, sportsmenTable, this);
    }

    // методы для взаимодействия с бд

    // работа с людьми

    // метод добавления человека в бд
    // принимает объект человека и добавляет с помощью менеджера людей

    bool savePerson(Person pers)
    {
        if (pers.isTrainer)
        {
            return trainersManager->savePerson(pers);
        }
        else
        {
            return sportsmenManager->savePerson(pers);
        }

    }

    // метод удаления человека из бд
    // принимает объект человека и удаляет с помощью менеджера людей
    bool removePerson(int id, bool isTrainer)
    {
        if (isTrainer)
        {
            return trainersManager->removePerson(id);
        }
        else
        {
            return sportsmenManager->removePerson(id);
        }
    }

    // метод возвращает из бд детей в виде спика List
    QList<Person> *getSportsmen()
    {
        QList<Person> *sportsmen = sportsmenManager->getPeople();

        for (int i = 0; i < sportsmen->count(); i++)
        {
            sportsmen[0][i].isTrainer = false;
        }

        return sportsmen;
    }


    // метод возвращает из бд тренеров в виде спика List
    QList<Person> *getTrainers()
    {
        QList<Person> *trainers = trainersManager->getPeople();

        for (int i = 0; i < trainers->count(); i++)
        {
            trainers[0][i].isTrainer = true;
        }

        return trainers;
    }


    bool saveGroup(Group group)
    {
        return groupManager->saveGroup(group);
    }

    bool removeGroup(int id)
    {
        return groupManager->removeGroup(id);
    }

    QList<Group> *getGroups()
    {
        return groupManager->getGroups();
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




















