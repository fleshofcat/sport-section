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

    QList<int> previousGroupsOrder;

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

        fillSportsmenLView(groups);
        QList<int> previousGroupsOrder = getSortedOrderByGroupsRange(groups);
        updateGroupsView(previousGroupsOrder, groups);
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
        sportsmenView->setPersIconPath(imagePath);
    }

    QList<Group> getGroups()
    {
        return this->currentGroups;
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
            [=] (QList<Person> sportsmen)
        {
            QList<Group> updatedGroups = computeSportsmenOrder(sportsmen);
            auto groupsOrder = getSortedOrderByGroupsRange(updatedGroups);
            updateGroupsView(groupsOrder, updatedGroups);
        });
    }

    void fillSportsmenLView(QList<Group> groups)
    {
        QList<Person> allSportsmen;

        for (auto group : groups)
        {
            allSportsmen << group.sportsmen;
        }

        sportsmenView->setSportsmen(allSportsmen);
    }

    QList<Group> computeSportsmenOrder(QList<Person> sportsmen)
    {
        QList<Group> currentGroups = oldGroups;

        // convert person position to additional rating
        for (int i = 0; i < sportsmen.count(); i++)
        {
            sportsmen[i].rating += sportsmen.count() / (i + 1);
        }

        // update the sportsmen in the current groups
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

        // in each group increase the trainers rating
        // by difference between odl and new group condition
        for (int g = 0; g < currentGroups.count(); g++)
        {
            int additional = currentGroups[g].getFullSportsmenRating()
                    - oldGroups[g].getFullSportsmenRating();

            currentGroups[g].increaseTrainersRating(additional);
        }

        return currentGroups;
    }

    QList<int> getSortedOrderByGroupsRange(QList<Group> groups)
    {
        // arrange by rating
        QList<QPair<float,int>> groupsRatingIndexes;
        for (int r = 0; r < groups.count(); r++)
        {
            groupsRatingIndexes << QPair<float,int>(groups[r].getGroupRating()
                               - oldGroups[r].getGroupRating(), r);
        }

sort_start:
        for (int i = 0; i < groupsRatingIndexes.count() - 1; i++)
        {
            if (groupsRatingIndexes[i].first < groupsRatingIndexes[i + 1].first)
            {
                groupsRatingIndexes.swap(i, i + 1);
                goto sort_start;
            }
        }

        QList<int> rangeOrderIndexes;
        for (auto ratingIndex : groupsRatingIndexes)
        {
            rangeOrderIndexes << ratingIndex.second;
        }

        return rangeOrderIndexes;
    }

    void updateGroupsView(QList<int> groupsOrder, QList<Group> currentGroups)
    {
        this->currentGroups = currentGroups;

        int currentPageIndex = 0;
        if (groupsView->currentIndex() != -1)
        {
            currentPageIndex = groupsOrder.indexOf(previousGroupsOrder.at(groupsView->currentIndex()));
        }

        this->previousGroupsOrder = groupsOrder;

        while (groupsView->count()) // drop the groupView state
        {
            groupsView->removeItem(0);
        }

        for (int index : groupsOrder)
        {
            QListWidget *peopleView = new QListWidget;
            peopleView->setIconSize(QSize(20, 20));

            for (int p = 0; p < currentGroups.at(index).trainers.count(); p++)
            {
                auto item = getPersonViewItem(currentGroups.at(index).trainers.at(p),
                                          oldGroups.at(index).trainers.at(p),
                                          trainersIconPath);

                peopleView->addItem(item);
            }
            peopleView->addItem("");


            for (int p = 0; p < currentGroups.at(index).sportsmen.count(); p++)
            {
                auto item = getPersonViewItem(currentGroups.at(index).sportsmen.at(p),
                                          oldGroups.at(index).sportsmen.at(p),
                                          sportsmanIconPath);

                peopleView->addItem(item);
            }


            groupsView->addItem(
                        peopleView, QIcon(groupIconPath),
                        currentGroups[index].groupName + " - "
                    + QString::number(double(currentGroups[index].getGroupRating()))
                    + " ( +"
                    + QString::number(double(currentGroups[index].getGroupRating()
                                      - oldGroups[index].getGroupRating()))
                    + ")");
        }

        groupsView->setCurrentIndex(currentPageIndex);
    }

    QListWidgetItem * getPersonViewItem(Person thePersonNow, Person thePersonEarlier, QString iconPath = "")
    {
        if (thePersonNow.id != thePersonEarlier.id)
            return nullptr;

        QString stringPers = thePersonNow.firstName
                     + " " + thePersonNow.secondName
                     + " " + thePersonNow.lastName
                     + " - " + QString::number(double(thePersonNow.rating))
                     + " ( +" + QString::number(double(thePersonNow.rating
                                                - thePersonEarlier.rating)) + ")";

        auto item = new QListWidgetItem(QIcon(iconPath), stringPers);
        item->setData(Qt::UserRole, QVariant(thePersonNow.id));

        return item;
    }

};






























