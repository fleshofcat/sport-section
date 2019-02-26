#pragma once

#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

#include "common/common_objects.h"
#include "ui/widgets/editors_box.h"
#include "ui/widgets/records_widget.h"
#include "ui/widgets/record_chooser.h"

class GroupEditor : public QWidget
{
    Q_OBJECT

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
        editorsBox->updateContent(Group::getPattern(), group.getInList());


        connect(saveButton, &QPushButton::pressed,
                this, &GroupEditor::on_saveGroup);

        connect(removeButton, &QPushButton::pressed,
                this, &GroupEditor::on_removeGroup);

        connect(exitButton, &QPushButton::pressed,
                this, &GroupEditor::exitIsRequred);




        connect(sportsmenViewer, &RecordsWidget::createRecordIsRequred,
                this, &GroupEditor::on_addSportsmen);

        connect(sportsmenViewer, &RecordsWidget::editRecordIsRequred,
                this, &GroupEditor::on_removeSportsmen);

        connect(trainersViewer, &RecordsWidget::createRecordIsRequred,
                this, &GroupEditor::on_addTrainer);

        connect(trainersViewer, &RecordsWidget::editRecordIsRequred,
                this, &GroupEditor::on_removeTrainer);

        updateContent(trainers, sportsmen, group);
    }

    void updateContent(QList<Person> trainers,
                       QList<Person> sportsmen,
                       Group group)
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

        updateViewer(trainersViewer, trainers, group.trainers_ids);
        updateViewer(sportsmenViewer, sportsmen, group.sportsmen_ids);
    }

    void dropContent()
    {
        updateContent(QList<Person>(), QList<Person>(), Group());
    }

private:
    void setUpUi()
    {
        editorsBox = new EditorsBox(Group::getPattern(), group.getInList(), this);


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
        int row = RecordChooser::getChoosedRow(
                    Person::personListToStringTable(sportsmen), this);

        if (row >= 0)
        {
            int id = sportsmen.at(row).id;

            if (!group.sportsmen_ids.contains(id))
            {
                group.sportsmen_ids << id;
            }

            updateWhenRunning(trainers, sportsmen);
        }
    }

    void on_removeSportsmen(int row)
    {
        int result = QMessageBox::question(
                    this, "",
                    "Удалить спортсмена?");

        if (result == QMessageBox::Yes)
        {
            if (row >= 0)
            {
                int id = sportsmen.at(row).id;
                group.sportsmen_ids.removeAll(id);

                updateWhenRunning(trainers, sportsmen);
            }
        }
    }

    void on_addTrainer()
    {
        int row = RecordChooser::getChoosedRow(
                    Person::personListToStringTable(trainers), this);

        if (row >= 0)
        {
            int id = trainers.at(row).id;

            if (!group.trainers_ids.contains(id))
            {
                group.trainers_ids << id;
            }

            updateWhenRunning(trainers, sportsmen);
        }
    }

    void on_removeTrainer(int row)
    {
        int result = QMessageBox::question(
                    this, "",
                    "Удалить тренера?");

        if (result == QMessageBox::Yes)
        {
            if (row >= 0)
            {
                int id = trainers.at(row).id;
                group.trainers_ids.removeAll(id);

                updateWhenRunning(trainers, sportsmen);
            }
        }
    }


    void on_saveGroup()
    {
        auto groupInList = editorsBox->getInList();

        group.setInList(groupInList);

        emit saveIsRequred(group);
    }

    void on_removeGroup()
    {
        emit removeIsRequred(this->group.id);
    }
};




























