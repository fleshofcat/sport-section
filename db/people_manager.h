#pragma once

#include <QtSql> // для работы с бд

#include "common/common_objects.h" // общие объекты (Person/Schedule)

// класс PeopleManager/МенеджерЛюдей
//
// отвечает за размещение людей в бд
// и правильную работу с ними
class PeopleManager : public QObject
{
    Q_OBJECT // обязательный макрос

    QString tableName = "people"; // имя таблицы в бд для взаимодействия

public:
    // конструктор - код, исполняющийся при создании объекта
    explicit PeopleManager(QObject *parent = nullptr)
        : QObject(parent)
    {                   // создание таблицы "people" внутри бд, если ее там нет
            QSqlQuery query("SELECT name FROM sqlite_master"
                            " WHERE name='" + tableName + "'");

            query.next();
            if (query.value(0).toString() != tableName)
            {
                query.exec("CREATE TABLE IF NOT EXISTS " + tableName + " (      "
                           " id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,     "
                           " first_name TEXT NOT NULL,                          "
                           " last_name TEXT NOT NULL,                           "
                           " birthday TEXT NOT NULL,                            "
                           " sport_type TEXT NOT NULL,                          "
                           " is_trainer INTEGER )                               ");

                // вывод отладочной информации что таблица в бд была создана
                qDebug() << "Creating " + tableName + " table inside database";
            }
    }


    // метод добавления человека
    bool addPerson(Person person)
    {
        // проверка что человек заполнен
        // если это не так, метод сразу завершается
        if (person.isFull() == false)
            return false;

        // создается и исполняется sql запрос на создания нового человека в бд
        QSqlQuery query;
        query.prepare("INSERT INTO " + tableName + "                            "
        "        (first_name, last_name, birthday, sport_type, is_trainer)      "
        " VALUES (:first_name, :last_name, :birthday, :sport_type, :is_trainer) ");

        query.bindValue(":first_name", person.firstName);
        query.bindValue(":last_name", person.lastName);
        query.bindValue(":birthday", person.birthday);
        query.bindValue(":sport_type", person.sportType);
        query.bindValue(":is_trainer", person.isTrainer);

        bool ret = query.exec();

        // если создание прошло успешно возвращается true
        // если нет то false
        if (!ret)
            qDebug() << "query.exec() in add person failed";

        return ret;
    }


    // метод удаления человека
    bool removePerson(Person person)
    {
        // создается и исполняется sql запрос
        // на удаление человека в бд по id
        QSqlQuery query;
        query.prepare("DELETE FROM " + tableName + " WHERE id = (:id)");
        query.bindValue(":id", person.id);

        bool ret = query.exec();

        if (!ret)
            qDebug() << "query.exec() in remove person failed";

        return ret; // возвращается результат запроса
    }


    // метод обновления человека в бд
    bool replacePersonById(Person person)
    {
        // создается и исполняется sql запрос
        // на обновление человека в бд по id
        QSqlQuery query;
        query.prepare(" UPDATE " + tableName + " SET    "
                      "     first_name = (:first_name), "
                      "     last_name = (:last_name),   "
                      "     birthday = (:birthday),     "
                      "     sport_type = (:sport_type), "
                      "     is_trainer = (:is_trainer)  "
                      " WHERE id = (:id)                ");


        query.bindValue(":id", person.id);
        query.bindValue(":first_name", person.firstName);
        query.bindValue(":last_name", person.lastName);
        query.bindValue(":birthday", person.birthday);
        query.bindValue(":sport_type", person.sportType);
        query.bindValue(":is_trainer", person.isTrainer);

        bool ret = query.exec();

        if (!ret)
            qDebug() << "query.exec() in replace person failed";

        return ret; // возвращается результат операции
    }


    // метод получения человека
    Person *getPerson(int id)
    {
        // sql запрос здесь происходит
        // с помощью модуля QSqlTableModel
        QSqlTableModel sqlModel;
        sqlModel.setTable("people");
        sqlModel.setFilter("id = " + QString::number(id));
        sqlModel.select();

        Person *pers = new Person;

        pers->id = sqlModel.record(0).value("id").toInt();
        pers->firstName = sqlModel.record(0).value("first_name").toString();
        pers->lastName = sqlModel.record(0).value("last_name").toString();
        pers->birthday = sqlModel.record(0).value("birthday").toString();
        pers->sportType = sqlModel.record(0).value("sport_type").toString();
        pers->isTrainer = sqlModel.record(0).value("is_trainer").toBool();

        return pers;
    }


    // метод получения всех людей в бд
    QList<Person> *getAllPeople()
    {
        // создается и исполняется sql запрос на получения всех людей
        QSqlQuery query;
        query.prepare("SELECT * FROM " + tableName);

        QList<Person> *peopleList = new QList<Person>();

        if (query.exec())        // если запрос прошел удачно
        {
            while (query.next()) // каждый человек извлекается из бд
            {                    // и "упаковывается" в список людей

                Person *person = new Person();

                person->id = query.value("id").toInt();
                person->firstName = query.value("first_name").toString();
                person->lastName = query.value("last_name").toString();
                person->birthday = query.value("birthday").toString();
                person->sportType = query.value("sport_type").toString();
                person->isTrainer = query.value("is_trainer").toBool();

                *peopleList << *person;
            }
            return peopleList;  // возвращается список людей
        }

        qDebug() << "query.exec() in getAllPeople failed";

        // если запрос прошел не удачно, возвраается пустой указатель
        return nullptr;
    }
};





