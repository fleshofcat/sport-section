#pragma once

#include <QStandardItemModel>
#include <QStackedWidget>

#include "ui/group_editor.h"

class GroupsPresenter : public QWidget
{
    Q_OBJECT

    QString icon_path;
    QList<Person> sportsmen;     // объект для хранения детей
    QList<Person> trainers;     // объект для хранения тренеров
    QList<Group> groups;

    QPushButton *createButton;
    QTableView *groupsViewer;
    QStackedWidget *widgetStack;
    GroupEditor *groupEditor;

signals:
    void needSave(Group group);
    void needRemove(int group_id);

public:
    GroupsPresenter(QString icon_path, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        this->icon_path = icon_path;
        setUpUi();
        setUpConnections();
    }

    GroupsPresenter(QWidget *parent = nullptr)
        : GroupsPresenter("", parent) { }

    void updateContent(QList<Person> sportsmen,
                       QList<Person> trainers,
                       QList<Group> groups)
    {
        this->sportsmen = sportsmen;
        this->trainers = trainers;
        this->groups = groups;

        updateView(groups);

        if (widgetStack->currentIndex() == 1)
        {
            groupEditor->updateWhenRunning(trainers, sportsmen);
        }
    }

private:
    void setUpUi()
    {
        createButton = new QPushButton("+");

        groupsViewer = new QTableView;
        groupsViewer->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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
            editGroup();
        });
        connect(groupsViewer, &QTableView::clicked, [=] (QModelIndex index)
        {
            editGroup(groups.at(index.row()));
        });

        connect(groupEditor, &GroupEditor::needSave, [=] (Group group)
        {
            emit needSave(group);
            showGroups();
        });
        connect(groupEditor, &GroupEditor::needRemove, [=] (int id)
        {
            emit needRemove(id);
            showGroups();
        });
        connect(groupEditor, &GroupEditor::needExit,
                this, &GroupsPresenter::showGroups);
    }

    void updateView(QList<Group> groups)
    {
        int columns = Group::pattern().count();
        int rows = groups.count();
        auto stringTable = Group::toStringTable(groups);

        QStandardItemModel *model = new QStandardItemModel(rows, columns);

        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < columns; c++)
            {
                QModelIndex index = model->index(r, c);
                model->setData(index, stringTable.at(r).at(c));
            }
            model->setVerticalHeaderItem(r, new QStandardItem(QIcon(icon_path), ""));
        }

        model->setHorizontalHeaderLabels(Group::pattern());
        groupsViewer->setModel(model);
    }

private slots:
    void editGroup(Group group = Group())
    {
        groupEditor->updateContent(trainers, sportsmen, group);
        widgetStack->setCurrentIndex(1);
    }

    void showGroups()
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





















