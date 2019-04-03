#include "ui/support/schedule_editor.h"


ScheduleEditor::
ScheduleEditor(Schedule schedule,
               QList<Group> groups,
               QWidget *parent)
    : QWidget(parent)
{
    setUpUi();
    setUpConnections();
    updateContent(schedule, groups);
}

ScheduleEditor::
ScheduleEditor(QWidget *parent)
    : ScheduleEditor(Schedule(), {} , parent) { }

void ScheduleEditor::
setGroupIconPath(QString groupIconPath)
{
    this->groupIconPath = groupIconPath;
    groupsViewer->setIconPath(groupIconPath);
}

void ScheduleEditor::
updateContent(Schedule schedule,
                              QList<Group> allGroups)
{
    this->schedule = schedule;
    this->oldSchedule = schedule;

    updateEditorsView(schedule);
    setPotentialGroupsForSchedule(allGroups);
}

Schedule ScheduleEditor::
getCurrentSchedule()
{
    schedule.setFields(
                titleEdit->text(),
                Schedule::Event(eventEdit->currentIndex() + 1),
                dateEdit->date()
                );

    return schedule;
}

Schedule ScheduleEditor::
getOldSchedule()
{
    return this->oldSchedule;
}

void ScheduleEditor::
setPotentialGroupsForSchedule(QList<Group> allGroups)
{
    this->allGroups = allGroups;
    updateNotEditableView(schedule);
}

void ScheduleEditor::
setUpUi()
{
    // editors
    dateEdit = new QDateTimeEdit;
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("yyyy.MM.dd");

    titleEdit = new QLineEdit;

    eventEdit = new QComboBox;
    eventEdit->addItem("Тренировка");
    eventEdit->addItem("Соревнования");

    sportTypeEdit = new QLineEdit;
    sportTypeEdit->setReadOnly(true);
    sportTypeEdit->setToolTip("Спорт расписания определяется группами в нем");

    QFormLayout *editors = new QFormLayout;
    auto titlesList = Schedule::getFullPattern();
    editors->addRow(titlesList.takeFirst(), titleEdit);
    editors->addRow(titlesList.takeFirst(), eventEdit);
    editors->addRow(titlesList.takeFirst(), dateEdit);
    editors->addRow(titlesList.takeFirst(), sportTypeEdit);

    // group viewer
    addGroupButton = new QPushButton("+");
    groupsViewer = new RecordsViewer;
    QVBoxLayout *groupViewerLayout = new QVBoxLayout;
    groupViewerLayout->addWidget(addGroupButton);
    groupViewerLayout->addWidget(groupsViewer);

    // standart buttons
    saveButton = new QPushButton("Сохранить");
    makeDoneButton = new QPushButton("Закрыть ведомость");
    removeButton = new QPushButton("Удалить");
    exitButton = new QPushButton("Выйти");
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(makeDoneButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(exitButton);

    QVBoxLayout *basicLayout = new QVBoxLayout;
    basicLayout->addLayout(editors);
    basicLayout->addWidget(new QLabel("Группы для участия"));
    basicLayout->addLayout(groupViewerLayout);
    basicLayout->addLayout(buttonLayout);

    setLayout(basicLayout);
}

void ScheduleEditor::
setUpConnections()
{
    connect(saveButton, &QPushButton::clicked, [=] ()
    {
        emit needSave(getCurrentSchedule());
    });
    connect(makeDoneButton, &QPushButton::clicked, [=] ()
    {
        emit needMakeDone(getCurrentSchedule());
    });
    connect(removeButton, &QPushButton::clicked, [=] ()
    {
        emit needRemove(schedule.id);
    });
    connect(exitButton, &QPushButton::clicked,
            this, &ScheduleEditor::needExit);

    connect(addGroupButton, &QPushButton::clicked, [=] ()
    {
        QList<Group> groupsToShow;
        for (Group group : this->allGroups)
        {
            if (!schedule.groups.contains(group))
            {
                if (schedule.getSportType() == group.getSportType()
                        || schedule.getSportType().isEmpty())
                {
                    groupsToShow << group;
                }
            }
        }

        int row = RecordChooser::getChoosedRow(
                    Group::toStringTable(groupsToShow), this, "Доступные группы", groupIconPath);

        if (row >= 0)
        {
            schedule.groups << groupsToShow.at(row);
            updateNotEditableView(schedule);
        }
    });
    connect(groupsViewer, &RecordsViewer::rowIsActivated, [=] (int row)
    {
        int result = QMessageBox::question(
                    this, " ", "Убрать группу?");

        if (result != QMessageBox::Yes)
        {
            return;
        }
        schedule.groups.removeAt(row);
        updateNotEditableView(schedule);
    });
}


void ScheduleEditor::
updateNotEditableView(Schedule sch)
{
    sportTypeEdit->setText(sch.getSportType());
    groupsViewer->updateContent(Group::toStringTable(sch.groups),
                                Group::getPreviewPattern());
}


void ScheduleEditor::
updateEditorsView(Schedule sch)
{
    titleEdit->setText(sch.getTitle());

    if (sch.getDateInString().isEmpty())
        dateEdit->setDate(QDate::currentDate());
    else
        dateEdit->setDate(sch.getDate());

    if (sch.getEvent() == Schedule::Event::TRAINING)
    {
        eventEdit->setCurrentIndex(0);
    }
    else if (sch.getEvent() == Schedule::Event::COMPETITION)
    {
        eventEdit->setCurrentIndex(1);
    }
}




















