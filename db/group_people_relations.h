#pragma once

#include <QtSql> // для работы с бд

class GroupPeopleRelations
{
    QString groupTable;
    QString peopleTable;
    QString groupPeopleTable;

public:
    GroupPeopleRelations(QString groupTable,
                         QString peopleTable)
    {
        constructor(groupTable, peopleTable, groupTable + "_" + peopleTable);
    }

    GroupPeopleRelations(QString groupTable,
                         QString peopleTable,
                         QString specifiedGroupPeopleTable)
    {
        constructor(groupTable, peopleTable, specifiedGroupPeopleTable);
    }


    bool updateLinks(int group_id, QList<int> people_ids)
    {
        if (removeGroupLinks(group_id))
        {
            return addLinks(group_id, people_ids);
        }
        return false;
    }

    bool removeGroupLinks(int group_id)
    {
        QSqlQuery query;
        query.prepare("DELETE FROM " + groupPeopleTable +
                      " WHERE group_id = (:group_id)");

        query.addBindValue(group_id);

        if (query.exec())
            return true;
        else
            qWarning() << "group links in " + groupPeopleTable +
                          " table was not removed. "
                       << query.lastError().text();

        return false;
    }

    QList<int> *getLinks(int group_id)
    {
        QSqlQuery query;
        query.prepare("SELECT person_id FROM " + groupPeopleTable +
                      " WHERE group_id = (:id)");
        query.addBindValue(group_id);

        if (query.exec())
        {
            QList<int> *people_ids = new QList<int>;

            while (query.next())
            {
                *people_ids << query.record().value(0).toInt();
            }

            return  people_ids;
        }
        else
            qWarning() << query.lastError().text();

        return nullptr;
    }


private:
    bool addLinks(int group_id, QList<int> people_ids)
    {
        for (int pers_id : people_ids)
        {
            QSqlQuery query;
            query.prepare("INSERT INTO " + groupPeopleTable  +
                          "     (group_id, person_id)       "
                          " VALUES (:group_id, :person_id)  ");

            query.addBindValue(group_id);
            query.addBindValue(pers_id);

            if (query.exec() == false)
            {
                qDebug() << query.lastError().text();
                return false;
            }
        }
        return true;
    }

    void constructor(QString groupTable,
                     QString peopleTable,
                     QString groupPeopleTable)
    {
        this->groupPeopleTable = groupPeopleTable;
        this->groupTable = groupTable;
        this->peopleTable = peopleTable;

        touchTable(groupPeopleTable);
    }

    void touchTable(QString table)
    {
        QSqlQuery query("SELECT name FROM sqlite_master"
                        " WHERE name='" + table + "'");

        if (query.next() == false)
        {
            if (query.exec("PRAGMA foreign_keys = ON"))
            {
                query.exec("CREATE TABLE IF NOT EXISTS " + table + " (                  \n" +
                           " group_id INTEGER NOT NULL,                                 \n"
                           " person_id INTEGER NOT NULL,                                \n"
                           " FOREIGN KEY (group_id) REFERENCES " + groupTable + "(id)   \n"
                           " FOREIGN KEY (person_id) REFERENCES " + peopleTable + "(id) \n"
                           ")                                                           \n");

                if (query.lastError().isValid() == false)
                    qDebug() << "Creating " + table + " table inside database";
            }
        }
    }

};





