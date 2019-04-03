#include "ui/support/schedule_closer.h"


ScheduleCloser::ScheduleCloser(Schedule sch, QWidget *parent)
    : QWidget(parent)
{
    setUpUi();
    setUpConnections();
    setSchedule(sch);
}

ScheduleCloser::ScheduleCloser(QWidget *parent)
    : ScheduleCloser(Schedule(), parent) {}

void ScheduleCloser::setSchedule(Schedule schedule)
{
    this->schedule = schedule;

    scheduleTitle->setText(schedule.getPreviewList().join(" "));
    resultView->setGroups(schedule.groups);
}


void ScheduleCloser::setUpUi()
{
    scheduleTitle = new QLabel();

    resultView = new PreviewResultCalculation;

    resultView->setGroupIconPath("../record/res/img/group.png");
    resultView->setTrainerIconPath("../record/res/img/trainer.png");
    resultView->setSportsmanIconPath("../record/res/img/sportsman.png");

    sportsmenView = new SportsmenResultPreview;
    sportsmenView->setSportsmanIconPath("../record/res/img/sportsman.png");
    sportsmenView->setSportsmenDescription ("Передвиньте спортсменов в нужном порядке");

    QHBoxLayout *resultEditLayout = new QHBoxLayout;
    resultEditLayout->addWidget(resultView);
    resultEditLayout->addWidget(sportsmenView);

    makeDoneButton = new QPushButton("Закрыть ведомость");
    exitButton = new QPushButton("Выйти");

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(makeDoneButton);
    buttonLayout->addWidget(exitButton);

    QVBoxLayout *basicLayout = new QVBoxLayout;
    basicLayout->addWidget(scheduleTitle);
    basicLayout->addLayout(resultEditLayout);
    basicLayout->addLayout(buttonLayout);

    basicLayout->setAlignment(scheduleTitle, Qt::Alignment(Qt::AlignmentFlag::AlignCenter));

    setLayout(basicLayout);
}

void ScheduleCloser::setUpConnections()
{
    connect(resultView, &PreviewResultCalculation::needUpdateShowesSportsmen,
            sportsmenView, &SportsmenResultPreview::setSportsmen);

    connect(sportsmenView, &SportsmenResultPreview::sportsmenOrderChanged,
        [=] (QList<Person> sportsmen)
    {
        QList<Group> updatedGroups = computeSportsmenOrder(sportsmen);
        resultView->updateGroupsView(updatedGroups);
    });

    connect(makeDoneButton, &QPushButton::clicked, [=] ()
    {
        schedule.groups = resultView->getGroups();
        emit needMakeDone(schedule);
    });
    connect(exitButton, &QPushButton::clicked, this, &ScheduleCloser::needExit);
}

QList<Group> ScheduleCloser::
computeSportsmenOrder(QList<Person> sportsmen)
{
    QList<Group> oldGroups = schedule.groups;
    QList<Group> currentGroups = schedule.groups;

    // convert person position to additional rating
    if (schedule.getEvent() == Schedule::Event::COMPETITION)
    {
        for (int i = 0; i < sportsmen.count(); i++)
        {
            sportsmen[i].increaseRating(sportsmen.count() / (i + 1));
            sportsmen[i].increaseEventNumber(1);
        }
    }
    else
    {
        for (int i = 0; i < sportsmen.count(); i++)
        {
            sportsmen[i].increaseRating(1);
            sportsmen[i].increaseEventNumber(1);
        }
    }

    // update the sportsmen in the current groups
    for (int g = 0; g < currentGroups.count(); g++)
    {
        currentGroups[g].increaseEventNumber(1);
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
        int additionalRating = currentGroups[g].getAccumSportsmenRating()
                - oldGroups[g].getAccumSportsmenRating();

        for (int t = 0; t < currentGroups[g].trainers.count(); t++)
        {
            currentGroups[g].trainers[t].increaseRating(additionalRating);
            currentGroups[g].trainers[t].increaseEventNumber(1);
        }
    }

    return currentGroups;
}
