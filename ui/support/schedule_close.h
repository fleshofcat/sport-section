#pragma once

#include <QToolBox>
#include <QPushButton>

#include "common/schedule.h"
#include "ui/support/sportsmen_view_for_event_result.h"
#include "ui/support/preview_result_calculation.h"

class ScheduleClose : public QWidget
{
    Q_OBJECT

    Schedule schedule;

    QLabel *scheduleTitle;
    PreviewResultCalculation *resultView;
    SportsmenViewForEventResult *sportsmenView;

    QPushButton *makeDoneButton;
    QPushButton *exitButton;

signals:
    void needMakeDone(Schedule sched);
    void needExit();

public:
    ScheduleClose(Schedule sch, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();
        setUpConnections();
        setSchedule(sch);
    }

    ScheduleClose(QWidget *parent = nullptr)
        : ScheduleClose(Schedule(), parent) {}

    void setSchedule(Schedule schedule)
    {
        this->schedule = schedule;

        scheduleTitle->setText(schedule.date);
        resultView->setGroups(schedule.groups);
    }

private:
    void setUpUi()
    {
        scheduleTitle = new QLabel(schedule.date);

        resultView = new PreviewResultCalculation;

        resultView->setGroupIconPath("../record/res/img/group.png");
        resultView->setTrainerIconPath("../record/res/img/trainer.png");
        resultView->setSportsmanIconPath("../record/res/img/sportsman.png");

        sportsmenView = new SportsmenViewForEventResult;
        sportsmenView->setPersIconPath("../record/res/img/sportsman.png");
        sportsmenView->setDescribeLabel("Передвиньте спортсменов в нужном порядке");

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

    void setUpConnections()
    {
        connect(resultView, &PreviewResultCalculation::needUpdateShowesSportsmen,
                sportsmenView, &SportsmenViewForEventResult::setSportsmen);

        connect(sportsmenView, &SportsmenViewForEventResult::sportsmenOrderChanged,
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
        connect(exitButton, &QPushButton::clicked, this, &ScheduleClose::needExit);
    }

    QList<Group> computeSportsmenOrder(QList<Person> sportsmen)
    {
        QList<Group> oldGroups = schedule.groups;
        QList<Group> currentGroups = schedule.groups;

        // convert person position to additional rating
        if (schedule.getEventNumber() == Schedule::Event::COMPETITION)
        {
            for (int i = 0; i < sportsmen.count(); i++)
            {
                sportsmen[i].rating += sportsmen.count() / (i + 1);
                sportsmen[i].eventsNumber += 1;
            }
        }
        else
        {
            for (int i = 0; i < sportsmen.count(); i++)
            {
                sportsmen[i].rating += 1;
                sportsmen[i].eventsNumber += 1;
            }
        }

        // update the sportsmen in the current groups
        for (int g = 0; g < currentGroups.count(); g++)
        {
            currentGroups[g].eventNumber += 1;
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

            for (int t = 0; t < currentGroups[g].trainers.count(); t++)
            {
                currentGroups[g].trainers[t].rating += additional;
                currentGroups[g].trainers[t].eventsNumber += 1;
            }
        }

        return currentGroups;
    }
};

// При хождение на 1 тренировку спортсмену начисляются +1 к опыту и + 1 к мероприятиям
// При хождение на 1 тренировку тренеру начисляются +1 к опыту и + 1 к мероприятиям

// При не прихождении на трениеровку спортсмену -1 опыта, все
// Тренера не учитываются

// При проведении соревнования спортсмену дается +1 к мероприятиям и +1 +/- в зависимости от результатов
// При проведении соревнования тренеру дается +1 к мероприятиям и +1 +/- в зависимости от результатов его команды


// +/-
// {
//




// }
























