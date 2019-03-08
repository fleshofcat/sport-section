#pragma once

#include <QToolBox>

#include "common/schedule.h"
#include "ui/support/preview_result_calculation.h"

class ScheduleClose : public QWidget
{
    Q_OBJECT

    Schedule schedule;

    PreviewResultCalculation *resultPrewiew;

signals:

public:
    ScheduleClose(Schedule sch, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();
        setSchedule(sch);
    }

    ScheduleClose(QWidget *parent = nullptr)
        : ScheduleClose(Schedule(), parent) {}

    void setSchedule(Schedule schedule)
    {
        this->schedule = schedule;


    }

private:
    void setUpUi()
    {
        resultPrewiew = new PreviewResultCalculation;

        resultPrewiew->setGroupIconPath("../record/res/img/group.png");
        resultPrewiew->setTrainerIconPath("../record/res/img/trainer.png");
        resultPrewiew->setSportsmanIconPath("../record/res/img/sportsman.png");

        QVBoxLayout *basicLayout = new QVBoxLayout;
        basicLayout->addWidget(resultPrewiew);
        setLayout(basicLayout);
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
























