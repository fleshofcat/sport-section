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
class Ui : public QWidget
{
    Q_OBJECT

    QString sportsmanIconPath = "../sport-section/res/img/sportsman.png";
    QString trainerIconPath =   "../sport-section/res/img/trainer.png";
    QString groupIconPath =     "../sport-section/res/img/group.png";
    QString scheduleIconPath =  "../sport-section/res/img/schedule.png";
    QString closedScheduleIconPath = "../sport-section/res/img/closed_schedule.png";
    QString statsIconPath =     "../sport-section/res/img/stats.png";

    QTabWidget          *tabs;
    PeoplePresenter     *sportsmenTab;
    PeoplePresenter     *trainersTab;
    GroupsPresenter     *groupTab;
    SchedulePresenter   *scheduleTab;
    StatsWidget         *statsTab;

    QList<Person>   sportsmen;
    QList<Person>   trainers;
    QList<Group>    groups;
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
    explicit Ui(QWidget *parent = nullptr);

    // метод с помощью которого данные от бд
    // будут загружаться в данный класс и отображаться пользователю
    void updateContent(QList<Person> sportsmen,
                       QList<Person> trainers,
                       QList<Group> groups,
                       QList<Schedule> schedules,
                       QList<Schedule> closedSchedules);

    // TODO make updateTrainers(trainers);
    //           updateSportsmen(sportsmen);
    //           upd...

    void resizeEvent(QResizeEvent *resizeEvent);

private:
    void setUpUi();
    void setUpConnections();
};

















