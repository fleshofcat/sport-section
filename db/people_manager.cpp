#include "db/people_manager.h"


PeopleManager::PeopleManager(QObject *parent)
    : QObject(parent) {}

PeopleManager::PeopleManager(QString tableName,
              QObject *parent)
    : QObject(parent)
{
    touchManager(tableName);
}


void PeopleManager::touchManager(QString table)
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
                    " sport_type TEXT NOT NULL,                        \n"
                    " phone_number TEXT,                               \n"
                    " rating TEXT NOT NULL,                            \n"
                    " events TEXT NOT NULL                             \n"
                    " )                                                \n");

        // вывод отладочной информации что таблица в бд была создана
        if (query.lastError().isValid() == false)
            qDebug() << "Creating " + table + " table inside database";
    }
}


bool PeopleManager::savePerson(Person pers)
{
    return pers.id <= 0 ? addPerson(pers) : updatePerson(pers);
}


// метод удаления человека
bool PeopleManager::removePerson(int id)
{
    // создается и исполняется sql запрос
    // на удаление человека в бд по id

    QSqlQuery query;
    query.prepare("DELETE FROM " + tableName + " WHERE id = (:id)");
    query.bindValue(":id", id);

    return query.exec(); // возвращается результат запроса
}


QList<Person> PeopleManager::getPeople()
{
    QSqlTableModel sqlModel;
    sqlModel.setTable(tableName);

    QList<Person> people;

    if (sqlModel.select())
    {
        for (int r = 0; r < sqlModel.rowCount(); r++)
        {
            QList<QString> persInList;

            for (int c = 0; c < Person::getFullPattern().count(); c++)
            {
                persInList << sqlModel.record(r).value(c + 1).toString();
            }

            Person pers;
            pers.setFullProperty(persInList);
            pers.id = sqlModel.record(r).value("id").toInt();

            people << pers;
        }
    }
    else
        qWarning() << "error in PeopleManager::getAllPeople - " + sqlModel.lastError().text();

    return people;
}

Person PeopleManager::getPerson(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM " + tableName + " WHERE id = (:id)");
    query.bindValue(":id", id);

    if (query.exec())
    {
        if (query.next())
        {
            QList<QString> persInList;

            for (int c = 0; c < Person::getFullPattern().count(); c++)
            {
                persInList << query.record().value(c + 1).toString();
            }

            Person pers;
            pers.setFullProperty(persInList);
            pers.id = query.record().value("id").toInt();

            return pers;
        }
    }
    return Person();
}

// метод добавления человека
bool PeopleManager::addPerson(Person person)
{
    // создается и исполняется sql запрос на создания нового человека в бд
    QSqlQuery query;
    query.prepare(
                "INSERT INTO " + tableName +
                " (first_name,              "
                "  second_name,             "
                "  last_name,               "
                "  birthday,                "
                "  sport_type,              "
                "  phone_number,            "
                "  rating,                  "
                "  events)                  "
                "   VALUES                  "
                " (:first_name,             "
                "  :second_name,            "
                "  :last_name,              "
                "  :birthday,               "
                "  :sport_type,             "
                "  :phone_number,           "
                "  :rating,                 "
                "  :events)                 ");

    for (QString field : person.getFullProperty())
    {
        query.addBindValue(field);
    }

    return query.exec();
}


// метод обновления человека в бд
bool PeopleManager::updatePerson(Person person)
{
    // создается и исполняется sql запрос
    // на обновление человека в бд по id
    QSqlQuery query;
    query.prepare(" UPDATE " + tableName + " SET        " +
                  "     first_name = (:first_name),     "
                  "     second_name = (:second_name),   "
                  "     last_name = (:last_name),       "
                  "     birthday = (:birthday),         "
                  "     sport_type = (:sport_type),     "
                  "     phone_number = (:phone_number), "
                  "     rating = (:rating),             "
                  "     events = (:events)              "
                  " WHERE id = (:id)                    ");


    query.bindValue(":id", person.id);

    for (QString field : person.getFullProperty())
    {
        query.addBindValue(field);
    }

    return query.exec(); // возвращается результат операции
}




































