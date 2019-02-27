#pragma once

#include "ui/people_tab.h"
#include "ui/groups_tab.h"


// класс MainWindow/ГлавноеОкно является классом-прослойкой
// между пользователем и программой
//
// он показывает пользователю информацию
// принимает и обрабатывает его запросы
class MainWindow : public QWidget
{
    Q_OBJECT    // обязательный макрос

signals:
    void saveSportsmen(Person pers);
    void removeSportsmen(int id);

    void saveTrainer(Person pers);
    void removeTrainer(int id);

    void saveGroup(Group group);
    void removeGroup(int id);

private:
    QTabWidget *tabs;

    PeopleTab *sportsmenTab;
    PeopleTab *trainersTab;
    GroupsTab *groupTab;

public:
    // код который будет выполняться при создании объекта от этого класса
    // имеет 1 не обязательный системный параметр
    explicit MainWindow(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();

        connect(sportsmenTab, &PeopleTab::savePerson, this, &MainWindow::saveSportsmen);
        connect(sportsmenTab, &PeopleTab::removePerson, this, &MainWindow::removeSportsmen);

        connect(trainersTab, &PeopleTab::savePerson, this, &MainWindow::saveTrainer);
        connect(trainersTab, &PeopleTab::removePerson, this, &MainWindow::removeTrainer);

        connect(groupTab, &GroupsTab::saveGroup, this, &MainWindow::saveGroup);
        connect(groupTab, &GroupsTab::removeGroup, this, &MainWindow::removeGroup);
    }


    // метод с помощью которого данные от бд
    // будут загружаться в данный класс и отображаться пользователю
    void updateContent(QList<Person> sportsmen,
                       QList<Person> trainers,
                       QList<Group> groups)
    {
        sportsmenTab->updateContent(sportsmen);
        trainersTab->updateContent(trainers);
        groupTab->updateContent(sportsmen, trainers, groups);
    }


private:
    void setUpUi()
    {
        this->resize(800, 400);

        sportsmenTab = new PeopleTab;
        trainersTab = new PeopleTab;
        groupTab = new GroupsTab;

        tabs = new QTabWidget(this);


        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabs->sizePolicy().hasHeightForWidth());
        tabs->setSizePolicy(sizePolicy);


        tabs->addTab(sportsmenTab, "Спортсмены");
        tabs->addTab(trainersTab, "Тренера");
        tabs->addTab(groupTab, "Группы");
    }

public:
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        tabs->resize(resizeEvent->size());
    }
};


