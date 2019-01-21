#pragma once

#include <QtSql>

#include "people_storage.h"
#include "relation_storage.h"

class DbManager : public QObject
{
    Q_OBJECT
public:
    PeopleStorage people;
    RelationStorage relations;

    explicit DbManager(QString db_path, QObject *parent = nullptr) : QObject (parent)
    {        
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(db_path);

        if (db.open() == true)
        {
            QSqlQuery query; // create database if not exist
            query.exec("CREATE TABLE IF NOT EXISTS people                   "
                       " ( id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,   "
                       " first_name TEXT NOT NULL,                          "
                       " last_name TEXT NOT NULL,                           "
                       " birthday TEXT NOT NULL,                            "
                       " sport_type TEXT NOT NULL,                          "
                       " status TEXT NOT NULL)                              ");

            query.exec("CREATE TABLE IF NOT EXISTS relations                "
                       " ( id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,   "
                       " trainer_id INTEGER NOT NULL,                       "
                       " child_id INTEGER NOT NULL,                         "
                       " sport_type TEXT NOT NULL)                          ");
        }
        else
        {
            qDebug() << "Error: Database connection is failed";
        }

    }

    ~DbManager()
    {
        {
            QSqlDatabase db = QSqlDatabase::database("QSQLITE");
            db.close();
        }
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }
};























