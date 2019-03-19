#pragma once

#include <QStandardItemModel>
#include <QStackedWidget>

#include "ui/support/group_editor.h"

class GroupsPresenter : public QWidget
{
    Q_OBJECT

    QList<Person> sportsmen;     // объект для хранения детей
    QList<Person> trainers;     // объект для хранения тренеров
    QList<Group> groups;

    QPushButton *createButton;
    RecordsViewer *groupsViewer;
    QStackedWidget *widgetStack;
    GroupEditor *groupEditor;

signals:
    void needSave(Group group);
    void needRemove(int group_id);

public:
    GroupsPresenter(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();
        setUpConnections();
    }

    void setGroupIconPath(QString groupIconPath)
    {
        groupsViewer->setIconPath(groupIconPath);
    }

    void setTrainerIconPath(QString trainerIconPath)
    {
        groupEditor->setTrainerIconPath(trainerIconPath);
    }

    void setSportsmanIconPath(QString sportsmanIconPath)
    {
        groupEditor->setSportsmanIconPath(sportsmanIconPath);
    }

    void updateContent(QList<Person> sportsmen,
                       QList<Person> trainers,
                       QList<Group> groups)
    {
        this->sportsmen = sportsmen;
        this->trainers = trainers;
        this->groups = groups;

        groupsViewer->updateContent(Group::toStringTable(groups),
                            Group::getPattern());

        if (widgetStack->currentIndex() == 1)
        {
            updateRunningEditor(sportsmen, trainers, groups);
        }
    }

private:
    void setUpUi()
    {
        createButton = new QPushButton("+");
        groupsViewer = new RecordsViewer;

        QVBoxLayout *viewerLayout = new QVBoxLayout;
        viewerLayout->addWidget(createButton);
        viewerLayout->addWidget(groupsViewer);

        QWidget *viewerWidget = new QWidget;
        viewerWidget->setLayout(viewerLayout);

        groupEditor = new GroupEditor;

        widgetStack = new QStackedWidget(this);
        widgetStack->addWidget(viewerWidget);
        widgetStack->addWidget(groupEditor);
    }

    void setUpConnections()
    {
        connect(createButton, &QPushButton::clicked, [=] ()
        {
            showEditor();
        });
        connect(groupsViewer, &RecordsViewer::rowIsActivated, [=] (int row)
        {
            showEditor(groups.at(row));
        });

        connect(groupEditor, &GroupEditor::needSave, [=] (Group group)
        {
            showGroups();
            emit needSave(group);
        });
        connect(groupEditor, &GroupEditor::needRemove, [=] (int id)
        {
            showGroups();
            emit needRemove(id);
        });
        connect(groupEditor, &GroupEditor::needExit, [=] ()
        {
            auto inputGroup  = groupEditor->getOldGroup();
            auto outputGroup = groupEditor->getCurrentGroup();

            if (inputGroup != outputGroup)
            {
                auto result = QMessageBox::question(this, " ",
                                "У вас есть несохраненные изменения:\n"
                                "Сохранить?");

                if (result == QMessageBox::Yes)
                {
                    showGroups();
                    emit needSave(outputGroup);
                }
            }
            showGroups();
        });
    }

private slots:
    void showEditor(Group group = Group())
    {
        QList<Person> availableSportsmen = Person::getFreePeople(sportsmen, group.sportsmen);
        availableSportsmen << group.sportsmen;

        groupEditor->updateContent(trainers, availableSportsmen, group);
        widgetStack->setCurrentIndex(1);
    }

    void showGroups()
    {
        groupEditor->updateContent(QList<Person>(), QList<Person>(), Group());
        widgetStack->setCurrentIndex(0);
    }

    void updateRunningEditor(QList<Person> sportsmen,
                      QList<Person> trainers,
                      QList<Group> groups)
    {
        if (groups.contains(groupEditor->getOldGroup())
                || groupEditor->getOldGroup().id == 0)
        {
            groupEditor->updateTrainersView(trainers);
            groupEditor->updateSportsmenView(sportsmen);
        }
        else
        {
            int result = QMessageBox::question(this, " ",
                    "Редактируемая группа была изменена извне:\n"
                    "'" + groupEditor->getCurrentGroup().getInList().join(" ") + "'\n"
                    "Продолжить редактирование?");

            if (result == QMessageBox::Ok)
            {
                groupEditor->updateTrainersView(trainers);
                groupEditor->updateSportsmenView(sportsmen);
            }
            else if (result == QMessageBox::No)
            {
                 showGroups();
            }
        }
    }

public:
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        widgetStack->resize(resizeEvent->size());
    }
};





















