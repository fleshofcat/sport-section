#pragma once

#include <QTextStream>
#include <QDateTime>

#include "ui/people_presenter.h"
#include "ui/groups_presenter.h"
#include "ui/schedule_presenter.h"
#include "ui/stats_widget.h"


// класс MainWindow/ГлавноеОкно является классом-прослойкой
// между пользователем и программой
//
// он показывает пользователю информацию
// принимает и обрабатывает его запросы
class MainWindow : public QWidget
{
    Q_OBJECT

    QString sportsmanIconPath = "../sport-section/res/img/sportsman.png";
    QString trainerIconPath = "../sport-section/res/img/trainer.png";
    QString groupIconPath = "../sport-section/res/img/group.png";
    QString scheduleIconPath = "../sport-section/res/img/schedule.png";
    QString closedScheduleIconPath = "../sport-section/res/img/closed_schedule.png";
    QString statsIconPath = "../sport-section/res/img/stats.png";

    QTabWidget *tabs;
    PeoplePresenter *sportsmenTab;
    PeoplePresenter *trainersTab;
    GroupsPresenter *groupTab;
    SchedulePresenter *scheduleTab;
    StatsWidget *statsTab;

    QList<Person> sportsmen;
    QList<Person> trainers;
    QList<Group> groups;
    QList<Schedule> schedules;

signals:
    void needSaveSportsman(Person pers);
    void needRemoveSportsman(int id);

    void needSaveTrainer(Person pers);
    void needRemoveTrainer(int id);

    void needSaveGroup(Group group);
    void needRemoveGroup(int id);

    void needSaveSchedule(Schedule chedule);
    void needRemoveSchedule(int id);

    void needMakeDoneSchedule(Schedule chedule);

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
                       QList<Schedule> schedules,
                       QList<Schedule> closedSchedules)
    {
        this->sportsmen = sportsmen;
        this->trainers = trainers;
        this->groups = groups;
        this->schedules = schedules;

        sportsmenTab->updateContent(sportsmen);
        trainersTab->updateContent(trainers);
        groupTab->updateContent(sportsmen, trainers, groups);
        scheduleTab->updateContent(schedules, closedSchedules, groups);
        statsTab->updateContent(groups, trainers, sportsmen);
    }

private:
    void setUpUi()
    {
        setWindowTitle(" ");

        this->resize(800, 400);

        sportsmenTab = new PeoplePresenter(sportsmanIconPath);
        trainersTab = new PeoplePresenter(trainerIconPath);

        groupTab = new GroupsPresenter;
        groupTab->setGroupIconPath(groupIconPath);
        groupTab->setTrainerIconPath(trainerIconPath);
        groupTab->setSportsmanIconPath(sportsmanIconPath);

        scheduleTab = new SchedulePresenter;
        scheduleTab->setScheduleIconPath(scheduleIconPath);
        scheduleTab->setClosedScheduleIconPath(closedScheduleIconPath);
        scheduleTab->setGroupIconPath(groupIconPath);

        statsTab = new StatsWidget;
        statsTab->setGroupIconPath(groupIconPath);
        statsTab->setTrainersIconPath(trainerIconPath);
        statsTab->setSportsmenIconPath(sportsmanIconPath);

        tabs = new QTabWidget(this);

        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabs->sizePolicy().hasHeightForWidth());
        tabs->setSizePolicy(sizePolicy);
        tabs->setMovable(true);

        tabs->addTab(scheduleTab,  QIcon(scheduleIconPath),  "Расписания");
        tabs->addTab(groupTab,     QIcon(groupIconPath),     "Группы");
        tabs->addTab(trainersTab,  QIcon(trainerIconPath),   "Тренеры");
        tabs->addTab(sportsmenTab, QIcon(sportsmanIconPath), "Спортсмены");
        tabs->addTab(statsTab,     QIcon(statsIconPath),     "Статистика");
    }

    void setUpConnections()
    {
        connect(sportsmenTab, &PeoplePresenter::needSave, this, &MainWindow::needSaveSportsman);
        connect(sportsmenTab, &PeoplePresenter::needRemove, this, &MainWindow::needRemoveSportsman);
        connect(sportsmenTab, &PeoplePresenter::needEdit, [=] (Person pers)
        {
            Group groupWithSportsman = Group::firstGroupWithSportsmen(groups, pers.id);
            sportsmenTab->editPerson(pers, groupWithSportsman.groupName);
        });

        connect(trainersTab, &PeoplePresenter::needSave, this, &MainWindow::needSaveTrainer);
        connect(trainersTab, &PeoplePresenter::needRemove, this, &MainWindow::needRemoveTrainer);
        connect(trainersTab, &PeoplePresenter::needEdit, [=] (Person pers)
        {
           Group groupWithTrainer = Group::firstGroupWithTrainer(groups, pers.id);
           trainersTab->editPerson(pers, groupWithTrainer.groupName);
        });

        connect(groupTab, &GroupsPresenter::needSave, this, &MainWindow::needSaveGroup);
        connect(groupTab, &GroupsPresenter::needRemove, this, &MainWindow::needRemoveGroup);
        connect(groupTab, &GroupsPresenter::needEdit, [=] (Group group)
        {
            Schedule scheduleWithGroup = Schedule::firstScheduleWithGroup(schedules, group.id);
            groupTab->editGroup(group, scheduleWithGroup.title);
        });

        connect(scheduleTab, &SchedulePresenter::needSave, this, &MainWindow::needSaveSchedule);
        connect(scheduleTab, &SchedulePresenter::needRemove, this, &MainWindow::needRemoveSchedule);
        connect(scheduleTab, &SchedulePresenter::needMakeDone, this, &MainWindow::needMakeDoneSchedule);
    }

public:
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        tabs->resize(resizeEvent->size());
    }
};


