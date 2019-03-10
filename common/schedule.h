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
    QString sportType;

    QList<Group> groups;

    Schedule(QString title, Event event, QDate date, QString sportType)
    {
        setFullFielsd(title, event, date, sportType);
    }
    Schedule(QList<QString> property)
    {
        setFullList(property);
    }
    Schedule(int id)
    {
        this->id = id;
    }
    Schedule() {}

    QString getStringDate()
    {
        return date.toString("yyyy.MM.dd");
    }

    QDate getDate()
    {
        return date;
    }

    void setDate(QDate inputDate)
    {
        date = inputDate;
    }

    void setFullFielsd(QString title, Event event,
                         QDate date, QString sportType)
    {
        this->title = title;
        this->event_int = event;
        this->date = date;
        this->sportType = sportType;
    }

    void setFullList(QList<QString> property)
    {
        if (property.count() == getFullPattern().count())
        {
            title = property.takeFirst();
            event_int = Event(property.takeFirst().toInt());
            setDate(QDate::fromString(property.takeFirst(), "yyyy.MM.dd"));
            sportType = property.takeFirst();
        }
    }

    static QList<QString> getFullPattern()
    {
        return {"Заголовок", "Событие", "Дата проведения", "Вид спорта"};
    }

    QList<QString> getFullList()
    {
        return {title, QString::number(int(event_int)), getStringDate(), sportType};
    }

    QList<QString> getPreviewList()
    {
        return {title, getEvent(), getStringDate()};
    }

    static QList<QString> getPreviewPattern()
    {
        return {"Событие", "Вид события", "Дата проведения"};
    }

    // TODO make full lists, and replace this in DbManager

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

    friend bool operator== (const Schedule &s1, const Schedule &s2);
    friend bool operator!= (const Schedule &s1, const Schedule &s2);
};

Q_DECLARE_METATYPE(Schedule)



















