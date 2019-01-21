#pragma once

#include <QtSql>

#include "relation.h"


class RelationStorage : public QObject
{
    Q_OBJECT
    QString tableName = "relations";

public:
    explicit RelationStorage(QObject *parent = nullptr)
        : QObject(parent) {}


    bool addRelation(Relation rel) // TODO
    {
        if (rel.isFull() == false)
            return false;

        QSqlQuery query;
        query.prepare("INSERT INTO " + tableName +
                      "         (trainer_id, child_id, sport_type)"
                      " VALUES  (:trainer_id, :child_id, :sport_type)");

        query.bindValue(":trainer_id", rel.trainer_id);
        query.bindValue(":child_id", rel.child_id);
        query.bindValue(":sport_type", rel.sportType);

        bool ret = query.exec();

        if (!ret)
            qDebug() << "query.exec() in add failed";

        return ret;
    }

    bool isValid(Relation /*rel*/) // TODO
    {
        bool fake = true;
        return fake;
    }
};





