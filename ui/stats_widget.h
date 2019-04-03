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
    StatsWidget(QWidget *parent = nullptr);

    void updateContent(QList<Group> groups,
                       QList<Person> trainers,
                       QList<Person> sportsmen);

    void setGroupIconPath(QString groupsIconPath = "");
    void setTrainersIconPath(QString trainersIconPath = "");
    void setSportsmenIconPath(QString sportsmenIconPath = "");

private:
    void setUpUi();
};




























