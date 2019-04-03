#include "ui/schedule_presenter.h"

SchedulePresenter::
SchedulePresenter(QWidget *parent)
    : QWidget(parent)
{
    setUpUi();
    setUpConnections();
}


void SchedulePresenter::
setScheduleIconPath(QString scheduleIconPath)
{
    showOpenScheduleButton->setIcon(QIcon(scheduleIconPath));
    scheduleViewer->setIconPath(scheduleIconPath);
}

void SchedulePresenter::
setClosedScheduleIconPath(QString closedScheduleIconPath)
{
    showClosedScheduleButton->setIcon(QIcon(closedScheduleIconPath));
    closedScheduleViewer->setIconPath(closedScheduleIconPath);
}

void SchedulePresenter::
setGroupIconPath(QString groupIconPath)
{
    scheduleEditor->setGroupIconPath(groupIconPath);
}

void SchedulePresenter::
updateContent(QList<Schedule> schedules, QList<Group> groups)
{
    this->groups = groups;
    this->schedules = schedules;

    scheduleViewer->updateContent(Schedule::toPreviewTable(schedules), Schedule::getPreviewPattern());

    if (widgetStack->currentIndex() == 1)
    {
         updateRunningEditor(schedules, groups);
    }
}

void SchedulePresenter::
updateClosedSchedule(QList<Schedule> closedShedules)
{
    closedScheduleViewer->updateContent(
                Schedule::toPreviewTable(closedShedules),
                Schedule::getPreviewPattern());
}

void SchedulePresenter::editSchedule(Schedule sched)
{
    scheduleEditor->updateContent(sched, getAvailableGroups(groups));
    showEditor();
}

void SchedulePresenter::
setUpUi()
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

void SchedulePresenter::
setUpConnections()
{
    connect(createButton, &QPushButton::clicked, [=] ()
    {
        editSchedule();
    });
    connect(scheduleViewer, &RecordsViewer::rowIsActivated, [=] (int row)
    {
        editSchedule(schedules.at(row));
    });

    connect(scheduleEditor, &ScheduleEditor::needSave, [=] (Schedule sch)
    {
        showSchedules();
        emit needSave(sch);
    });
    connect(scheduleEditor, &ScheduleEditor::needMakeDone, [=] (Schedule sch)
    {
        closeSchedule(sch);
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

void SchedulePresenter::
updateRunningEditor(QList<Schedule> schedules, QList<Group> groups)
{
    if (schedules.contains(scheduleEditor->getOldSchedule())
            || scheduleEditor->getOldSchedule().id == 0)
    {
        scheduleEditor->setPotentialGroupsForSchedule(getAvailableGroups(groups));
    }
    else
    {
        int result = QMessageBox::question(this, " ",
                "Редактируемое расписание было изменено извне:\n"
                "'" + scheduleEditor->getCurrentSchedule().getPreviewList().join(" ") + "'\n"
                "Продолжить редактирование?");

        if (result == QMessageBox::Ok)
        {
            scheduleEditor->setPotentialGroupsForSchedule(groups);
        }
        else if (result == QMessageBox::No)
        {
             showSchedules();
        }
    }
}

void SchedulePresenter::
showSchedules()
{
    widgetStack->setCurrentIndex(0);
    scheduleEditor->updateContent(Schedule(), {});
    scheduleCloser->setSchedule(Schedule());
}

void SchedulePresenter::
showEditor()
{
    widgetStack->setCurrentIndex(1);
}

void SchedulePresenter::
showScheduleCloser()
{
    widgetStack->setCurrentIndex(2);
}

void SchedulePresenter::
closeSchedule(Schedule sched)
{
    showScheduleCloser();
    scheduleCloser->setSchedule(sched);
}

void SchedulePresenter::
showDoneSchedules()
{
    widgetStack->setCurrentIndex(3);
}

QList<Group> SchedulePresenter::
getAvailableGroups(QList<Group> groups)
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

void SchedulePresenter::
resizeEvent(QResizeEvent *resizeEvent)
{
    widgetStack->resize(resizeEvent->size());
}













