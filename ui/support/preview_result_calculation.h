#pragma once

#include <QLabel>
#include <QToolBox>
#include <QListWidget>
#include <QVBoxLayout>

#include "common/group.h"

class PreviewResultCalculation : public QWidget
{
    Q_OBJECT

    QList<Group> oldGroups;
    QList<Group> currentGroups;
    QList<int> previousGroupsOrder;

    QString groupIconPath;
    QString trainersIconPath;
    QString sportsmanIconPath;

    QLabel *groupsLabel;
    QToolBox *groupsView;

signals:
    void needUpdateShowesSportsmen(QList<Person> allSportsmen);

public:
    explicit PreviewResultCalculation(QWidget *parent = nullptr);

    void setGroups(QList<Group> groups);
    QList<Group> getGroups();

    void setGroupIconPath(QString imagePath);
    void setTrainerIconPath(QString imagePath);
    void setSportsmanIconPath(QString imagePath);

public slots:
    void updateGroupsView(QList<Group> currentGroups);

private:
    void setUpUi();
    QList<int> getSortedOrderByGroupsRange(QList<Group> groups);
    void dropGroupsView();
    QListWidgetItem *getPersonViewItem(Person thePersonNow,
                                       Person thePersonEarlier,
                                       QString iconPath = "");
};






























