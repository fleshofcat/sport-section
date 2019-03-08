#pragma once

#include <QToolBox>
#include <QPushButton>

#include "common/schedule.h"
#include "ui/support/preview_result_calculation.h"

class ScheduleClose : public QWidget
{
    Q_OBJECT

    Schedule schedule;

    QLabel *scheduleTitle;
    PreviewResultCalculation *resultView;
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

        makeDoneButton = new QPushButton("Закрыть ведомость");
        exitButton = new QPushButton("Выйти");

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(makeDoneButton);
        buttonLayout->addWidget(exitButton);

        QVBoxLayout *basicLayout = new QVBoxLayout;
        basicLayout->addWidget(scheduleTitle);
        basicLayout->addWidget(resultView);
        basicLayout->addLayout(buttonLayout);

        basicLayout->setAlignment(scheduleTitle, Qt::Alignment(Qt::AlignmentFlag::AlignCenter));

        setLayout(basicLayout);
    }

    void setUpConnections()
    {
        connect(makeDoneButton, &QPushButton::clicked, [=] ()
        {
            schedule.groups = resultView->getGroups();
            emit needMakeDone(schedule);
        });
        connect(exitButton, &QPushButton::clicked, this, &ScheduleClose::needExit);
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
























