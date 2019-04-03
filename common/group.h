#pragma once

#include <QList>
#include "common/person.h"

class Group
{
    QString groupName;
    int eventsNumber = 0;

public:
    int id = 0;

    QList<Person> trainers;
    QList<Person> sportsmen;

    explicit Group(QString groupName);
    explicit Group(int id);
    explicit Group();

    void setGroupName(QString newGroupName);
    QString getGroupName();

    void setEventsNumber(int newNumber);
    int getEventsNumber();
    void increaseEventNumber(int increaseValue);

    QList<QString> getPreviewProperty();
    static QList<QString> getPreviewPattern();


    void setSaveableProperty(QList<QString> property);
    QList<QString> getSaveableProperty();
    static QList<QString> getSaveablePattern();


    QList<QString> getFullProperty();

    QList<int> getTrainersIds();
    QList<int> getSportsmenIds();

    QString getSportType();
    double getRating();
    int getAccumSportsmenRating();

    void updateSportsman(Person pers);

    static QList<QList<QString>> toStringTable(QList<Group> groups);
    static QList<QList<QVariant>> toStatsTable(QList<Group> groups);
    static QList<QString> getStatsPattern();

    static QList<Person> getAllSportsmen(QList<Group> groups);
    static QList<Person> getFreeSportsmen(
            QList<Group> allGroups, QList<Person> allSportsmen);

    static Group getFirstGroupWithSportsmen(
            QList<Group> groups, int sportsmen_id);

    static Group getFirstGroupWithTrainer(
            QList<Group> groups, int trainer_id);

    friend bool operator== (const Group &g1, const Group &g2);
    friend bool operator!= (const Group &g1, const Group &g2);
};

Q_DECLARE_METATYPE(Group)



















