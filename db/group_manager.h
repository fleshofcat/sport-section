#pragma once

#include <QtSql> // для работы с бд

#include "common/group.h"
#include "db/relations_in_db.h"

// класс ScheduleManager/МенеджерРасписаний
//
// отвечает за хранение расписаний в бд
class GroupManager : public QObject
{
    Q_OBJECT

    friend class TestGroupManager;

    QString groupTable;

    RelationsInDb refsToTrainers;
    RelationsInDb refsToSportsmen;

public:
    GroupManager(QObject *parent = nullptr)
        : QObject(parent) {}


    GroupManager(QString groupTable,
                 QString trainerTable,
                 QString sportsmanTable,
                 QObject *parent = nullptr)
        : QObject(parent)
    {
        touchManager(groupTable, trainerTable, sportsmanTable);
    }


    void touchManager(QString groupTable,
                      QString trainerTable,
                      QString sportsmanTable)
    {
        touchTable(groupTable);

        refsToTrainers.touchManager(groupTable, trainerTable);
        refsToSportsmen.touchManager(groupTable, sportsmanTable);
    }


    bool saveGroup(Group group)
    {
        if (group.id < 1)
            return addGroup(group);
        else
            return updateGroup(group);
    }

    bool removeGroup(int group_id)
    {
        if (refsToTrainers.removeMainLinks(group_id))
        {
            if (refsToSportsmen.removeMainLinks(group_id))
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


    QList<Group> getGroups()
    {
        QSqlQuery query("SELECT * FROM " + groupTable);

        QList<Group> groups;

        if (!query.lastError().isValid())
        {
            while (query.next())
            {
                QList<QString> groupInList;

                for (int i = 0; i < Group::pattern().count(); i++)
                {
                    groupInList << query.record().value(i + 1).toString();
                }

                Group group(groupInList);
                group.id = query.record().value("id").toInt();


                for (int id : refsToTrainers.getLinks(group.id))
                {
                    group.trainers << Person(id);
                }
                for (int id : refsToSportsmen.getLinks(group.id))
                {
                    group.sportsmen << Person(id);
                }

                groups << group;
            }

        } else
            qWarning() << query.lastError().text();

        return groups;
    }

    Group getGroup(int id)
    {
        QSqlQuery query;
        query.prepare("SELECT * FROM " + groupTable + " WHERE id = (:id)");
        query.bindValue(":id", id);

        if (query.exec())
        {
            if (query.next())
            {
                QList<QString> groupInList;

                for (int i = 0; i < Group::pattern().count(); i++)
                {
                    groupInList << query.record().value(i + 1).toString();
                }

                Group group(groupInList);
                group.id = query.record().value("id").toInt();


                for (int id : refsToTrainers.getLinks(group.id))
                {
                    group.trainers << Person(id);
                }
                for (int id : refsToSportsmen.getLinks(group.id))
                {
                    group.sportsmen << Person(id);
                }

                return group;
            }
        }

        return Group();
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
                if (refsToTrainers.updateLinks(group.id, group.getTrainersIds()))
                {
                    if (refsToSportsmen.updateLinks(group.id, group.getSportsmenIds()))
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
            if (refsToTrainers.updateLinks(group.id, group.getTrainersIds()))
            {
                if (refsToSportsmen.updateLinks(group.id, group.getSportsmenIds()))
                {
                    return true;
                }
            }
        } else
            qWarning() << "GroupManager::updateGroup: "
                       << query.lastError().text();

        return false;
    }

    void touchTable(QString groupTable)
    {
        this->groupTable = groupTable;

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

    static void bindValueList(QSqlQuery &query, QList<QString> list)
    {
        for (QString field : list)
        {
            query.addBindValue(field);
        }
    }
};



