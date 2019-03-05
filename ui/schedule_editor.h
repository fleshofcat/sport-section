#pragma once

#include <QLabel>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QComboBox>
#include <QDateTimeEdit>

#include "common/schedule.h"
#include "common/group.h"

#include "ui/widgets/records_viewer.h"
#include "ui/widgets/record_chooser.h"

class ScheduleEditor : public QWidget
{
    Q_OBJECT
    friend class TestScheduleEditor;

    Schedule schedule;
    Schedule old_schedule;
    QList<Group> allGroups;

    QPushButton *saveButton;
    QPushButton *removeButton;
    QPushButton *exitButton;

    QComboBox *eventEdit;
    QDateTimeEdit *dateEdit;
    QLineEdit *sportTypeEdit;


    QPushButton *addGroupButton;
    RecordsViewer *groupsViewer;

signals:
    void needSave(Schedule schedule);
    void needRemove(int id);
    void needExit();

public:
    ScheduleEditor(Schedule schedule,
                   QList<Group> groups,
                   QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();
        setUpConnections();
        showData(schedule, groups);
    }

    ScheduleEditor(QWidget *parent = nullptr)
        : ScheduleEditor(Schedule(), {} , parent) { }

    void showData(Schedule schedule,
                  QList<Group> allGroups)
    {
        this->schedule = schedule;
        this->old_schedule = schedule;

        setEditors(schedule);
        updateGroups(allGroups);
    }

    Schedule currentSchedule()
    {
        QList<QString> scheduleInList;
        scheduleInList << QString::number(eventEdit->currentIndex());
        scheduleInList << dateEdit->text();
        scheduleInList << sportTypeEdit->text();

        schedule.setInList(scheduleInList);
        return schedule;
    }

    Schedule oldSchedule()
    {
        return this->old_schedule;
    }

    void updateGroups(QList<Group> allGroups)
    {
        this->allGroups = allGroups;

        updateGroupsViewer();
    }

private:
    void setUpUi()
    {
        // editors
        dateEdit = new QDateTimeEdit;
        dateEdit->setCalendarPopup(true);
        dateEdit->setDisplayFormat("yyyy.MM.dd");

        eventEdit = new QComboBox;
        eventEdit->addItem("Сбор");
        eventEdit->addItem("Тренировка");
        eventEdit->addItem("Соревнования");

        sportTypeEdit = new QLineEdit;

        QFormLayout *editors = new QFormLayout;
        editors->addRow("Дата проведения", dateEdit);
        editors->addRow("Вид события", eventEdit);
        editors->addRow("Вид спорта", sportTypeEdit);

        // group viewer
        addGroupButton = new QPushButton("+");
        groupsViewer = new RecordsViewer;
        QVBoxLayout *groupViewerLayout = new QVBoxLayout;
        groupViewerLayout->addWidget(addGroupButton);
        groupViewerLayout->addWidget(groupsViewer);

        // standart buttons
        saveButton = new QPushButton("Сохранить");
        removeButton = new QPushButton("Удалить");
        exitButton = new QPushButton("Выйти");
        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(removeButton);
        buttonLayout->addWidget(exitButton);

        QVBoxLayout *basicLayout = new QVBoxLayout;
        basicLayout->addLayout(editors);
        basicLayout->addWidget(new QLabel("Группы для участия"));
        basicLayout->addLayout(groupViewerLayout);
        basicLayout->addLayout(buttonLayout);

        setLayout(basicLayout);
    }

    void setUpConnections()
    {
        connect(saveButton, &QPushButton::clicked, [=] ()
        {
            emit needSave(currentSchedule());
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
                    groupsToShow << group;
                }
            }

            int row = RecordChooser::getChoosedRow(
                        Group::toStringTable(groupsToShow), this);

            if (row >= 0)
            {
                schedule.groups << groupsToShow.at(row);
                updateGroupsViewer();
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
            updateGroupsViewer();
        });
    }

    void updateGroupsViewer()
    {
        groupsViewer->updateContent(Group::toStringTable(schedule.groups),
                                    Group::pattern());
    }

    void setEditors(Schedule sch)
    {
        if (sch.date.isEmpty())
            dateEdit->setDate(QDate::currentDate());
        else
            dateEdit->setDate(QDate::fromString(sch.date));

        eventEdit->setCurrentIndex(int(sch.event));
        sportTypeEdit->setText(sch.sportType);
    }
};

























