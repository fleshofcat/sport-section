#pragma once

#include <QtSql>

class RelationsInDb : public QObject
{
    Q_OBJECT
    friend class TestGroupPeopleRelations;

    QString main;
    QString secondary;
    QString relations;

    QString main_id;
    QString secondary_id;

    bool isSecondaryIdHaveManyMainIds = true;

public:
    RelationsInDb(QObject *parent = nullptr);
    RelationsInDb(QString mainTable,
                         QString secondaryTable,
                         QString specificRelationsTable = "",
                         QObject *parent = nullptr);

    void touchManager(QString mainTable,
                      QString secondaryTable,
                      QString specificRelationsTable = "");

    bool updateLinks(int mainTable_id, QList<int> secondaryTable_ids);
    bool removeMainLinks(int mainTable_id);
    QList<int> getLinks(int mainTable_id);
    void setSecondIdHaveOnlyOneMainId(bool onlyOne);

private:
    void touchTable(QString table);
    bool addLinks(int mainTable_id, QList<int> secondaryTable_ids);
    bool isIdsValid(int mainId, QList<int> secondaryIds);
};





