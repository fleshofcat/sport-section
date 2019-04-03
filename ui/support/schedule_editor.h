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
    Schedule oldSchedule;
    QList<Group> allGroups;

    QPushButton *saveButton;
    QPushButton *makeDoneButton;
    QPushButton *removeButton;
    QPushButton *exitButton;

    // editors
    QLineEdit *titleEdit;
    QComboBox *eventEdit;
    QDateTimeEdit *dateEdit;

    // not editable
    QLineEdit *sportTypeEdit;

    QPushButton *addGroupButton;
    RecordsViewer *groupsViewer;

signals:
    void needMakeDone(Schedule schedule);
    void needSave(Schedule schedule);
    void needRemove(int id);
    void needExit();

public:
    ScheduleEditor(Schedule schedule, QList<Group> groups,
                   QWidget *parent = nullptr);
    ScheduleEditor(QWidget *parent = nullptr);

    void updateContent(Schedule schedule, QList<Group> allGroups);
    void setPotentialGroupsForSchedule(QList<Group> allGroups);

    Schedule getCurrentSchedule();
    Schedule getOldSchedule();

    void setGroupIconPath(QString groupIconPath);

private:
    void setUpUi();
    void setUpConnections();

    void updateNotEditableView(Schedule sch);
    void updateEditorsView(Schedule sch);
};

























