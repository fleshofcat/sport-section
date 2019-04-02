#pragma once

#include <QList>
#include "common/person.h"

// класс Group/Группа
// хранение данных о графике тренировок тренеров и детей
class Group
{
    QString groupName;
    int eventsNumber = 0;

public:
    int id = 0;         // присваивается в бд

    QList<Person> trainers;
    QList<Person> sportsmen;

    Group(QString groupName)
    {
        setGroupName(groupName);
    }
    Group(int id)
    {
        this->id = id;
    }
    Group() {}

    void setGroupName(QString newGroupName)
    {
        groupName = newGroupName;
    }
    QString getGroupName()
    {
        return groupName;
    }

    void setEventsNumber(int newNumber)
    {
        eventsNumber = newNumber;
    }
    int getEventsNumber()
    {
        return eventsNumber;
    }
    void increaseEventNumber(int increaseValue)
    {
        setEventsNumber(getEventsNumber() + increaseValue);
    }

    QList<QString> getPreviewProperty()
    {
        return {getGroupName(), getSportType()};
    }

    static QList<QString> getPreviewPattern()
    {
        return {"Группа", "Спорт"};
    }


    void setSaveableProperty(QList<QString> property)
    {
        if (property.count() == getSaveableProperty().count())
        {
            setGroupName(property.takeFirst());
            setEventsNumber(property.takeFirst().toInt());
        }
    }

    QList<QString> getSaveableProperty()
    {
        return {getGroupName(), QString::number(getEventsNumber())};
    }

    static QList<QString> getSaveablePattern()
    {
        return {"Группа", "Мероприятий"};
    }


    QList<QString> getFullProperty()
    {
        return { getGroupName(),
                    getSportType(),
                    QString::number(getEventsNumber()),
                    QString::number(getRating()) };
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

    QString getSportType()
    {
        QString sport;
        if (!trainers.isEmpty())
        {
            sport = trainers.first().getSportType();
        }
        else if (!sportsmen.isEmpty())
        {
            sport = sportsmen.first().getSportType();
        }

        return sport;
    }

    double getRating()
    {
        if (!sportsmen.isEmpty())
        {
            int sportsmenRating = getAccumSportsmenRating();

            return double(sportsmenRating) / double(sportsmen.count());;
        }
        return 0;
    }

    int getAccumSportsmenRating()
    {
        return Person::getAccumRating(sportsmen);
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

    static QList<QList<QVariant>>
    toStatsTable(QList<Group> groups)
    {
        QList<QList<QVariant>> ret;

        for (auto group : groups)
        {
            QList<QVariant> stats;
            for (QString field : group.getPreviewProperty())
            {
                stats << QVariant(field);
            }
            stats << QVariant(group.getRating());
            stats << QVariant(group.getEventsNumber());

            ret << stats;
        }
        return ret;
    }

    static QList<QString>
    getStatsPattern()
    {
        auto ret = Group::getPreviewPattern();
        ret << "Рейтинг";
        ret << "Мероприятий";
        return ret;
    }

    static QList<QList<QString>>
    toStringTable(QList<Group> groups)
    {
        QList<QList<QString>> stringTable;
        for (Group group : groups)
        {
            stringTable << group.getPreviewProperty();
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
    getFirstGroupWithSportsmen(QList<Group> groups, int sportsmen_id)
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
    getFirstGroupWithTrainer(QList<Group> groups, int trainer_id)
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



















