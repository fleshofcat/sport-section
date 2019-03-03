#pragma once

#include <QStackedWidget>

#include "ui/schedule_editor.h"

class SchedulePresenter : public QWidget
{
    Q_OBJECT
    friend class TestSchedulePresenter;

    QList<Schedule> schedules;
    QList<Group> groups;

    QStackedWidget *widgetStack;
    RecordsWidget *schedulesViewer;
    ScheduleEditor *scheduleEditor;

signals:
    void saveSchedule(Schedule schedule);
    void removeSchedule(int id);

public:
    SchedulePresenter(QWidget *parent = nullptr)
        : SchedulePresenter({}, {}, parent) { }

    SchedulePresenter(QList<Schedule> schedules, QList<Group> groups, QWidget *parent = nullptr) : QWidget(parent)
    {
        setUpUi();
        showData(schedules, groups);
        setUpConnections();
    }

    void showData(QList<Schedule> schedules, QList<Group> groups)
    {
        this->groups = groups;
        this->schedules = schedules;

        schedulesViewer->updateData(
                    Schedule::getPattern(),
                    Schedule::schedulesToTable(schedules));

        if (widgetStack->currentIndex() == 1)
        {
            if (schedules.contains(scheduleEditor->oldSchedule()))
            {
                scheduleEditor->updateGroups(groups);
            }
            else
            {
                int result = QMessageBox::question(this, " ",
                        "Данное расписание было изменено извне. "
                        "Продолжить редактирование?");
                if (result == QMessageBox::Ok)
                {
                    scheduleEditor->updateGroups(groups);
                }
                else if (result == QMessageBox::No)
                {
                     setViewerMode();
                }
            }
        }
    }

private:
    void setUpUi()
    {
        schedulesViewer = new RecordsWidget;
        scheduleEditor = new ScheduleEditor;

        widgetStack = new QStackedWidget(this);

        widgetStack->addWidget(schedulesViewer);
        widgetStack->addWidget(scheduleEditor);
    }

    void setUpConnections()
    {
        connect(schedulesViewer, &RecordsWidget::createRecordActivated, [=] ()
        {
            scheduleEditor->showData(Schedule(), groups);
            widgetStack->setCurrentIndex(1);
        });
        connect(schedulesViewer, &RecordsWidget::recordActivated, [=] (int row)
        {
            scheduleEditor->showData(schedules.at(row), groups);
            widgetStack->setCurrentIndex(1);
        });


        connect(scheduleEditor, &ScheduleEditor::needSave, [=] (Schedule sch)
        {
            setViewerMode();
            emit saveSchedule(sch);
        });
        connect(scheduleEditor, &ScheduleEditor::needRemove, [=] (int id)
        {
            setViewerMode();
            emit removeSchedule(id);
        });
        connect(scheduleEditor, &ScheduleEditor::needExit,
                this, &SchedulePresenter::setViewerMode);
    }

private slots:
    void setViewerMode()
    {
        scheduleEditor->showData(Schedule(), {});
        widgetStack->setCurrentIndex(0);
    }

public:
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        widgetStack->resize(resizeEvent->size());
    }
};





















