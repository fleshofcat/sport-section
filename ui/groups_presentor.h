#pragma once

#include <QStackedWidget>

#include "ui/group_editor.h"

class GroupsPresentor : public QWidget
{
    Q_OBJECT

    QList<Person> sportsmen;     // объект для хранения детей
    QList<Person> trainers;     // объект для хранения тренеров
    QList<Group> groups;

    QStackedWidget *widgetStack;
    RecordsWidget *recordsViewer;
    GroupEditor *groupEditor;

signals:
    void saveGroup(Group group);
    void removeGroup(int group_id);

public:
    GroupsPresentor(QWidget *parent = nullptr) : QWidget(parent)
    {
        setUpUi();
    }


    void updateContent(QList<Person> sportsmen,
                       QList<Person> trainers,
                       QList<Group> groups)
    {
        this->sportsmen = sportsmen;
        this->trainers = trainers;
        this->groups = groups;

        recordsViewer->updateData(
                    Group::getPattern(),
                    Group::groupListToStringTable(groups));

        if (widgetStack->currentIndex() == 1)
        {
            groupEditor->updateWhenRunning(trainers, sportsmen);
        }
    }

private:
    void setUpUi()
    {
        recordsViewer = new RecordsWidget;
        groupEditor = new GroupEditor;

        widgetStack = new QStackedWidget(this);

        widgetStack->addWidget(recordsViewer);
        widgetStack->addWidget(groupEditor);


        connect(recordsViewer, &RecordsWidget::createRecordActivate,
                this, &GroupsPresentor::on_createRecord);

        connect(recordsViewer, &RecordsWidget::recordActivated,
                this, &GroupsPresentor::on_editRecord);



        connect(groupEditor, &GroupEditor::saveIsRequred,
                this, &GroupsPresentor::on_saveGroup);

        connect(groupEditor, &GroupEditor::removeIsRequred,
                this, &GroupsPresentor::on_removeGroup);

        connect(groupEditor, &GroupEditor::exitIsRequred,
                this, &GroupsPresentor::groupEditorExit);

    }


private slots:
    void on_createRecord()
    {
        groupEditor->updateContent(trainers, sportsmen, Group());
        widgetStack->setCurrentIndex(1);
    }


    void on_editRecord(int row)
    {
        if (row >= 0)
        {
            groupEditor->updateContent(trainers, sportsmen, groups.at(row));
            widgetStack->setCurrentIndex(1);
        }
    }

    void on_saveGroup(Group group)
    {
        groupEditorExit();
        emit saveGroup(group);
    }

    void on_removeGroup(int id)
    {
        groupEditorExit();
        emit removeGroup(id);
    }

    void groupEditorExit()
    {
        groupEditor->updateContent(QList<Person>(), QList<Person>(), Group());
        widgetStack->setCurrentIndex(0);
    }

public:
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        widgetStack->resize(resizeEvent->size());
    }
};





















