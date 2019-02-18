#pragma once

#include <QtSql> // библиотека для работы с бд

#include "people_manager.h" // файл объекта отвечающего за "людей" в бд
#include "group_manager.h" // файл объекта отвечающего за "расписание" в бд

// класс DbManager/БдМенеджер
class DbManager : public QObject
{
    Q_OBJECT    // обязательный макрос для Qt Framework

    PeopleManager *sportsmenManager;
    PeopleManager *trainersManager;
    GroupManager *groupManager;   // указатель на объект отвечающий за "расписание" в бд
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

        sportsmenManager = new PeopleManager("sportsmen", this);
        trainersManager =  new PeopleManager("trainers", this);
//        groupManager = new GroupManager(this);// создание объекта хранения расписаний
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

    Person getPersonPattern()
    {
        return Person(Person::getPattern());
    }

    // метод возвращает из бд всех людей в виде спика List

    // метод возвращает из бд детей в виде спика List
    QList<Person> *getChildren()
    {
        return sportsmenManager->getPeople();
    }


    // метод возвращает из бд тренеров в виде спика List
    QList<Person> *getTrainers()
    {
        return trainersManager->getPeople();
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




















