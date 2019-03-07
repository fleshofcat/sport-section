#pragma once

#include <QToolBox>

#include "common/schedule.h"
#include "ui/support/preview_result_calculation.h"

class ScheduleResult : public QWidget
{
    Q_OBJECT

    Schedule schedule;

    QToolBox *toolBox;
    QListWidget *listView;

signals:

public:
    ScheduleResult(Schedule sch, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        this->schedule = sch;

        setUpUi();

        QStringList strs = {"one", "two", "three"};

        for (QString str : strs)
        {
//            listView.
//            toolBox->addItem(new QLabel(str), QIcon("../record/res/img/schedule.png"), str);
        }

//        toolBox->set

//        toolBox->setM
    }

    ScheduleResult(QWidget *parent = nullptr)
        : ScheduleResult(Schedule(), parent) {}

private:
    void setUpUi()
    {
//        toolBox = new QToolBox(this);
        listView = new QListWidget(this);
        QHBoxLayout *layout = new QHBoxLayout;
//        layout->addWidget(toolBox);
        layout->addWidget(listView);
        setLayout(layout);
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
























