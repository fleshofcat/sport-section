#pragma once

#include <QtSql>

#include "common/person.h"

// класс PeopleManager/МенеджерЛюдей
//
// отвечает за размещение людей в бд
// и правильную работу с ними
class PeopleManager : public QObject
{
    Q_OBJECT

    QString tableName;

public:
    PeopleManager(QObject *parent = nullptr)
        : QObject(parent) {}

    PeopleManager(QString tableName,
                  QObject *parent = nullptr)
        : QObject(parent)
    {
        touchManager(tableName);
    }


    void touchManager(QString table)
    {
        this->tableName = table;

        QSqlQuery query("SELECT name FROM sqlite_master"
                            " WHERE name='" + table + "'");

        if (query.next() == false)
        {
            query.exec( "CREATE TABLE IF NOT EXISTS " + table + " (        \n" +
                        " id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,     \n"
                        " first_name TEXT NOT NULL,                        \n"
                        " second_name TEXT,                                \n"
                        " last_name TEXT NOT NULL,                         \n"
                        " birthday TEXT NOT NULL,                          \n"
                        " sport_type TEXT NOT NULL                         \n"
                        " )                                                \n");

            // вывод отладочной информации что таблица в бд была создана
            if (query.lastError().isValid() == false)
                qDebug() << "Creating " + table + " table inside database";
        }
    }


    bool savePerson(Person pers)
    {
        return pers.id <= 0 ? addPerson(pers) : updatePerson(pers);
    }


    // метод удаления человека
    bool removePerson(int id)
    {
        // создается и исполняется sql запрос
        // на удаление человека в бд по id

        QSqlQuery query;
        query.prepare("DELETE FROM " + tableName + " WHERE id = (:id)");
        query.bindValue(":id", id);

        return query.exec(); // возвращается результат запроса
    }


    QList<Person> getPeople()
    {
        QSqlTableModel sqlModel;
        sqlModel.setTable(tableName);

        QList<Person> people;

        if (sqlModel.select())
        {
            for (int r = 0; r < sqlModel.rowCount(); r++)
            {
                QList<QString> persInList;

                for (int c = 0; c < Person::pattern().count(); c++)
                {
                    persInList << sqlModel.record(r).value(c + 1).toString();
                }

                Person pers(persInList);

                pers.id = sqlModel.record(r).value("id").toInt();

                people << pers;
            }
        }
        else
            qWarning() << "error in PeopleManager::getAllPeople - " + sqlModel.lastError().text();

        return people;
    }

private:
    // метод добавления человека
    bool addPerson(Person person)
    {
        // создается и исполняется sql запрос на создания нового человека в бд
        QSqlQuery query;
        query.prepare(
                    "INSERT INTO " + tableName +
                    "   (first_name, second_name, last_name, birthday, sport_type)               "
                    "   VALUES (:first_name, :second_name, :last_name, :birthday, :sport_type)   ");

        for (QString field : person.getInList())
        {
            query.addBindValue(field);
        }

        bool ok = query.exec();
        QString err = query.lastError().text();
        auto dd = err;
        return ok;
    }


    // метод обновления человека в бд
    bool updatePerson(Person person)
    {
        // создается и исполняется sql запрос
        // на обновление человека в бд по id
        QSqlQuery query;
        query.prepare(" UPDATE " + tableName + " SET    " +
                      "     first_name = (:first_name), "
                      "     second_name = (:second_name), "
                      "     last_name = (:last_name),   "
                      "     birthday = (:birthday),     "
                      "     sport_type = (:sport_type)  "
                      " WHERE id = (:id)                ");


        query.bindValue(":id", person.id);

        for (QString field : person.getInList())
        {
            query.addBindValue(field);
        }

        return query.exec(); // возвращается результат операции
    }
};






