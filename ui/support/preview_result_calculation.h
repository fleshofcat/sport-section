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

        emit needUpdateShowesSportsmen(Group::getAllSportsmen(groups));

        QList<int> previousGroupsOrder = getSortedOrderByGroupsRange(groups); // TODO test to rm
        updateGroupsView(groups);
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

    QList<Group> getGroups()
    {
        return this->currentGroups;
    }

public slots:
    void updateGroupsView(QList<Group> currentGroups)
    {
        QList<int> groupsOrder = getSortedOrderByGroupsRange(currentGroups);

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

private:
    void setUpUi()
    {
        groupsLabel = new QLabel("--------------------- результаты мероприятия ---------------------");
        groupsView = new QToolBox;
        groupsView->setStyleSheet("QToolBox{ icon-size: 25px; }");

        QVBoxLayout *basicLayout = new QVBoxLayout;
        basicLayout->addWidget(groupsLabel);
        basicLayout->addWidget(groupsView);

        setLayout(basicLayout);
    }

    void setUpConnections()
    {

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






























