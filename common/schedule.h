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

    QString sportType;

    Schedule(QList<QString> personData)
    {
        date = QDate::currentDate();

        setInList(personData);
    }
    Schedule() {}

    void setInList(QList<QString>)
    {
    }

    QList<QString> getInList()
    {
        return QList<QString>();
    }

    static QList<QString> getPattern()
    {
        return QList<QString>();
    }
};






















