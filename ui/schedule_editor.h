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
    QList<Group> groups;

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
                  QList<Group> groups)
    {
        this->schedule = schedule;
        this->old_schedule = schedule;

        setEditors(schedule);
        updateGroups(groups);
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

    void updateGroups(QList<Group> groups)
    {
        this->groups = groups;
        showValidGroups(groups, schedule.group_ids);
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
            for (Group group : this->groups)
            {
                if (schedule.group_ids.contains(group.id))
                {
                    groupsToShow << group;
                }
            }

            int row = RecordChooser::getChoosedRow(
                        Group::toStringTable(groupsToShow), this);

            if (row >= 0)
            {
                int id = groupsToShow.at(row).id;
                schedule.group_ids << id;

                showValidGroups(groups, schedule.group_ids);
            }
        });
        connect(groupsViewer, &RecordsViewer::rowIsActivated, [=] (int row)
        {
            int result = QMessageBox::question(
                        this, " ", "Убрать группу?");

            if (result == QMessageBox::Yes)
            {
                schedule.group_ids.removeAt(row);
                showValidGroups(groups, schedule.group_ids);
            }
        });
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

    void showValidGroups(QList<Group> groups, QList<int> valid_ids)
    {
        QList<Group> validGroups;

        for (Group group : groups)
        {
            if (valid_ids.contains(group.id))
            {
                validGroups << group;
            }
        }

        groupsViewer->updateContent(
                    Group::toStringTable(validGroups),
                    Group::pattern());
    }
};

























