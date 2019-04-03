#include "main_window.h"

Ui::Ui(QWidget *parent)
    : QWidget(parent)
{
    setUpUi();
    setUpConnections();
}

void Ui::updateSportsmen(QList<Person> sportsmen)
{
    this->sportsmen = sportsmen;
    sportsmenTab->updateContent(sportsmen);
    updateStats();
}

void Ui::updateTrainers(QList<Person> trainers)
{
    this->trainers = trainers;
    trainersTab->updateContent(trainers);
    updateStats();
}

void Ui::updateGroups(QList<Group> groups)
{
    this->groups = groups;
    groupTab->updateContent(sportsmen, trainers, groups);
    updateStats();
}

void Ui::updateSchedules(QList<Schedule> schedules)
{
    this->schedules = schedules;
    scheduleTab->updateContent(schedules, groups);
    updateStats();
}

void Ui::updateClosedSchedules(QList<Schedule> closedSchedules)
{
    this->closedSchedules = schedules;
    scheduleTab->updateClosedSchedule(closedSchedules);
    updateStats();
}

void Ui::updateStats()
{
    statsTab->updateContent(groups, trainers, sportsmen);
}


void Ui::setUpUi()
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

void Ui::setUpConnections()
{
    connect(sportsmenTab, &PeoplePresenter::needSave, this, &Ui::needSaveSportsman);
    connect(sportsmenTab, &PeoplePresenter::needRemove, this, &Ui::needRemoveSportsman);
    connect(sportsmenTab, &PeoplePresenter::needEdit, [=] (Person pers)
    {
        Group groupWithSportsman = Group::getFirstGroupWithSportsmen(groups, pers.id);
        sportsmenTab->editPerson(pers, groupWithSportsman.getGroupName());
    });

    connect(trainersTab, &PeoplePresenter::needSave, this, &Ui::needSaveTrainer);
    connect(trainersTab, &PeoplePresenter::needRemove, this, &Ui::needRemoveTrainer);
    connect(trainersTab, &PeoplePresenter::needEdit, [=] (Person pers)
    {
       Group groupWithTrainer = Group::getFirstGroupWithTrainer(groups, pers.id);
       trainersTab->editPerson(pers, groupWithTrainer.getGroupName());
    });

    connect(groupTab, &GroupsPresenter::needSave, this, &Ui::needSaveGroup);
    connect(groupTab, &GroupsPresenter::needRemove, this, &Ui::needRemoveGroup);
    connect(groupTab, &GroupsPresenter::needEdit, [=] (Group group)
    {
        Schedule scheduleWithGroup = Schedule::firstScheduleWithGroup(schedules, group.id);
        groupTab->editGroup(group, scheduleWithGroup.getTitle());
    });

    connect(scheduleTab, &SchedulePresenter::needSave, this, &Ui::needSaveSchedule);
    connect(scheduleTab, &SchedulePresenter::needRemove, this, &Ui::needRemoveSchedule);
    connect(scheduleTab, &SchedulePresenter::needMakeDone, this, &Ui::needMakeDoneSchedule);
}

void Ui::resizeEvent(QResizeEvent *resizeEvent)
{
    tabs->resize(resizeEvent->size());
}




