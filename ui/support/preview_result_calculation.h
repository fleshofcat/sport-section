#pragma once

#include <QToolBox>
#include <QPair>

#include "common/group.h"
#include "ui/support/sportsmen_view_for_event_result.h"

class PreviewResultCalculation : public QWidget
{
    Q_OBJECT

    QList<Group> oldGroups;
    QList<Group> currentGroups;

    QString groupIconPath;
    QString trainersIconPath;
    QString sportsmanIconPath;

    QLabel *groupsLabel;
    QToolBox *groupsView;

    SportsmenViewForEventResult *sportsmenView;

public:
    PreviewResultCalculation(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();
        setUpConnections();
    }

    void setGroups(QList<Group> groups)
    {
        this->oldGroups = groups;
        this->currentGroups = groups;

        fillArrangedSportsmenList(groups);

        for (Group group : groups)
        {
            QListWidget *trainersView = new QListWidget;
            trainersView->setIconSize(QSize(20, 20));

            for (Person pers : group.trainers)
            {
                QString stringPers = pers.firstName
                             + " " + pers.secondName
                             + " " + pers.lastName
                             + " ( " + QString::number(pers.rating)
                             + " +0 )";

                auto item = new QListWidgetItem(QIcon(trainersIconPath), stringPers, trainersView);
                item->setData(Qt::UserRole, QVariant(pers.id));

                trainersView->addItem(item);
            }

            groupsView->addItem(trainersView, QIcon(groupIconPath), group.groupName + " ( "
                               + QString::number(double(group.getGroupRating())) + " +0 )");
        }

    }

    void setGroupIconPath(QString imagePath)
    {
        this->groupIconPath = imagePath;
    }

    void setTrainerIconPath(QString imagePath)
    {
        this->trainersIconPath = imagePath;
    }

    void setSportsmanIconPath(QString imagePath)
    {
        this->sportsmanIconPath = imagePath;
    }

private:
    void setUpUi()
    {
        groupsLabel = new QLabel("--------------------- результаты мероприятия ---------------------");
        groupsView = new QToolBox;
        groupsView->setStyleSheet("QToolBox{ icon-size: 25px; }");

        QVBoxLayout *groupsLayout = new QVBoxLayout;
        groupsLayout->addWidget(groupsLabel);
        groupsLayout->addWidget(groupsView);

        sportsmenView = new SportsmenViewForEventResult;
        sportsmenView->setPersIconPath(sportsmanIconPath);
        sportsmenView->setDescribeLabel("Передвиньте спортсменов в нужном порядке");

        QHBoxLayout *basicLayout = new QHBoxLayout;
        basicLayout->addLayout(groupsLayout);
        basicLayout->addWidget(sportsmenView);

        setLayout(basicLayout);
    }

    void setUpConnections()
    {
        connect(sportsmenView, &SportsmenViewForEventResult::sportsmenOrderChanged,
            this, &PreviewResultCalculation::computeSportsmenResult);
    }

    void fillArrangedSportsmenList(QList<Group> groups)
    {
        QList<Person> allSportsmen;

        for (auto group : groups)
        {
            allSportsmen << group.sportsmen;
        }

        sportsmenView->setSportsmen(allSportsmen);
    }

    void computeSportsmenResult(QList<Person> sportsmen)
    {
        currentGroups = oldGroups;

        for (int i = 0; i < sportsmen.count(); i++)
        {
            sportsmen[i].rating += sportsmen.count() / (i + 1);
        }

        // update the current groups
        for (int g = 0; g < currentGroups.count(); g++)
        {
            for (Person pers : sportsmen)
            {
                if (currentGroups[g].getSportsmenIds().contains(pers.id))
                {
                    currentGroups[g].updateSportsman(pers);
                }
            }
        }

        updateGroupsView();
    }

    void updateGroupsView()
    {
        QList<QPair<float,int>> groupsPos;

        for (int r = 0; r < currentGroups.count(); r++)
        {
            groupsPos << QPair<float,int>(currentGroups[r].getGroupRating()
                               - oldGroups[r].getGroupRating(), r);
        }

sort_start:
        for (int i = 0; i < groupsPos.count() - 1; i++)
        {
            if (groupsPos[i].first < groupsPos[i + 1].first)
            {
                groupsPos.swap(i, i + 1);
                goto sort_start;
            }
        }

        repaintGroupsView(groupsPos);
    }

    void repaintGroupsView(QList<QPair<float,int>> groupsPos)
    {
        while (groupsView->count()) // drop the groupView state
        {
            groupsView->removeItem(0);
        }

        for (auto index : groupsPos)
        {
            QListWidget *trainersView = new QListWidget;
            trainersView->setIconSize(QSize(20, 20));

            for (int p = 0; p < currentGroups.at(index.second).trainers.count(); p++)
            {
                QString stringPers = currentGroups.at(index.second).trainers.at(p).firstName
                             + " " + currentGroups.at(index.second).trainers.at(p).secondName
                             + " " + currentGroups.at(index.second).trainers.at(p).lastName
                             + " ( " + QString::number(currentGroups.at(index.second).trainers.at(p).rating)
                             + " +" + QString::number(currentGroups.at(index.second).trainers.at(p).rating
                                                     - oldGroups.at(index.second).trainers.at(p).rating)
                             + ")";

                auto item = new QListWidgetItem(QIcon(trainersIconPath), stringPers, trainersView);
                item->setData(Qt::UserRole, QVariant(currentGroups.at(index.second).trainers.at(p).id));

                trainersView->addItem(item);
            }


            groupsView->addItem(
                        trainersView, QIcon(groupIconPath),
                        currentGroups[index.second].groupName + " - "
                    + QString::number(double(currentGroups[index.second].getGroupRating()))
                    + " ( +"
                    + QString::number(double(currentGroups[index.second].getGroupRating()
                                      - oldGroups[index.second].getGroupRating()))
                    + ")");
        }
    }

};






























