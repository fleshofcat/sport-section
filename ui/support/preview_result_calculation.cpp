#include "ui/support/preview_result_calculation.h"


PreviewResultCalculation::PreviewResultCalculation(QWidget *parent)
    : QWidget(parent)
{
    setUpUi();
}

void PreviewResultCalculation::setGroups(QList<Group> groups)
{
    this->oldGroups = groups;
    this->currentGroups = groups;
    this->previousGroupsOrder = {};

    emit needUpdateShowesSportsmen(Group::getAllSportsmen(groups));

    updateGroupsView(groups);
}

void PreviewResultCalculation::setGroupIconPath(QString imagePath)
{
    this->groupIconPath = imagePath;
}

void PreviewResultCalculation::setTrainerIconPath(QString imagePath)
{
    this->trainersIconPath = imagePath;
}

void PreviewResultCalculation::setSportsmanIconPath(QString imagePath)
{
    this->sportsmanIconPath = imagePath;
}

QList<Group> PreviewResultCalculation::getGroups()
{
    return this->currentGroups;
}


void PreviewResultCalculation::updateGroupsView(QList<Group> currentGroups)
{
    this->currentGroups = currentGroups;

    QList<int> groupsOrder = getSortedOrderByGroupsRange(currentGroups);

    int currentPageIndex = 0;
    if (groupsView->currentIndex() != -1 && !previousGroupsOrder.isEmpty())
    {
        currentPageIndex = groupsOrder.indexOf(previousGroupsOrder.at(groupsView->currentIndex()));
    }

    this->previousGroupsOrder = groupsOrder;

    dropGroupsView();

//        QApplication::processEvents(); // TODO it works

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
                    currentGroups[index].getGroupName() + " - "
                + QString::number(double(currentGroups[index].getRating()))
                + " ( +"
                + QString::number(double(currentGroups[index].getRating()
                                  - oldGroups[index].getRating()))
                + ")");
    }

    groupsView->setCurrentIndex(currentPageIndex);
}


void PreviewResultCalculation::setUpUi()
{
    groupsLabel = new QLabel("--------------------- результаты мероприятия ---------------------");
    groupsView = new QToolBox;
    groupsView->setStyleSheet("QToolBox{ icon-size: 25px; }");

    QVBoxLayout *basicLayout = new QVBoxLayout;
    basicLayout->addWidget(groupsLabel);
    basicLayout->addWidget(groupsView);

    setLayout(basicLayout);
}

QList<int> PreviewResultCalculation::getSortedOrderByGroupsRange(QList<Group> groups)
{
    // arrange by rating
    QList<QPair<double,int>> groupsRatingIndexes;
    for (int r = 0; r < groups.count(); r++)
    {
        groupsRatingIndexes << QPair<double,int>(groups[r].getRating()
                           - oldGroups[r].getRating(), r);
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

void PreviewResultCalculation::dropGroupsView()
{
    delete groupsView;               // smells very bad
    groupsView = new QToolBox;
    groupsView->setStyleSheet("QToolBox{ icon-size: 25px; }");
    layout()->addWidget(groupsView);
}

QListWidgetItem *PreviewResultCalculation::getPersonViewItem(Person thePersonNow, Person thePersonEarlier, QString iconPath)
{
    if (thePersonNow.id != thePersonEarlier.id)
        return nullptr;

    QString stringPers = thePersonNow.getFirstName()
                 + " " + thePersonNow.getSecondName()
                 + " " + thePersonNow.getLastName()
                 + " - " + QString::number(double(thePersonNow.getRating()))
                 + " ( +" + QString::number(double(thePersonNow.getRating()
                                            - thePersonEarlier.getRating())) + ")";

    auto item = new QListWidgetItem(QIcon(iconPath), stringPers);
    item->setData(Qt::UserRole, QVariant(thePersonNow.id));

    return item;
}
