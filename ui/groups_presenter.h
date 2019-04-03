#pragma once

#include <QStandardItemModel>
#include <QStackedWidget>

#include "ui/support/group_editor.h"

class GroupsPresenter : public QWidget
{
    Q_OBJECT

    QList<Person> sportsmen;
    QList<Person> trainers;
    QList<Group> groups;

    QPushButton *createButton;
    RecordsViewer *groupsViewer;
    QStackedWidget *widgetStack;
    GroupEditor *groupEditor;

signals:
    void needSave(Group group);
    void needRemove(int group_id);
    void needEdit(Group group);

public:
    explicit GroupsPresenter(QWidget *parent = nullptr);

    void updateContent(QList<Person> sportsmen,
                       QList<Person> trainers,
                       QList<Group> groups);

    void editGroup(Group group, QString groupsScheduleName);

    void setGroupIconPath(QString groupIconPath);
    void setTrainerIconPath(QString trainerIconPath);
    void setSportsmanIconPath(QString sportsmanIconPath);

    void resizeEvent(QResizeEvent *resizeEvent);
private:
    void setUpUi();
    void setUpConnections();

    void showEditor(Group group = Group(),
                    QString groupsScheduleName = "");

    void showGroupsPreview();

    void updateRunningEditor(QList<Person> sportsmen,
                             QList<Person> trainers,
                             QList<Group> groups);
};





















