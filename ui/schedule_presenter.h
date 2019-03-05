#pragma once

#include <QStackedWidget>

#include <QStandardItemModel>

#include "ui/schedule_editor.h"

class SchedulePresenter : public QWidget
{
    Q_OBJECT
    friend class TestSchedulePresenter;

    QString icon_path;
    QList<Schedule> schedules;
    QList<Group> groups;

    RecordsViewer *scheduleViewer;
    QPushButton *createButton;
    QStackedWidget *widgetStack;
    ScheduleEditor *scheduleEditor;

signals:
    void needSave(Schedule schedule);
    void needRemove(int id);

public:
    SchedulePresenter(QWidget *parent = nullptr)
        : SchedulePresenter("", parent) { }

    SchedulePresenter(QString icon_path, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        this->icon_path = icon_path;
        setUpUi();
        setUpConnections();
    }

    void updateContent(QList<Schedule> schedules, QList<Group> groups)
    {
        this->groups = groups;
        this->schedules = schedules;

        scheduleViewer->updateContent(Schedule::toStringTable(schedules), Schedule::pattern());

        if (widgetStack->currentIndex() == 1)
        {
             updateSchedule(schedules, groups);
        }
    }

private:
    void setUpUi()
    {
        createButton = new QPushButton("+");

        scheduleViewer = new RecordsViewer;
        scheduleViewer->setIconPath(icon_path);

        QVBoxLayout *viewerLayout = new QVBoxLayout;
        viewerLayout->addWidget(createButton);
        viewerLayout->addWidget(scheduleViewer);

        QWidget *viewerWidget = new QWidget;
        viewerWidget->setLayout(viewerLayout);

        scheduleEditor = new ScheduleEditor;

        widgetStack = new QStackedWidget(this);
        widgetStack->addWidget(viewerWidget);
        widgetStack->addWidget(scheduleEditor);
    }

    void setUpConnections()
    {
        connect(createButton, &QPushButton::clicked, [=] ()
        {
            showEditor();
        });
        connect(scheduleViewer, &RecordsViewer::rowIsActivated, [=] (int row)
        {
            showEditor(schedules.at(row));
        });

        connect(scheduleEditor, &ScheduleEditor::needSave, [=] (Schedule sch)
        {
            emit needSave(sch);
            showSchedules();
        });
        connect(scheduleEditor, &ScheduleEditor::needRemove, [=] (int id)
        {
            emit needRemove(id);
            showSchedules();
        });
        connect(scheduleEditor, &ScheduleEditor::needExit,
                this, &SchedulePresenter::showSchedules);
    }

    void updateSchedule(QList<Schedule> schedules, QList<Group> groups)
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
                 showSchedules();
            }
        }
    }



private slots:
    void showEditor(Schedule sched = Schedule())
    {
        scheduleEditor->showData(sched, groups);
        widgetStack->setCurrentIndex(1);
    }

    void showSchedules()
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





















