#pragma once

#include "group.h"

class Schedule
{
public:
    enum class Event {
        EMPTY,
        TRAINING,
        COMPETITION
    };

private:
    QDate date;
    Event event = Event::TRAINING;
    QString title;

public:
    int id = 0;
    QList<Group> groups;

    Schedule(int id)
    {
        this->id = id;
    }
    Schedule() {}

    void setTitle(QString newTitle)
    {
        this->title = newTitle;
    }
    QString getTitle()
    {
        return this->title;
    }

    void setDate(QDate newDate)
    {
        date = newDate;
    }
    void setDate(QString newDate)
    {
        setDate(QDate::fromString(newDate, "dd.MM.yyyy"));
    }
    QDate getDate()
    {
        return date;
    }
    QString getDateInString()
    {
        return getDate().toString("dd.MM.yyyy");
    }

    void setEvent(Schedule::Event newEvent)
    {
        event = newEvent;
    }
    void setEvent(int newEvent)
    {
        setEvent(Schedule::Event(newEvent));
    }
    Schedule::Event getEvent()
    {
        return event;
    }
    QString getEventInString()
    {
        if (getEvent() == Event::TRAINING)
        {
            return "Тренировка";
        }
        if (getEvent() == Event::COMPETITION)
        {
            return "Соревнование";
        }

        return "";
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
        setTitle(title);
        setEvent(event);
        setDate(date);
    }

    static QList<QString> getFullPattern()
    {
        return {"Заголовок", "Событие", "Дата проведения", "Вид спорта"};
    }

    void setSavableProperty(QList<QString> property)
    {
        if (property.count() == getSaveblePattern().count())
        {
            setTitle(property.takeFirst());
            setEvent(property.takeFirst().toInt());
            setDate(property.takeFirst());
        }
    }

    QList<QString> getSavebleProperty()
    {
        return {getTitle(), QString::number(int(getEvent())), getDateInString()};
    }

    static QList<QString> getSaveblePattern()
    {
        return {"Заголовок", "Событие", "Дата проведения"};
    }

    QList<QString> getPreviewList()
    {
        return {getTitle(), getEventInString(), getDateInString()};
    }

    static QList<QString> getPreviewPattern()
    {
        return {"Событие", "Вид события", "Дата проведения"};
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
    toPreviewTable(QList<Schedule> schedules)
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



















