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
    SchedulePresenter(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();
        setUpConnections();
    }


    void setScheduleIconPath(QString scheduleIconPath)
    {
        showOpenScheduleButton->setIcon(QIcon(scheduleIconPath));
        scheduleViewer->setIconPath(scheduleIconPath);
    }

    void setClosedScheduleIconPath(QString closedScheduleIconPath)
    {
        showClosedScheduleButton->setIcon(QIcon(closedScheduleIconPath));
        closedScheduleViewer->setIconPath(closedScheduleIconPath);
    }

    void setGroupIconPath(QString groupIconPath)
    {
        scheduleEditor->setGroupIconPath(groupIconPath);
    }

    void updateContent(QList<Schedule> schedules, QList<Schedule> closedShedules, QList<Group> groups)
    {
        this->groups = groups;
        this->schedules = schedules;

        scheduleViewer->updateContent(Schedule::toStringTable(schedules), Schedule::getPreviewPattern());
        closedScheduleViewer->updateContent(Schedule::toStringTable(closedShedules), Schedule::getPreviewPattern());

        if (widgetStack->currentIndex() == 1)
        {
             updateRunningEditor(schedules, groups);
        }
    }

private:
    void setUpUi()
    {
        createButton = new QPushButton("+");
        showClosedScheduleButton = new QPushButton("Показать завершенные");
        showOpenScheduleButton = new QPushButton("Показать не завершенные");

        scheduleViewer = new RecordsViewer;

        QVBoxLayout *viewerLayout = new QVBoxLayout;
        viewerLayout->addWidget(createButton);
        viewerLayout->addWidget(scheduleViewer);
        viewerLayout->addWidget(showClosedScheduleButton);

        QWidget *viewerWidget = new QWidget;
        viewerWidget->setLayout(viewerLayout);


        QPushButton *closedScheduleTitle = new QPushButton("Прошедшие события");
        closedScheduleTitle->setFlat(true);
        closedScheduleViewer = new RecordsViewer;

        QVBoxLayout *closedScheduleLayout = new QVBoxLayout;
        closedScheduleLayout->addWidget(closedScheduleTitle);
        closedScheduleLayout->addWidget(closedScheduleViewer);
        closedScheduleLayout->addWidget(showOpenScheduleButton);
        closedScheduleLayout->setAlignment(closedScheduleTitle,
                        Qt::Alignment(Qt::AlignmentFlag::AlignCenter));

        QWidget *viewClosedScheduleWidget = new QWidget;
        viewClosedScheduleWidget->setLayout(closedScheduleLayout);

        scheduleEditor = new ScheduleEditor;

        scheduleCloser = new ScheduleCloser;

        widgetStack = new QStackedWidget(this);
        widgetStack->addWidget(viewerWidget);
        widgetStack->addWidget(scheduleEditor);
        widgetStack->addWidget(scheduleCloser);
        widgetStack->addWidget(viewClosedScheduleWidget);
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
            showSchedules();
            emit needSave(sch);
        });
        connect(scheduleEditor, &ScheduleEditor::needMakeDone, [=] (Schedule sch)
        {
            showScheduleCloser(sch);
        });
        connect(scheduleEditor, &ScheduleEditor::needRemove, [=] (int id)
        {
            showSchedules();
            emit needRemove(id);
        });
        connect(scheduleEditor, &ScheduleEditor::needExit, [=] ()
        {
            auto oldSchedule  = scheduleEditor->getOldSchedule();
            auto currentSchedule = scheduleEditor->getCurrentSchedule();

            if (oldSchedule != currentSchedule)
            {
                auto result = QMessageBox::question(this, " ",
                                "У вас есть несохраненные изменения:\n"
                                "Сохранить?");

                if (result == QMessageBox::Yes)
                {
                    showSchedules();
                    emit needSave(currentSchedule);
                }
            }
            showSchedules();
        });

        connect(scheduleCloser, &ScheduleCloser::needMakeDone, [=] (Schedule sch)
        {
            showSchedules();
            emit needMakeDone(sch);
        });


        connect(showClosedScheduleButton, &QPushButton::clicked, this, &SchedulePresenter::showDoneSchedules);
        connect(showOpenScheduleButton,   &QPushButton::clicked, this, &SchedulePresenter::showSchedules);
        connect(scheduleCloser,        &ScheduleCloser::needExit, this, &SchedulePresenter::showSchedules);
    }

    void updateRunningEditor(QList<Schedule> schedules, QList<Group> groups)
    {
        if (schedules.contains(scheduleEditor->getOldSchedule())
                || scheduleEditor->getOldSchedule().id == 0)
        {
            scheduleEditor->updateGroups(getAvailableGroups(groups));
        }
        else
        {
            int result = QMessageBox::question(this, " ",
                    "Редактируемое расписание было изменено извне:\n"
                    "'" + scheduleEditor->getCurrentSchedule().getPreviewList().join(" ") + "'\n"
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
    void showSchedules()
    {
        widgetStack->setCurrentIndex(0);
        scheduleEditor->showData(Schedule(), {});
        scheduleCloser->setSchedule(Schedule());
    }

    void showEditor(Schedule sched = Schedule())
    {
        widgetStack->setCurrentIndex(1);
        scheduleEditor->showData(sched, getAvailableGroups(groups));
    }

    void showScheduleCloser(Schedule sched)
    {
        widgetStack->setCurrentIndex(2);
         scheduleCloser->setSchedule(sched);
    }

    void showDoneSchedules()
    {
        widgetStack->setCurrentIndex(3);
    }

    QList<Group> getAvailableGroups(QList<Group> groups)
    {
        QList<Group> availableGroups;
        for (Group group : groups)
        {
            if (group.trainers.count() > 0)
            {
                availableGroups << group;
            }
        }
        return availableGroups;
    }

public:
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        widgetStack->resize(resizeEvent->size());
    }
};





















