#pragma once

#include <QTabWidget>
#include <QVBoxLayout>

#include "common/group.h"
#include "ui/widgets/records_viewer.h"

class StatsWidget : public QWidget
{
    Q_OBJECT

    QTabWidget statsTabs;
    RecordsViewer groupsStats;
    RecordsViewer trainersStats;
    RecordsViewer sportsmenStats;

public:
    StatsWidget(QWidget *parent = nullptr)
        : QWidget (parent)
    {
        setUpUi();
    }

    void
    updateContent(QList<Group> groups,
                  QList<Person> trainers,
                  QList<Person> sportsmen)
    {
        QList<QList<QVariant>> sportStats;
        for (Person pers : sportsmen)
        {
            QList<QVariant> stats;
            for (QString field : pers.getPreviewList())
            {
                stats << QVariant(field);
            }
            stats << QVariant(pers.rating);
            stats << QVariant(pers.eventsNumber);
            stats << QVariant(Group::firstGroupWithSportsmen(
                                  groups, pers.id).groupName);
            sportStats << stats;
        }

        groupsStats.updateContent(Group::getStatsTable(groups), Group::getStatsPattern());
        trainersStats.updateContent(Person::getStatsTable(trainers), Person::getStatsPattern());
        sportsmenStats.updateContent(sportStats, Person::getStatsPattern() << "Группа");
    }

    void setGroupIconPath(QString groupsIconPath = "")
    {
        statsTabs.setTabIcon(0, QIcon(groupsIconPath));
        groupsStats.setIconPath(groupsIconPath);
    }

    void setTrainersIconPath(QString trainersIconPath = "")
    {
        statsTabs.setTabIcon(1, QIcon(trainersIconPath));
        trainersStats.setIconPath(trainersIconPath);
    }

    void setSportsmenIconPath(QString sportsmenIconPath = "")
    {
        statsTabs.setTabIcon(2, QIcon(sportsmenIconPath));
        sportsmenStats.setIconPath(sportsmenIconPath);
    }

private:
    void setUpUi()
    {
        groupsStats.setSortingEnabled(true);
        trainersStats.setSortingEnabled(true);
        sportsmenStats.setSortingEnabled(true);

        statsTabs.addTab(&groupsStats, "Группы");
        statsTabs.addTab(&trainersStats, "Тренеры");
        statsTabs.addTab(&sportsmenStats, "Спортсмены");

        auto layout = new QVBoxLayout;
        layout->addWidget(&statsTabs);
        setLayout(layout);
    }


};




























