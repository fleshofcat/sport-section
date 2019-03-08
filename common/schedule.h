#pragma once

#include <QDateTime>

#include "group.h"

class Schedule
{
public:
    enum class Event {
        EMPTY,
        TRAINING,
        COMPETITION
    };

    int id = 0;
    Event event = Event::EMPTY;
    QString date;
    QString sportType;

    QList<Group> groups;

    Schedule(Event event, QString date, QString sportType)
    {
        this->event = event;
        this->date = date;
        this->sportType = sportType;
    }

    Schedule(QList<QString> property)
    {
        setInList(property);
    }

    Schedule(int id)
    {
        this->id = id;
    }

    Schedule() {}

    void setInList(QList<QString> property)
    {
        if (property.count() == pattern().count())
        {
            event = Event(property.takeFirst().toInt());
            date = property.takeFirst();
            sportType = property.takeFirst();
        }
    }

    QList<QString> getInList()
    {
        return {QString::number(int(event)), date, sportType};
    }

    QList<int> getGroupsIds()
    {
        QList<int> groupsIds;

        for (Group group : groups)
        {
            groupsIds << group.id;
        }

        return groupsIds;
    }

    static QList<QString> pattern()
    {
        return {"Событие", "Дата проведения", "Вид спорта"};
    }

    static QList<QList<QString>>
    toStringTable(QList<Schedule> schedules)
    {
        QList<QList<QString>> stringTable;
        for (Schedule sch : schedules)
        {
            auto inList = sch.getInList();

            if (inList.first().toInt() == int(Event::EMPTY))
            {
                inList.first() = "Сбор";
            }
            else if (inList.first().toInt() == int(Event::TRAINING))
            {
                inList.first() = "Тренировка";
            }
            else if (inList.first().toInt() == int(Event::COMPETITION))
            {
                inList.first() = "Соревнование";
            }

            stringTable << inList;
        }

        return stringTable;
    }

    friend bool operator== (const Schedule &s1, const Schedule &s2);
    friend bool operator!= (const Schedule &s1, const Schedule &s2);
};

Q_DECLARE_METATYPE(Schedule)



















