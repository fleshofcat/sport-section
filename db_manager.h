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

        if (db.open() == false)
        {
            qDebug() << "Error: Database connection is failed";
        }
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























