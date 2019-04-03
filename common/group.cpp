#include "common/group.h"


Group::Group(QString groupName)
{
    setGroupName(groupName);
}
Group::Group(int id) : id(id) { }
Group::Group() {}

void Group::setGroupName(QString newGroupName)
{
    groupName = newGroupName;
}
QString Group::getGroupName()
{
    return groupName;
}

void Group::setEventsNumber(int newNumber)
{
    eventsNumber = newNumber;
}
int Group::getEventsNumber()
{
    return eventsNumber;
}
void Group::increaseEventNumber(int increaseValue)
{
    setEventsNumber(getEventsNumber() + increaseValue);
}

QList<QString> Group::getPreviewProperty()
{
    return {getGroupName(), getSportType()};
}

QList<QString> Group::getPreviewPattern()
{
    return {"Группа", "Спорт"};
}


void Group::setSaveableProperty(QList<QString> property)
{
    if (property.count() == getSaveableProperty().count())
    {
        setGroupName(property.takeFirst());
        setEventsNumber(property.takeFirst().toInt());
    }
}

QList<QString> Group::getSaveableProperty()
{
    return {getGroupName(), QString::number(getEventsNumber())};
}

QList<QString> Group::getSaveablePattern()
{
    return {"Группа", "Мероприятий"};
}


QList<QString> Group::getFullProperty()
{
    return { getGroupName(),
                getSportType(),
                QString::number(getEventsNumber()),
                QString::number(getRating()) };
}

QList<int> Group::getTrainersIds()
{
    QList<int> trainers_ids;

    for (auto trainer : trainers)
    {
        trainers_ids << trainer.id;
    }

    return trainers_ids;
}

QList<int> Group::getSportsmenIds()
{
    QList<int> sportsmen_ids;

    for (auto trainer : sportsmen)
    {
        sportsmen_ids << trainer.id;
    }

    return sportsmen_ids;
}

QString Group::getSportType()
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

double Group::getRating()
{
    if (!sportsmen.isEmpty())
    {
        int sportsmenRating = getAccumSportsmenRating();

        return double(sportsmenRating) / double(sportsmen.count());;
    }
    return 0;
}

int Group::getAccumSportsmenRating()
{
    return Person::getAccumRating(sportsmen);
}

void Group::updateSportsman(Person pers)
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

QList<QList<QVariant>>
Group::toStatsTable(QList<Group> groups)
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

QList<QString> Group::getStatsPattern()
{
    auto ret = Group::getPreviewPattern();
    ret << "Рейтинг";
    ret << "Мероприятий";
    return ret;
}

QList<QList<QString>>
Group::toStringTable(QList<Group> groups)
{
    QList<QList<QString>> stringTable;
    for (Group group : groups)
    {
        stringTable << group.getPreviewProperty();
    }

    return stringTable;
}

QList<Person> Group::getAllSportsmen(QList<Group> groups)
{
    QList<Person> allSportsmen;

    for (auto group : groups)
    {
        allSportsmen << group.sportsmen;
    }

    return allSportsmen;
}

QList<Person>
Group::getFreeSportsmen(QList<Group> allGroups, QList<Person> allSportsmen)
{
    for (Group group : allGroups)
    {
        allSportsmen = Person::getFreePeople(
                    allSportsmen, group.sportsmen);
    }
    return allSportsmen;
}

Group Group::getFirstGroupWithSportsmen(
        QList<Group> groups, int sportsmen_id)
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

Group Group::getFirstGroupWithTrainer(
        QList<Group> groups, int trainer_id)
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

bool operator== (const Group &g1, const Group &g2)
{
    return (g1.id == g2.id)
            && Group(g1).getFullProperty() == Group(g2).getFullProperty()
            && Group(g1).trainers == Group(g2).trainers
            && Group(g1).sportsmen == Group(g2).sportsmen;
}

bool operator!= (const Group &g1, const Group &g2)
{
    return !(g1 == g2);
}






