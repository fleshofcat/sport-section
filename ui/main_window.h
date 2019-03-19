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

    QString sportsmanIconPath = "../record/res/img/sportsman.png";
    QString trainerIconPath = "../record/res/img/trainer.png";
    QString groupIconPath = "../record/res/img/group.png";
    QString scheduleIconPath = "../record/res/img/schedule.png";
    QString closedScheduleIconPath = "../record/res/img/closed_schedule.png";

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

        tabs = new QTabWidget(this);

        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabs->sizePolicy().hasHeightForWidth());
        tabs->setSizePolicy(sizePolicy);
        tabs->setMovable(true);

        tabs->addTab(scheduleTab, QIcon(scheduleIconPath),   "Расписания");
        tabs->addTab(groupTab, QIcon(groupIconPath),         "Группы");
        tabs->addTab(trainersTab, QIcon(trainerIconPath),    "Тренеры");
        tabs->addTab(sportsmenTab, QIcon(sportsmanIconPath), "Спортсмены");
    }

    void setUpConnections()
    {
        connect(sportsmenTab, &PeoplePresenter::savePerson, this, &MainWindow::needSaveSportsman);
        connect(trainersTab, &PeoplePresenter::savePerson, this, &MainWindow::needSaveTrainer);
        connect(groupTab, &GroupsPresenter::needSave, this, &MainWindow::needSaveGroup);

        connect(sportsmenTab, &PeoplePresenter::removePerson, [=] (int id)
        {
            for (Group group : groups)
            {
                if (group.getSportsmenIds().contains(id))
                {
                    QString groupName = group.getInList().at(0);
                    sportsmenTab->showWarning("Пока этот спортсмен состоит в группе '" +
                                              groupName + "' его нельзя удалить.");
                    return;
                }
            }
             emit needRemoveSportsman(id);
        });

        connect(trainersTab, &PeoplePresenter::removePerson, [=] (int id)
        {
            for (Group group : groups)
            {
                if (group.getTrainersIds().contains(id))
                {
                    QString groupName = group.getInList().at(0);
                    trainersTab->showWarning("Пока этот тренер состоит в группе '" +
                                             groupName + "' его нельзя удалить.");
                    return;
                }
            }
             emit needRemoveTrainer(id);
        });

        connect(groupTab, &GroupsPresenter::needRemove, [=] (int id)
        {
            for (Schedule sch : schedules)
            {
                if (sch.getGroupsIds().contains(id))
                {
                    QString scheduleName = QString::number(schedules.indexOf(sch) + 1);
                    trainersTab->showWarning("Пока эта группа находится в расписании '" +
                                             scheduleName + "' ее нельзя удалить.");
                    return;
                }
            }
             emit needRemoveGroup(id);
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


