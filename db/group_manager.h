#pragma once

#include <QtSql> // для работы с бд

#include "common/common_objects.h"
#include "db/group_people_relations.h"

// класс ScheduleManager/МенеджерРасписаний
//
// отвечает за хранение расписаний в бд
class GroupManager : public QObject
{
    Q_OBJECT

    friend class TestGroupStorage;

    QString groupTable;

    GroupPeopleRelations *refsToTrainers;
    GroupPeopleRelations *refsToSportsmen;

public:
    // конструктор
    // если при создании этого объекта в бд нет нужной ему таблицы
    // он сам создаст ее
    explicit GroupManager(QString groupTable,
                          QString trainerTable,
                          QString sportsmanTable,
                          QObject *parent = nullptr)
        : QObject(parent)
    {
        this->groupTable = groupTable;
        touchGroupTable(groupTable);

        refsToTrainers = new GroupPeopleRelations(
                    groupTable, trainerTable);

        refsToSportsmen = new GroupPeopleRelations(
                    groupTable, sportsmanTable);
    }

    bool saveGroup(Group group)
    {
        if (group.id < 1)
        {
            return addGroup(group);
        }
        else
        {
            return updateGroup(group);
        }
    }

    bool removeGroup(int group_id)
    {
        if (refsToTrainers->removeGroupLinks(group_id))
        {
            if (refsToSportsmen->removeGroupLinks(group_id))
            {
                QSqlQuery query;
                query.prepare("DELETE FROM " + groupTable + " WHERE id = (:id)");
                query.addBindValue(group_id);

                if (query.exec())
                {
                    return true;
                }
                else
                {
                    qWarning() << "group was not removed. "
                               << query.lastError().text();
                }
            }
        }

        return false;
    }


    QList<Group> *getGroups()
    {
        QSqlQuery query("SELECT * FROM " + groupTable);

        if (query.lastError().isValid() == false)
        {
            QList<Group> *groups = new QList<Group>;

            while (query.next())
            {
                QList<QString> groupInList;

                for (int i = 0; i < Group::getPattern().count(); i++)
                {
                    groupInList << query.record().value(i + 1).toString();
                }

                Group group(groupInList);
                group.id = query.record().value("id").toInt();

                auto trainersRefs = refsToTrainers->getLinks(group.id);
                auto sportsmenRefs = refsToSportsmen->getLinks(group.id);

                if (trainersRefs != nullptr &&
                        sportsmenRefs != nullptr)
                {
                    group.trainers_ids = *trainersRefs;
                    group.sportsmen_ids = *sportsmenRefs;
                    *groups << group;
                }
            }
            return groups;

        } else
            qWarning() << query.lastError().text();

        return nullptr;
    }

private:
    bool addGroup(Group group)
    {       
        int maxGroupId = getMaxIdFromTable(groupTable);


        if (maxGroupId >= 0)
        {
            group.id = maxGroupId + 1;

            QSqlQuery query;
            query.prepare("INSERT INTO " + groupTable +
                          "         (id, group_name, sport_type)    "
                          " VALUES  (:id, :group_name, :sport_type) ");

            query.addBindValue(group.id);
            bindValueList(query, group.getInList());

            if (query.exec())
            {
                if (refsToTrainers->updateLinks(group.id, group.trainers_ids))
                {
                    if (refsToSportsmen->updateLinks(group.id, group.sportsmen_ids))
                    {
                        return true;
                    }
                }
            } else
                qWarning() << query.lastError().text();
        } else
            qWarning() << "GroupManager::addGroup failed to get maxGroupId";

        return false;
    }


    bool updateGroup(Group group)
    {
        QSqlQuery query;
        query.prepare(" UPDATE " + groupTable + " SET   \n" +
                      "     group_name = (:group_name), \n"
                      "     sport_type = (:sport_type)  \n"
                      " WHERE id = (:id)                \n");


        query.bindValue(":id", group.id);
        bindValueList(query, group.getInList());

        if (query.exec())
        {
            if (refsToTrainers->updateLinks(group.id, group.trainers_ids))
            {
                if (refsToSportsmen->updateLinks(group.id, group.sportsmen_ids))
                {
                    return true;
                }
            }
        } else
            qWarning() << "group was not updated in "
                          + groupTable + " table. "
                       << query.lastError().text();

        return false;
    }


    void touchGroupTable(QString groupTable)
    {
        QSqlQuery query("SELECT name FROM sqlite_master"
                        " WHERE name='" + groupTable + "'");

        if (query.next() == false)
        {
            query.exec("CREATE TABLE IF NOT EXISTS " + groupTable + " ( \n" +
                       " id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,   \n"
                       " group_name TEXT NOT NULL,                      \n"
                       " sport_type TEXT NOT NULL                       \n"
                       ")                                                 ");

            if (query.lastError().isValid() == false)
                qDebug() << "Creating " + groupTable + " table inside database";
        }
    }


    static int getMaxIdFromTable(QString table) // возвращает -1 при неудаче
    {
        QSqlQuery query("SELECT MAX(id) FROM " + table);

        if (query.lastError().isValid() == false)
        {
            if (query.next())
            {
                return query.record().value(0).toInt();
            }
        }
        else
        {
            qWarning() << query.lastError().text();
        }
        return -1;
    }

    void bindValueList(QSqlQuery &query, QList<QString> list) // TODO test it
    {
        for (QString field : list)
        {
            query.addBindValue(field);
        }
    }

};



