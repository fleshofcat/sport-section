#pragma once

#include <QToolBox>
#include <QPushButton>

#include "common/schedule.h"
#include "ui/support/sportsmen_result_preview.h"
#include "ui/support/preview_result_calculation.h"

class ScheduleCloser : public QWidget
{
    Q_OBJECT

    Schedule schedule;

    QLabel *scheduleTitle;
    PreviewResultCalculation *resultView;
    SportsmenResultPreview *sportsmenView;

    QPushButton *makeDoneButton;
    QPushButton *exitButton;

signals:
    void needMakeDone(Schedule sched);
    void needExit();

public:
    ScheduleCloser(Schedule sch, QWidget *parent = nullptr);
    ScheduleCloser(QWidget *parent = nullptr);

    void setSchedule(Schedule schedule);

private:
    void setUpUi();
    void setUpConnections();
    QList<Group> computeSportsmenOrder(QList<Person> sportsmen);
};



















