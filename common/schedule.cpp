#include "common/schedule.h"


void Schedule::setTitle(QString newTitle)
{
    this->title = newTitle;
}
QString Schedule::getTitle()
{
    return this->title;
}

void Schedule::setDate(QDate newDate)
{
    date = newDate;
}
void Schedule::setDate(QString newDate)
{
    setDate(QDate::fromString(newDate, "dd.MM.yyyy"));
}
QDate Schedule::getDate()
{
    return date;
}
QString Schedule::getDateInString()
{
    return getDate().toString("dd.MM.yyyy");
}

void Schedule::setEvent(Schedule::Event newEvent)
{
    event = newEvent;
}
void Schedule::setEvent(int newEvent)
{
    setEvent(Schedule::Event(newEvent));
}
Schedule::Event Schedule::getEvent()
{
    return event;
}
QString Schedule::getEventInString()
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

QString Schedule::getSportType()
{
    return groups.isEmpty() ?
                ""
              : groups[0].getSportType();
}


void Schedule::setFields(QString title, Event event,
                     QDate date)
{
    setTitle(title);
    setEvent(event);
    setDate(date);
}

QList<QString> Schedule::getFullPattern()
{
    return {"Заголовок", "Событие", "Дата проведения", "Вид спорта"};
}

void Schedule::setSavableProperty(QList<QString> property)
{
    if (property.count() == getSaveblePattern().count())
    {
        setTitle(property.takeFirst());
        setEvent(property.takeFirst().toInt());
        setDate(property.takeFirst());
    }
}

QList<QString> Schedule::getSavebleProperty()
{
    return {getTitle(), QString::number(int(getEvent())), getDateInString()};
}

QList<QString> Schedule::getSaveblePattern()
{
    return {"Заголовок", "Событие", "Дата проведения"};
}

QList<QString> Schedule::getPreviewList()
{
    return {getTitle(), getEventInString(), getDateInString()};
}

QList<QString> Schedule::getPreviewPattern()
{
    return {"Событие", "Вид события", "Дата проведения"};
}

QList<int> Schedule::getGroupsIds()
{
    QList<int> groupsIds;

    for (Group group : groups)
    {
        groupsIds << group.id;
    }

    return groupsIds;
}

QList<QList<QString>>
Schedule::toPreviewTable(QList<Schedule> schedules)
{
    QList<QList<QString>> stringTable;
    for (Schedule sch : schedules)
    {
        stringTable << sch.getPreviewList();
    }

    return stringTable;
}

Schedule Schedule::firstScheduleWithGroup(QList<Schedule> schedules, int group_id)
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


bool operator== (const Schedule &s1, const Schedule &s2)
{
    return (s1.id == s2.id)
            && Schedule(s1).getSavebleProperty() == Schedule(s2).getSavebleProperty()
            && Schedule(s1).getGroupsIds() == Schedule(s2).getGroupsIds();
}

bool operator!= (const Schedule &s1, const Schedule &s2)
{
    return !(s1 == s2);
}

