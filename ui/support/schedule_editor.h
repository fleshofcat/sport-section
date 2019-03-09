#pragma once

#include <QLabel>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QComboBox>
#include <QDateTimeEdit>

#include "common/schedule.h"

#include "ui/widgets/records_viewer.h"
#include "ui/widgets/record_chooser.h"

class ScheduleEditor : public QWidget
{
    Q_OBJECT
    friend class TestScheduleEditor;

    QString groupIconPath;

    Schedule schedule;
    Schedule old_schedule;
    QList<Group> allGroups;

    QPushButton *saveButton;
    QPushButton *makeDoneButton;
    QPushButton *removeButton;
    QPushButton *exitButton;

    QLineEdit *titleEdit;
    QComboBox *eventEdit;
    QDateTimeEdit *dateEdit;
    QLineEdit *sportTypeEdit;


    QPushButton *addGroupButton;
    RecordsViewer *groupsViewer;

signals:
    void needMakeDone(Schedule schedule);
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

    void setGroupIconPath(QString groupIconPath)
    {
        this->groupIconPath = groupIconPath;
        groupsViewer->setIconPath(groupIconPath);
    }

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
        scheduleInList << titleEdit->text();
        scheduleInList << QString::number(eventEdit->currentIndex() + 1);
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

        titleEdit = new QLineEdit;

        eventEdit = new QComboBox;
        eventEdit->addItem("Тренировка");
        eventEdit->addItem("Соревнования");

        sportTypeEdit = new QLineEdit;

        QFormLayout *editors = new QFormLayout;
        auto titlesList = Schedule::getEditPattern();

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

    void setUpConnections()
    {
        connect(saveButton, &QPushButton::clicked, [=] ()
        {
            emit needSave(currentSchedule());
        });
        connect(makeDoneButton, &QPushButton::clicked, [=] ()
        {
            emit needMakeDone(currentSchedule());
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
                        Group::toStringTable(groupsToShow), this, "Доступные группы", groupIconPath);

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
                                    Group::getPattern());
    }

    void setEditors(Schedule sch)
    {
        titleEdit->setText(sch.title);

        if (sch.date.isEmpty())
            dateEdit->setDate(QDate::currentDate());
        else
            dateEdit->setDate(QDate::fromString(sch.date));

        if (sch.getEventNumber() == Schedule::Event::TRAINING)
        {
            eventEdit->setCurrentIndex(0);
        }
        else if (sch.getEventNumber() == Schedule::Event::COMPETITION)
        {
            eventEdit->setCurrentIndex(1);
        }

        sportTypeEdit->setText(sch.sportType);
    }
};

























