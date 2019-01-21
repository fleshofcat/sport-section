#pragma once

#include <QtSql>

#include "relation.h"

class RelationStorage : public QObject
{
    Q_OBJECT
    QString tableName = "relations";

public:
    explicit RelationStorage(QObject *parent = nullptr)
        : QObject(parent)
    {               // create relation table in db if not exist
        QSqlQuery query("SELECT name FROM sqlite_master"
                        " WHERE name='" + tableName + "'");

        query.next();
        if (query.value(0).toString() != tableName)
        {
            query.exec("CREATE TABLE IF NOT EXISTS relations                "
                       " ( id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,   "
                       " trainer_id INTEGER NOT NULL,                       "
                       " child_id INTEGER NOT NULL)                         ");
            //                   " sport_type TEXT NOT NULL)                          ");

            qDebug() << "Creating relation table inside database";
        }
    }


    bool addRelation(Relation rel)
    {
        if (rel.isFull() == false)
            return false;

        QSqlQuery query;
        query.prepare("INSERT INTO " + tableName +
                      "         (trainer_id, child_id)"
                      " VALUES  (:trainer_id, :child_id)");

        query.bindValue(":trainer_id", rel.trainer_id);
        query.bindValue(":child_id", rel.child_id);

        bool ret = query.exec();

        if (!ret)
            qDebug() << "query.exec() in addRelation failed";

        return ret;
    }


    bool removeRelation(Relation rel)
    {
        QSqlQuery query;

        query.prepare("DELETE FROM " + tableName + " WHERE id = (:id)");
        query.bindValue(":id", rel.id);

        bool ret = query.exec();

        if (!ret)
            qDebug() << "query.exec() in removeRelation failed";

        return ret;
    }


    bool replaceRelationById(Relation rel)
    {
        QSqlQuery query;
        query.prepare(" UPDATE " + tableName + " SET    "
                      "     trainer_id = (:trainer_id), "
                      "     child_id = (:child_id)      "
                      " WHERE id = (:id)                ");


        query.bindValue(":id", rel.id);
        query.bindValue(":trainer_id", rel.trainer_id);
        query.bindValue(":child_id", rel.child_id);

        bool ret = query.exec();

        if (!ret)
            qDebug() << "query.exec() in replace relation failed";

        return ret;
    }


    QList<Relation> *getAllRelations()
    {
        QSqlQuery query;
        query.prepare("SELECT * FROM " + tableName);

        QList<Relation> *relationsList = new QList<Relation>();

        if (query.exec())
        {
            while (query.next())
            {

                Relation *rel = new Relation();

                rel->id = query.value("id").toInt();
                rel->trainer_id = query.value("trainer_id").toInt();
                rel->child_id = query.value("child_id").toInt();

                *relationsList << *rel;
            }
            return relationsList;
        }

        qDebug() << "query.exec() in get all relations failed";
        return nullptr;
    }
};





