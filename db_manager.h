#pragma once

#include <QtSql> // библиотека для работы с бд

#include "people_manager.h" // файл объекта отвечающего за "людей" в бд
#include "schedule_manager.h" // файл объекта отвечающего за "расписание" в бд

// класс DbManager/БдМенеджер
class DbManager : public QObject
{
    Q_OBJECT    // обязательный макрос для Qt Framework

    PeopleManager *peopleManager;       // указатель на объект отвечающий за "людей" в бд
    ScheduleManager *scheduleManager;   // указатель на объект отвечающий за "расписание" в бд
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

        peopleManager = new PeopleManager(this);    // создание объекта хранения людей
        scheduleManager = new ScheduleManager(this);// создание объекта хранения расписаний
    }

    // методы для взаимодействия с бд

    // работа с людьми

    // метод добавления человека в бд
    // принимает объект человека и добавляет с помощью менеджера людей
    bool addPerson(Person pers)
    {
        return peopleManager->addPerson(pers);
    }

    // метод удаления человека из бд
    // принимает объект человека и удаляет с помощью менеджера людей
    bool removePerson(Person pers)
    {
        // проверка что человек не состаит в существующих расписаниях
        if (isBelongToSomeSchedule(pers))
        {
            return false; // если состоит, вернуть false
        }
        // если не состоит провести провести удаление
        return peopleManager->removePerson(pers);
    }

    // метод обновления человека в бд по его id
    bool replacePersonById(Person pers)
    {
        return peopleManager->replacePersonById(pers);
    }

    // метод возвращает из бд всех людей в виде спика List
    QList<Person> *getPeople()
    {
        return peopleManager->getAllPeople();
    }


    // работа с расписанием

    // добавление расписания
    bool addSchedule(Schedule sched)
    {
        if (isScheduleValid(sched))
        {
            return scheduleManager->addSchedule(sched);
        }
        return false;
    }

    // удаление расписаний
    bool removeSchedule(Schedule sched)
    {
        return scheduleManager->removeSchedule(sched);
    }

    // обновление расписаний в бд по id
    bool replaceScheduleById(Schedule sched)
    {
        // проверка расписания на актуальность
        if (isScheduleValid(sched))
        {
            return scheduleManager->replaceScheduleById(sched);
        }
        return false;
    }

    // вернуть все расписания
    QList<Schedule> *getSchedule()
    {
        return scheduleManager->getAllSchedules();
    }

    // Деструктор выполняется когда объект уничтожается
    // закрытие соединения с бд
    ~DbManager()
    {
        {
            QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
            db.close();
        }
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

// методы для внутреннего использования внутри этого класса
private:

    // проверка что человек внутри бд существует    // TODO
    bool isPersonExist(int pers_id)
    {
        // взять всех людей из бд
        QList<Person> *people = this->peopleManager->getAllPeople();

        // попытаться найти человека с заданным id в списке людей из бд
        for (Person pers : *people)
        {
            if (pers.id == pers_id)
            {
                return true;
            }
        }
        // если не нашлось то false
        return false;
    }

    // проверка что человек принадлежит к расписанию
    bool isBelongToSomeSchedule(Person pers)
    {
        // взять все расписания из бд
        QList<Schedule> *schedule = this->getSchedule();

        // искать во всех расписаниях ссылки на id этого человека
        for (Schedule sched : *schedule)
        {
            if (sched.child_id == pers.id
                    || sched.trainer_id == pers.id)
            {
                return true;
            }
        }
        return false;
    }

    // проверка что все люди на которых ссылается расписание существует
    bool isScheduleValid(Schedule sched)
    {
        if (isPersonExist(sched.child_id)
                && isPersonExist(sched.trainer_id))
        {
            return true;
        }
        return false;
    }


};




















