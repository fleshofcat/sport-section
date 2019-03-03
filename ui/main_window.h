#pragma once

#include <QTextStream>
#include <QDateTime>

#include "ui/people_presenter.h"
#include "ui/groups_presenter.h"
#include "ui/schedule_presenter.h"


// класс MainWindow/ГлавноеОкно является классом-прослойкой
// между пользователем и программой
//
// он показывает пользователю информацию
// принимает и обрабатывает его запросы
class MainWindow : public QWidget
{
    Q_OBJECT

    QTabWidget *tabs;
    PeoplePresenter *sportsmenTab;
    PeoplePresenter *trainersTab;
    GroupsPresenter *groupTab;
    SchedulePresenter *scheduleTab;

    QList<Person> sportsmen;
    QList<Person> trainers;
    QList<Group> groups;
    QList<Schedule> schedules;

signals:
    void saveSportsman(Person pers);
    void removeSportsman(int id);

    void saveTrainer(Person pers);
    void removeTrainer(int id);

    void saveGroup(Group group);
    void removeGroup(int id);

    void saveSchedule(Schedule chedule);
    void removeSchedule(int id);

public:
    // код который будет выполняться при создании объекта от этого класса
    // имеет 1 не обязательный системный параметр
    explicit MainWindow(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();
        setUpConnections();
    }


    // метод с помощью которого данные от бд
    // будут загружаться в данный класс и отображаться пользователю
    void updateContent(QList<Person> sportsmen,
                       QList<Person> trainers,
                       QList<Group> groups,
                       QList<Schedule> schedules)
    {
        this->sportsmen = sportsmen;
        this->trainers = trainers;
        this->groups = groups;
        this->schedules = schedules;

        sportsmenTab->updateContent(sportsmen);
        trainersTab->updateContent(trainers);
        groupTab->updateContent(sportsmen, trainers, groups);
        scheduleTab->showData(schedules, groups);
    }

private:
    void setUpUi()
    {
        this->resize(800, 400);

        sportsmenTab = new PeoplePresenter;
        trainersTab = new PeoplePresenter;
        groupTab = new GroupsPresenter;
        scheduleTab = new SchedulePresenter;

        tabs = new QTabWidget(this);


        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabs->sizePolicy().hasHeightForWidth());
        tabs->setSizePolicy(sizePolicy);
        tabs->setMovable(true);

        tabs->addTab(scheduleTab, QIcon("../record/res/img/schedule.png"),   "Расписания");
        tabs->addTab(groupTab, QIcon("../record/res/img/group.png"),         "Группы");
        tabs->addTab(trainersTab, QIcon("../record/res/img/trainer.png"),    "Тренера");
        tabs->addTab(sportsmenTab, QIcon("../record/res/img/sportsman.png"), "Спортсмены");
    }

    void setUpConnections()
    {
        connect(sportsmenTab, &PeoplePresenter::savePerson, this, &MainWindow::saveSportsman);
        connect(trainersTab, &PeoplePresenter::savePerson, this, &MainWindow::saveTrainer);
        connect(groupTab, &GroupsPresenter::saveGroup, this, &MainWindow::saveGroup);
        connect(scheduleTab, &SchedulePresenter::saveSchedule, this, &MainWindow::saveSchedule);
        connect(scheduleTab, &SchedulePresenter::removeSchedule, this, &MainWindow::removeSchedule);

        connect(sportsmenTab, &PeoplePresenter::removePerson, [=] (int id)
        {
            for (Group group : groups)
            {
                if (group.sportsmen_ids.contains(id))
                {
                    QString groupName = group.getInList().at(0);
                    sportsmenTab->showWarning("Пока этот спортсмен состоит в группе '" +
                                              groupName + "' его нельзя удалить.");
                    return;
                }
            }
             emit removeSportsman(id);
        });

        connect(trainersTab, &PeoplePresenter::removePerson, [=] (int id)
        {
            for (Group group : groups)
            {
                if (group.trainers_ids.contains(id))
                {
                    QString groupName = group.getInList().at(0);
                    trainersTab->showWarning("Пока этот тренер состоит в группе '" +
                                             groupName + "' его нельзя удалить.");
                    return;
                }
            }
             emit removeTrainer(id);
        });

        connect(groupTab, &GroupsPresenter::removeGroup, [=] (int id)
        {
            for (Schedule sch : schedules)
            {
                if (sch.group_ids.contains(id))
                {
                    QString scheduleName = QString::number(schedules.indexOf(sch) + 1);
                    trainersTab->showWarning("Пока эта группа находится в расписании '" +
                                             scheduleName + "' ее нельзя удалить.");
                    return;
                }
            }
             emit removeSchedule(id);
        });
    }

public:
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        tabs->resize(resizeEvent->size());
    }
};


