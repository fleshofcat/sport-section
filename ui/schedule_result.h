#pragma once

#include "common/schedule.h"
#include <QFormLayout>
#include <QWidget>

class ScheduleResult : public QWidget
{
    Q_OBJECT
public:
    ScheduleResult(Schedule sch, QWidget *parent = nullptr)
        : QWidget(parent)
    {

    }

    ScheduleResult(QWidget *parent = nullptr)
        : ScheduleResult(Schedule(), parent) {}
};





























