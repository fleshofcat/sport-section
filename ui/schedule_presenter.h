#pragma once

#include <QStackedWidget>

#include <QStandardItemModel>

#include "ui/support/schedule_editor.h"
#include "ui/support/schedule_closer.h"

class SchedulePresenter : public QWidget
{
    Q_OBJECT
    friend class TestSchedulePresenter;

    QList<Schedule> schedules;
    QList<Group> groups;

    QStackedWidget *widgetStack;
    QPushButton *createButton;

    QPushButton *showClosedScheduleButton;
    QPushButton *showOpenScheduleButton;

    RecordsViewer *scheduleViewer;
    RecordsViewer *closedScheduleViewer;

    ScheduleEditor *scheduleEditor;
    ScheduleCloser *scheduleCloser;

signals:
    void needMakeDone(Schedule schedule);
    void needSave(Schedule schedule);
    void needRemove(int id);

public:
    explicit
    SchedulePresenter(QWidget *parent = nullptr);

    void updateContent(QList<Schedule> schedules, QList<Group> groups);
    void updateClosedSchedule(QList<Schedule> closedShedules);

    void editSchedule(Schedule sched = Schedule());
    void closeSchedule(Schedule sched);

    void setScheduleIconPath(QString scheduleIconPath);
    void setClosedScheduleIconPath(QString closedScheduleIconPath);
    void setGroupIconPath(QString groupIconPath);

    void resizeEvent(QResizeEvent *resizeEvent);

private:
    void setUpUi();
    void setUpConnections();

    void updateRunningEditor(QList<Schedule> schedules, QList<Group> groups);

private slots:
    void showSchedules();
    void showEditor();
    void showScheduleCloser();
    void showDoneSchedules();

    QList<Group> getAvailableGroups(QList<Group> groups);
};





















