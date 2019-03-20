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
    int eventNumber = 0;

    QList<Person> trainers;
    QList<Person> sportsmen;

    Group(QString groupName)
    {
        this->groupName = groupName;
    }
    Group(int id)
    {
        this->id = id;
    }
    Group() {}

    QList<QString> getInList()
    {
        return {groupName, getSportTypeByPeople()};
    }

    static QList<QString> getPattern()
    {
        return {"Группа", "Спорт"};
    }

    void setSaveableProperty(QList<QString> property)
    {
        if (property.count() == getSaveableProperty().count())
        {
            groupName   = property.takeFirst();
            eventNumber = property.takeFirst().toInt();
        }
    }

    QList<QString> getSaveableProperty()
    {
        return {groupName, QString::number(eventNumber)};
    }

    static QList<QString> getSaveablePattern()
    {
        return {"Группа", "Количество мероприятий"};
    }

    QList<QString> getFullProperty()
    {
        return { groupName,
                    getSportTypeByPeople(),
                    QString::number(eventNumber),
                    QString::number(getGroupRating()) };
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

    QString getSportTypeByPeople()
    {
        QString sport;
        if (!trainers.isEmpty())
        {
            sport = trainers.first().sportType;
        }
        else if (!sportsmen.isEmpty())
        {
            sport = sportsmen.first().sportType;
        }

        return sport;
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

    static QList<Person>
    getFreeSportsmen(QList<Group> allGroups, QList<Person> allSportsmen)
    {
        for (Group group : allGroups)
        {
            allSportsmen = Person::getFreePeople(
                        allSportsmen, group.sportsmen);
        }
        return allSportsmen;
    }

    static Group
    firstGroupWithSportsmen(QList<Group> groups, int sportsmen_id)
    {
        for (Group group : groups)
        {
            if (group.getSportsmenIds().contains(sportsmen_id))
            {
                return group;
            }
        }

        return Group();
    }

    static Group
    firstGroupWithTrainer(QList<Group> groups, int trainer_id)
    {
        for (Group group : groups)
        {
            if (group.getTrainersIds().contains(trainer_id))
            {
                return group;
            }
        }

        return Group();
    }

    friend bool operator== (const Group &g1, const Group &g2);
    friend bool operator!= (const Group &g1, const Group &g2);
};


Q_DECLARE_METATYPE(Group)



















