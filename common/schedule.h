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

    explicit Schedule(int id) : id(id) { }
    explicit Schedule() {}

    void setTitle(QString newTitle);
    QString getTitle();

    void setDate(QDate newDate);
    void setDate(QString newDate);
    QDate getDate();
    QString getDateInString();

    void setEvent(Schedule::Event newEvent);
    void setEvent(int newEvent);
    Schedule::Event getEvent();
    QString getEventInString();

    QString getSportType();


    void setFields(QString title, Event event, QDate date);

    static QList<QString> getFullPattern();

    void setSavableProperty(QList<QString> property);

    QList<QString> getSavebleProperty();

    static QList<QString> getSaveblePattern();

    QList<QString> getPreviewList();

    static QList<QString> getPreviewPattern();

    QList<int> getGroupsIds();

    static QList<QList<QString>>
    toPreviewTable(QList<Schedule> schedules);

    static Schedule firstScheduleWithGroup(
            QList<Schedule> schedules, int group_id);

    friend bool operator== (const Schedule &s1, const Schedule &s2);
    friend bool operator!= (const Schedule &s1, const Schedule &s2);
};

Q_DECLARE_METATYPE(Schedule)



















