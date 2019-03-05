#pragma once

#include <QList>
#include "common/person.h"

// класс Group/Группа
// хранение данных о графике тренировок тренеров и детей
class Group
{
public:
    int id = 0;         // присваивается в бд

    QString groupName;
    QString sportType;

    QList<Person> trainers;
    QList<Person> sportsmen;

//    QList<int> trainers_ids;
//    QList<int> sportsmen_ids;

    // констуктор мгновенного создания полного объекта
    Group(QList<QString> property)
    {
        setInList(property);
    }

    Group(int id)
    {
        this->id = id;
    }

    // конструктор создания пустого объекта
    Group() {}

    void setInList(QList<QString> property)
    {
        if (property.count() == Group::pattern().count())
        {
            groupName = property.at(0);
            sportType = property.at(1);
        }
    }

    QList<QString> getInList()
    {
        return {groupName, sportType};
    }

    QList<int> getTrainersIds()
    {
        QList<int> trainers_ids;

        for (auto trainer : trainers)
        {
            trainers_ids << trainer.id;
        }

        return trainers_ids;
    }

    QList<int> getSportsmenIds()
    {
        QList<int> sportsmen_ids;

        for (auto trainer : sportsmen)
        {
            sportsmen_ids << trainer.id;
        }

        return sportsmen_ids;
    }

    static QList<QString> pattern()
    {
        return {"Группа", "Спорт"};
    }

    static QList<QList<QString>>
    toStringTable(QList<Group> groups)
    {
        QList<QList<QString>> stringTable;
        for (Group group : groups)
        {
            stringTable << group.getInList();
        }

        return stringTable;
    }

    friend bool operator== (const Group &g1, const Group &g2);
    friend bool operator!= (const Group &g1, const Group &g2);
};


Q_DECLARE_METATYPE(Group)



















