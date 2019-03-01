#pragma once

#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

#include "common/group.h"
#include "common/person.h"

#include "ui/widgets/editors_box.h"
#include "ui/widgets/records_widget.h"
#include "ui/widgets/record_chooser.h"

class GroupEditor : public QWidget
{
    Q_OBJECT

    friend class TestGroupEditor;

    QList<Person> sportsmen;
    QList<Person> trainers;
    Group group;

    QPushButton *saveButton = new QPushButton("Сохранить");
    QPushButton *removeButton = new QPushButton("Удалить");
    QPushButton *exitButton = new QPushButton("Выйти");

    EditorsBox *editorsBox;

    RecordsWidget *trainersViewer;
    RecordsWidget *sportsmenViewer;

signals:
    void saveIsRequred(Group group);
    void removeIsRequred(int id);
    void exitIsRequred();

public:
    GroupEditor(QWidget *parent = nullptr)
        : GroupEditor({}, {}, Group(), parent) { }

    GroupEditor(QList<Person> trainers,
                QList<Person> sportsmen,
                Group group,
                QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();

        connect(saveButton, &QPushButton::clicked, this, &GroupEditor::on_saveGroup);
        connect(removeButton, &QPushButton::clicked, this, &GroupEditor::on_removeGroup);
        connect(exitButton, &QPushButton::clicked, this, &GroupEditor::exitIsRequred);

        connect(sportsmenViewer, &RecordsWidget::createRecordActivate, this, &GroupEditor::on_addSportsmen);
        connect(sportsmenViewer, &RecordsWidget::recordActivated, this, &GroupEditor::on_removeSportsmen);

        connect(trainersViewer, &RecordsWidget::createRecordActivate, this, &GroupEditor::on_addTrainer);
        connect(trainersViewer, &RecordsWidget::recordActivated, this, &GroupEditor::on_removeTrainer);

        updateContent(trainers, sportsmen, group);
    }

    void updateContent(QList<Person> trainers,
                       QList<Person> sportsmen,
                       Group group = Group())
    {
        this->group = group;

        editorsBox->updateContent(Group::getPattern(), group.getInList());
        updateWhenRunning(trainers, sportsmen);
    }

    void updateWhenRunning(QList<Person> trainers,
                           QList<Person> sportsmen)
    {
        this->trainers = trainers;
        this->sportsmen = sportsmen;

        group.dropFakeIds(Person::getIds(sportsmen),
                                Person::getIds(trainers));

        updateViewer(trainersViewer, trainers, group.trainers_ids);
        updateViewer(sportsmenViewer, sportsmen, group.sportsmen_ids);
    }


private:
    void setUpUi()
    {
        editorsBox = new EditorsBox;

        trainersViewer = new RecordsWidget;
        sportsmenViewer = new RecordsWidget;


        QVBoxLayout *trainersViewerLayout = new QVBoxLayout;
        trainersViewerLayout->addWidget(new QLabel("Тренеры"));
        trainersViewerLayout->addWidget(trainersViewer);

        QVBoxLayout *sportsmenViewerLayout = new QVBoxLayout;
        sportsmenViewerLayout->addWidget(new QLabel("Спортсмены"));
        sportsmenViewerLayout->addWidget(sportsmenViewer);


        QHBoxLayout *peopleViewerLayout = new QHBoxLayout;
        peopleViewerLayout->addItem(trainersViewerLayout);
        peopleViewerLayout->addItem(sportsmenViewerLayout);


        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(removeButton);
        buttonLayout->addWidget(exitButton);


        QVBoxLayout *basicLayout = new QVBoxLayout;
        basicLayout->addWidget(editorsBox);
        basicLayout->addLayout(peopleViewerLayout);
        basicLayout->addItem(buttonLayout);

        setLayout(basicLayout);
    }

    void updateViewer(RecordsWidget *peopleViewer,
                      QList<Person> people, QList<int> ids)
    {
        QList<QList<QString>> strindTable;
        for (int id : ids)
        {
            for (Person pers : people)
            {
                if (pers.id == id)
                {
                    strindTable << pers.getInList();
                }
            }
        }

        peopleViewer->updateData(Person::getPattern(), strindTable);
    }

private slots:
    void on_addSportsmen()
    {
        QList<Person> peopleToShow;
        for (Person pers : sportsmen)
        {
            if (!group.sportsmen_ids.contains(pers.id)
                    /*&& group.sportType == pers.sportType*/)
            {
                peopleToShow << pers;
            }
        }

        int row = RecordChooser::getChoosedRow(
                    Person::personListToStringTable(peopleToShow), this);

        if (row >= 0)
        {
            int id = peopleToShow.at(row).id;
            group.sportsmen_ids << id;

            updateWhenRunning(trainers, sportsmen);
        }
    }

    void on_removeSportsmen(int row)
    {
        int result = QMessageBox::question(this, " ",
                    "Убрать спортсмена из группы?");

        if (result == QMessageBox::Yes)
        {
            group.sportsmen_ids.removeAt(row);
            updateWhenRunning(trainers, sportsmen);
        }
    }

    void on_addTrainer()
    {
        QList<Person> peopleToShow;
        for (Person pers : trainers)
        {
            if (!group.trainers_ids.contains(pers.id)
                    /*&& group.sportType == pers.sportType*/)
            {
                peopleToShow << pers;
            }
        }

        int row = RecordChooser::getChoosedRow(
                    Person::personListToStringTable(peopleToShow), this);

        if (row >= 0)
        {
            int id = peopleToShow.at(row).id;
            group.trainers_ids << id;

            updateWhenRunning(trainers, sportsmen);
        }
    }

    void on_removeTrainer(int row)
    {
        int result = QMessageBox::question(this, " ",
                    "Убрать тренера из группы?");

        if (result == QMessageBox::Yes)
        {
            group.trainers_ids.removeAt(row);
            updateWhenRunning(trainers, sportsmen);
        }
    }


    void on_saveGroup()
    {
        group.setInList(editorsBox->getInList());
        emit saveIsRequred(group);
    }

    void on_removeGroup()
    {
        emit removeIsRequred(this->group.id);
    }
};




























