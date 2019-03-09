#pragma once

#include <QStackedWidget>

#include <QStandardItemModel>

#include "ui/support/schedule_editor.h"
#include "ui/support/schedule_close.h"

class SchedulePresenter : public QWidget
{
    Q_OBJECT
    friend class TestSchedulePresenter;

    QList<Schedule> schedules;
    QList<Schedule> closedSchedules;
    QList<Group> groups;

    QStackedWidget *widgetStack;
    QPushButton *createButton;

    QPushButton *showClosedScheduleButton;
    QPushButton *showOpenScheduleButton;

    RecordsViewer *scheduleViewer;
    RecordsViewer *closedScheduleViewer;
    ScheduleEditor *scheduleEditor;
    ScheduleClose *scheduleCloser;

signals:
    void needMakeDone(Schedule schedule);
    void needRemoveDoneSchedule(int id); // TODO дописать запрос на удаление при щелчке на закрытое расписание (так же в эот момент может придти запрос на восстановление)
    void needSave(Schedule schedule);
    void needRemove(int id);

public:
    SchedulePresenter(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();
        setUpConnections();
    }

    void setIconsPaths(QString scheduleIconPath = "",
                       QString closedScheduleIconPath = "",
                       QString groupIconPath = "")
    {
        if (scheduleIconPath != "")
        {
            showOpenScheduleButton->setIcon(QIcon(scheduleIconPath));
            scheduleViewer->setIconPath(scheduleIconPath);
        }
        if (closedScheduleIconPath != "")
        {
            showClosedScheduleButton->setIcon(QIcon(closedScheduleIconPath));
            closedScheduleViewer->setIconPath(closedScheduleIconPath);
        }
        if (groupIconPath != "")
        {
            scheduleEditor->setGroupIconPath(groupIconPath);
        }

    }

    void updateContent(QList<Schedule> schedules, QList<Group> groups)
    {
        updateContent(schedules, {}, groups);
    }

    void updateContent(QList<Schedule> schedules, QList<Schedule> closedShedules, QList<Group> groups)
    {
        this->groups = groups;
        this->schedules = schedules;
        this->closedSchedules = closedShedules;

        scheduleViewer->updateContent(Schedule::toStringTable(schedules), Schedule::getPreviewPattern());
        closedScheduleViewer->updateContent(Schedule::toStringTable(closedShedules), Schedule::getEditPattern());

        if (widgetStack->currentIndex() == 1)
        {
             updateSchedule(schedules, groups);
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


        QLabel *closedScheduleTitle = new QLabel("Прошедшие события");
        closedScheduleViewer = new RecordsViewer;\

        QVBoxLayout *closedScheduleLayout = new QVBoxLayout;
        closedScheduleLayout->addWidget(closedScheduleTitle);
        closedScheduleLayout->addWidget(closedScheduleViewer);
        closedScheduleLayout->addWidget(showOpenScheduleButton);
        closedScheduleLayout->setAlignment(closedScheduleTitle,
                        Qt::Alignment(Qt::AlignmentFlag::AlignCenter));

        QWidget *viewClosedScheduleWidget = new QWidget;
        viewClosedScheduleWidget->setLayout(closedScheduleLayout);

        scheduleEditor = new ScheduleEditor;

        scheduleCloser = new ScheduleClose;

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
        connect(scheduleEditor, &ScheduleEditor::needExit,
                this, &SchedulePresenter::showSchedules);

        connect(scheduleCloser, &ScheduleClose::needExit,
                this, &SchedulePresenter::showSchedules);
        connect(scheduleCloser, &ScheduleClose::needMakeDone, [=] (Schedule sch)
        {
            showSchedules();
            emit needMakeDone(sch);
        });

        connect(showClosedScheduleButton, &QPushButton::clicked, this, &SchedulePresenter::showDoneSchedules);
        connect(showOpenScheduleButton, &QPushButton::clicked, this, &SchedulePresenter::showSchedules);
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
    void showSchedules()
    {
        scheduleEditor->showData(Schedule(), {});
        scheduleCloser->setSchedule(Schedule());
        widgetStack->setCurrentIndex(0);
    }

    void showEditor(Schedule sched = Schedule())
    {
        scheduleEditor->showData(sched, groups);
        widgetStack->setCurrentIndex(1);
    }

    void showScheduleCloser(Schedule sched) // TODO
    {
         scheduleCloser->setSchedule(sched);
         widgetStack->setCurrentIndex(2);
    }

    void showDoneSchedules()
    {
        widgetStack->setCurrentIndex(3);
    }

public:
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        widgetStack->resize(resizeEvent->size());
    }
};





















