#pragma once

#include <QList>
#include <QDateTime>

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

    QList<int> group_ids;

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
    Schedule() {}

    void setInList(QList<QString> property)
    {
        if (property.count() == getPattern().count())
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

    static QList<QString> getPattern()
    {
        return {"Событие", "Дата", "Вид спорта"};
    }
};






















