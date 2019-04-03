#pragma once

#include <QtSql>

#include "common/group.h"
#include "db/relations_in_db.h"

class GroupManager : public QObject
{
    Q_OBJECT

    friend class TestGroupManager;

    QString groupTable;

    RelationsInDb refsToTrainers;
    RelationsInDb refsToSportsmen;

public:
    explicit GroupManager(QObject *parent = nullptr);
    explicit GroupManager(QString groupTable,
                          QString trainerTable,
                          QString sportsmanTable,
                          QObject *parent = nullptr);

    void touchManager(QString groupTable,
                      QString trainerTable,
                      QString sportsmanTable);

    bool saveGroup(Group group);
    bool removeGroup(int group_id);

    QList<Group> getGroups();
    Group getGroup(int id);

private:
    bool addGroup(Group group);
    bool updateGroup(Group group);

    void touchTable(QString groupTable);

    // возвращает -1 при неудаче
    static int getMaxIdFromTable(QString table);
    static void bindValueList(QSqlQuery &query, QList<QString> list);
};



