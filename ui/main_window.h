#pragma once

#include "ui/people_editor.h"
#include "ui/groups_editor.h"


// класс MainWindow/ГлавноеОкно является классом-прослойкой
// между пользователем и программой
//
// он показывает пользователю информацию
// принимает и обрабатывает его запросы
class MainWindow : public QWidget
{
    Q_OBJECT

    QTabWidget *tabs;
    PeoplePresentor *sportsmenTab;
    PeoplePresentor *trainersTab;
    GroupsEditor *groupTab;

    QList<Person> sportsmen;
    QList<Person> trainers;
    QList<Group> groups;

signals:
    void saveSportsman(Person pers);
    void removeSportsman(int id);

    void saveTrainer(Person pers);
    void removeTrainer(int id);

    void saveGroup(Group group);
    void removeGroup(int id);

public:
    // код который будет выполняться при создании объекта от этого класса
    // имеет 1 не обязательный системный параметр
    explicit MainWindow(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();

        connect(sportsmenTab, &PeoplePresentor::savePerson, this, &MainWindow::saveSportsman);
        connect(sportsmenTab, &PeoplePresentor::removePerson, this, &MainWindow::on_removeSportsman);

        connect(trainersTab, &PeoplePresentor::savePerson, this, &MainWindow::saveTrainer);
        connect(trainersTab, &PeoplePresentor::removePerson, this, &MainWindow::on_removeTrainer);

        connect(groupTab, &GroupsEditor::saveGroup, this, &MainWindow::saveGroup);
        connect(groupTab, &GroupsEditor::removeGroup, this, &MainWindow::removeGroup);
    }


    // метод с помощью которого данные от бд
    // будут загружаться в данный класс и отображаться пользователю
    void updateContent(QList<Person> sportsmen,
                       QList<Person> trainers,
                       QList<Group> groups)
    {
        this->sportsmen = sportsmen;
        this->trainers = trainers;
        this->groups = groups;

        sportsmenTab->updateContent(sportsmen);
        trainersTab->updateContent(trainers);
        groupTab->updateContent(sportsmen, trainers, groups);
    }

private slots:
    void on_removeSportsman(int id)
    {
        for (Group group : groups)
        {
            if (group.sportsmen_ids.contains(id))
            {
                QString groupName = group.getInList().at(0);
                sportsmenTab->showWarning("Пока этот спортсмен состоит в группе '" +
                                          groupName + "' его нельзя удалить.");
                return;
            }
        }
         emit removeSportsman(id);
    }

    void on_removeTrainer(int id)
    {
        for (Group group : groups)
        {
            if (group.trainers_ids.contains(id))
            {
                QString groupName = group.getInList().at(0);
                trainersTab->showWarning("Пока этот тренер состоит в группе '" +
                                         groupName + "' его нельзя удалить.");
                return;
            }
        }
         emit removeTrainer(id);
    }

private:
    void setUpUi()
    {
        this->resize(800, 400);

        sportsmenTab = new PeoplePresentor;
        trainersTab = new PeoplePresentor;
        groupTab = new GroupsEditor;

        tabs = new QTabWidget(this);


        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabs->sizePolicy().hasHeightForWidth());
        tabs->setSizePolicy(sizePolicy);
        tabs->setMovable(true);

        tabs->addTab(groupTab, "Группы");
        tabs->addTab(trainersTab, "Тренера");
        tabs->addTab(sportsmenTab, "Спортсмены");
    }

public:
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        tabs->resize(resizeEvent->size());
    }
};


