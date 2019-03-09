#pragma once

#include <QtSql> // для работы с бд

class RelationsInDb : public QObject
{
    Q_OBJECT
    friend class TestGroupPeopleRelations;

    // table names
    QString main;
    QString secondary;
    QString relations;

    QString main_id;
    QString secondary_id;

    bool isSecondaryIdHaveManyMainIds = true;

public:
    RelationsInDb(QObject *parent = nullptr)
        : QObject(parent) { }

    RelationsInDb(QString mainTable,
                         QString secondaryTable,
                         QString specificRelationsTable = "",
                         QObject *parent = nullptr)
        : QObject(parent)
    {
        touchManager(mainTable, secondaryTable, specificRelationsTable);
    }


    void touchManager(QString mainTable,
                      QString secondaryTable,
                      QString specificRelationsTable = "")
    {
        if (specificRelationsTable == "")
        {
            specificRelationsTable = mainTable + "_" + secondaryTable;
        }

        this->relations = specificRelationsTable;
        this->main = mainTable;
        this->secondary = secondaryTable;

        this->main_id = main + "_id";
        this->secondary_id = secondary + "_id";

        touchTable(specificRelationsTable);
    }

    void setSecondIdHaveOnlyOneMainId(bool onlyOne)
    {
        isSecondaryIdHaveManyMainIds = !onlyOne;
    }

    bool updateLinks(int mainTable_id, QList<int> secondaryTable_ids)
    {
        bool idsValid = isIdsValid(mainTable_id, secondaryTable_ids);
        if (idsValid)
        {
            bool rm_success = removeMainLinks(mainTable_id);
            if (rm_success)
            {
                bool add_success = addLinks(mainTable_id, secondaryTable_ids);
                return add_success;
            }
        }
        return false;
    }

    bool removeMainLinks(int mainTable_id)
    {        
        QSqlQuery query;
        QString qqq = QString("DELETE FROM %1 WHERE %2 = (?)")
                .arg(relations).arg(main_id);

        query.prepare(qqq);
        query.addBindValue(mainTable_id);

        if (query.exec())
            return true;
        else
            qWarning() << "RelationsInDb: links in " + relations +
                          " table was not removed. "
                       << query.lastError().text();

        return false;
    }

    QList<int> getLinks(int mainTable_id)
    {
        QSqlQuery query;
        query.prepare(QString("SELECT %1 FROM %2 WHERE %3 = (:id)")
                      .arg(secondary_id).arg(relations).arg(main_id));


        query.addBindValue(mainTable_id);

        QList<int> secondaryTable_ids;

        if (query.exec())
        {
            while (query.next())
            {
                secondaryTable_ids << query.record().value(0).toInt();
            }
        }
        else
            qWarning() << query.lastError().text();

        return  secondaryTable_ids;
    }


private:
    bool addLinks(int mainTable_id, QList<int> secondaryTable_ids)
    {
        for (int id : secondaryTable_ids)
        {
            QSqlQuery query;
            query.prepare(QString("INSERT INTO %1 (%2, %3)  "
                                  "VALUES (?, ?)            ")
                          .arg(relations).arg(main_id).arg(secondary_id));

            query.addBindValue(mainTable_id);
            query.addBindValue(id);

            if (query.exec() == false)
            {
                qWarning() << query.lastError().text();
                return false;
            }
        }
        return true;
    }

    bool isIdsValid(int mainId, QList<int> secondaryIds)
    {
        if (isSecondaryIdHaveManyMainIds == false)
        {
            for (int id : secondaryIds)
            {
                QString str = QString("SELECT %1 FROM %2 "
                                      " WHERE %3 = %4 ")
                           .arg(main_id)
                           .arg(relations)
                           .arg(secondary_id)
                           .arg(QString::number(id));

                QSqlQuery query(str);


                if (query.next())
                {
                    int result = query.record().value(0).toInt();
                    if (result != mainId)
                        return false;
                }

                if (query.next())
                    return false;
            }
        }

        return true;
    }

    void touchTable(QString table)
    {
        QSqlQuery query(QString("SELECT name FROM sqlite_master"
                                " WHERE name='%1'").arg(table));

        if (query.next() == false)
        {
            if (query.exec("PRAGMA foreign_keys = ON"))
            {
                query.exec(QString("CREATE TABLE IF NOT EXISTS %1 ( \n"
                           " %2 INTEGER NOT NULL,                   \n"
                           " %3 INTEGER NOT NULL,                   \n"
                           " FOREIGN KEY (%4) REFERENCES %6(id)     \n"
                           " FOREIGN KEY (%5) REFERENCES %7(id)     \n"
                           ")                                       \n")
                           .arg(table).arg(main_id).arg(secondary_id).arg(main_id)
                           .arg(secondary_id).arg(main).arg(secondary));


                if (query.lastError().isValid() == false)
                    qDebug() << "Creating " + table + " table inside database";
            }
        }
    }

};





