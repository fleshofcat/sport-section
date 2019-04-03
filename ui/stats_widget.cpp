#include "stats_widget.h"


StatsWidget::
StatsWidget(QWidget *parent)
    : QWidget (parent)
{
    setUpUi();
}

void StatsWidget::
updateContent(QList<Group> groups,
              QList<Person> trainers,
              QList<Person> sportsmen)
{
    QList<QList<QVariant>> sportStats;
    for (Person pers : sportsmen)
    {
        QList<QVariant> stats;
        for (QString field : pers.getPreviewProperty())
        {
            stats << QVariant(field);
        }
        stats << QVariant(pers.getRating());
        stats << QVariant(pers.getEventsNumber());
        stats << QVariant(Group::getFirstGroupWithSportsmen(
                              groups, pers.id).getGroupName());
        sportStats << stats;
    }

    groupsStats.updateContent(Group::toStatsTable(groups), Group::getStatsPattern());
    trainersStats.updateContent(Person::getStatsTable(trainers), Person::getStatsPattern());
    sportsmenStats.updateContent(sportStats, Person::getStatsPattern() << "Группа");
}

void StatsWidget::
setGroupIconPath(QString groupsIconPath)
{
    statsTabs.setTabIcon(0, QIcon(groupsIconPath));
    groupsStats.setIconPath(groupsIconPath);
}

void StatsWidget::
setTrainersIconPath(QString trainersIconPath)
{
    statsTabs.setTabIcon(1, QIcon(trainersIconPath));
    trainersStats.setIconPath(trainersIconPath);
}

void StatsWidget::
setSportsmenIconPath(QString sportsmenIconPath)
{
    statsTabs.setTabIcon(2, QIcon(sportsmenIconPath));
    sportsmenStats.setIconPath(sportsmenIconPath);
}

void StatsWidget::
setUpUi()
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





























