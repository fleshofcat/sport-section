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

    int eventNumber = 0;

    QList<Person> trainers;
    QList<Person> sportsmen;

    Group(QList<QString> property)
    {
        setInList(property);
    }
    Group(int id)
    {
        this->id = id;
    }
    Group() {}

    void setInList(QList<QString> property)
    {
        if (property.count() == Group::getPattern().count())
        {
            groupName = property.at(0);
            sportType = property.at(1);
        }
    }

    QList<QString> getInList()
    {
        return {groupName, sportType};
    }

    static QList<QString> getPattern()
    {
        return {"Группа", "Спорт"};
    }

    void setFullList(QList<QString> property)
    {
        if (property.count() == getFullList().count())
        {
            groupName   = property.takeFirst();
            sportType   = property.takeFirst();
            eventNumber = property.takeFirst().toInt();
        }
    }

    QList<QString> getFullList()
    {
        return {groupName, sportType, QString::number(eventNumber)};
    }

    static QList<QString> getFullPattern()
    {
        return {"Группа", "Спорт", "Количество мероприятий"};
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

    double getGroupRating() // was float TODO
    {
        if (!sportsmen.isEmpty())
        {
            int sportsmenRating = getFullSportsmenRating();

            return double(sportsmenRating) / double(sportsmen.count());;
        }
        return 0;
    }

    int getFullSportsmenRating()
    {
        return Person::getPeopleRating(sportsmen);
    }

    void updateSportsman(Person pers)
    {
        for (int i = 0; i < sportsmen.count(); i++)
        {
            if (sportsmen.at(i).id == pers.id)
            {
                sportsmen[i] = pers;
                return;
            }
        }
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

    static QList<Person> getAllSportsmen(QList<Group> groups)
    {
        QList<Person> allSportsmen;

        for (auto group : groups)
        {
            allSportsmen << group.sportsmen;
        }

        return allSportsmen;
    }

    friend bool operator== (const Group &g1, const Group &g2);
    friend bool operator!= (const Group &g1, const Group &g2);
};


Q_DECLARE_METATYPE(Group)



















