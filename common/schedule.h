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
    } event = Event::EMPTY;

    int id = 0;
    QDate date;

    Schedule(QList<QString>) { }
    Schedule() {}
};






















