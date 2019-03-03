#pragma once

#include <QList>
//#include "common/person.h"

// класс Group/Группа
// хранение данных о графике тренировок тренеров и детей
class Group
{
public:
    int id = 0;         // присваивается в бд

    QString groupName;
    QString sportType;

    QList<int> trainers_ids;
    QList<int> sportsmen_ids;

    // констуктор мгновенного создания полного объекта
    Group(QList<QString> property)
    {
        setInList(property);
    }

    // конструктор создания пустого объекта
    Group() {}

    void setInList(QList<QString> property)
    {
        if (property.count() == Group::getPattern().count())
        {
            groupName = property.at(0);
            sportType = property.at(1);
        }
    }

    void dropFakeIds(QList<int> all_sportsmen, QList<int> all_trainers)
    {
        sportsmen_ids = clearFakeIds(sportsmen_ids, all_sportsmen);
        trainers_ids = clearFakeIds(trainers_ids, all_trainers);
    }


    static QList<int> clearFakeIds(QList<int> ids_for_clear, QList<int> all_ids)
    {
        for (int id : ids_for_clear)
        {
            if (!all_ids.contains(id))
            {
                ids_for_clear.removeAll(id);
            }
        }

        return ids_for_clear;
    }

    QList<QString> getInList()
    {
        return {groupName, sportType};
    }

    static QList<QString> getPattern()
    {
        return {"Группа", "Спорт"};
    }

    static QList<QList<QString>>
    groupListToStringTable(QList<Group> groups)
    {
        QList<QList<QString>> stringTable;
        for (Group group : groups)
        {
            stringTable << group.getInList();
        }

        return stringTable;
    }
};





















