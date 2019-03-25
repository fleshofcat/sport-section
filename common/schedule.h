#pragma once

#include <QDateTime>

#include "group.h"

class Schedule
{


    QDate date;

public:
    enum class Event {
        EMPTY,
        TRAINING,
        COMPETITION
    };

    int id = 0;
    Event event_int = Event::TRAINING;
    QString title;

    QList<Group> groups;

    // TODO rm
//    Schedule(QString title, Event event, QDate date, QString sportType)
//    {
//        setFields(title, event, date, sportType);
//    }
    Schedule(int id)
    {
        this->id = id;
    }
    Schedule() {}

    QString getStringDate()
    {
        return date.toString("dd.MM.yyyy");
    }

    QDate getDate()
    {
        return date;
    }

    void setDate(QDate inputDate)
    {
        date = inputDate;
    }

    void setTitle(QString title)
    {
        this->title = title;
    }

    QString getSportType()
    {
        return groups.isEmpty() ?
                    ""
                  : groups[0].getSportType();
    }

    void setFields(QString title, Event event,
                         QDate date)
    {
        this->title = title;
        this->event_int = event;
        this->date = date;
    }

    static QList<QString> getFullPattern()
    {
        return {"Заголовок", "Событие", "Дата проведения", "Вид спорта"};
    }

    void setSavableProperty(QList<QString> property)
    {
        if (property.count() == getSaveblePattern().count())
        {
            title = property.takeFirst();
            event_int = Event(property.takeFirst().toInt());
            setDate(QDate::fromString(property.takeFirst(), "dd.MM.yyyy"));
        }
    }

    QList<QString> getSavebleProperty()
    {
        return {title, QString::number(int(event_int)), getStringDate()};
    }

    static QList<QString> getSaveblePattern()
    {
        return {"Заголовок", "Событие", "Дата проведения"};
    }

    QList<QString> getPreviewList()
    {
        return {title, getEvent(), getStringDate()};
    }

    static QList<QString> getPreviewPattern()
    {
        return {"Событие", "Вид события", "Дата проведения"};
    }

    Schedule::Event getEventNumber()
    {
        return this->event_int;
    }

    QString getEvent()
    {
        if (event_int == Event::TRAINING)
        {
            return "Тренировка";
        }
        if (event_int == Event::COMPETITION)
        {
            return "Соревнование";
        }

        return "";
    }

    void setEvent(Schedule::Event event)
    {
        this->event_int = event;
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

    static QList<QList<QString>>
    toStringTable(QList<Schedule> schedules)
    {
        QList<QList<QString>> stringTable;
        for (Schedule sch : schedules)
        {
            stringTable << sch.getPreviewList();
        }

        return stringTable;
    }

    static Schedule
    firstScheduleWithGroup(QList<Schedule> schedules, int group_id)
    {
        for (Schedule sch : schedules)
        {
            if (sch.getGroupsIds().contains(group_id))
            {
                return sch;
            }
        }

        return Schedule();
    }

    friend bool operator== (const Schedule &s1, const Schedule &s2);
    friend bool operator!= (const Schedule &s1, const Schedule &s2);
};

Q_DECLARE_METATYPE(Schedule)



















