#pragma once

#include <QLabel>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

#include "common/group.h"
#include "common/person.h"

#include "ui/widgets/records_widget.h"
#include "ui/widgets/record_chooser.h"

class GroupEditor : public QWidget
{
    Q_OBJECT

    friend class TestGroupEditor;

    // data
    QList<Person> sportsmen;
    QList<Person> trainers;
    Group group;

    // standart buttons
    QPushButton *saveButton;
    QPushButton *removeButton;
    QPushButton *exitButton;

    // editors
    QLineEdit *groupNameField;
    QLineEdit *sportTypeField;

    // people in group
    QPushButton *addTrainerButton;
    QPushButton *addSportsmanButton;
    RecordsViewer *trainersV;
    RecordsViewer *sportsmenV;

signals:
    void needSave(Group group);
    void needRemove(int id);
    void needExit();

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
        setUpConnects();
        updateContent(trainers, sportsmen, group);
    }

    void updateContent(QList<Person> trainers,
                       QList<Person> sportsmen,
                       Group group = Group())
    {
        this->group = group;

        groupNameField->setText(group.groupName);
        sportTypeField->setText(group.sportType);

        updateWhenRunning(trainers, sportsmen);
    }

    void updateWhenRunning(QList<Person> trainers,
                           QList<Person> sportsmen)
    {
        this->trainers = trainers;
        this->sportsmen = sportsmen;

        group.dropFakeIds(Person::getIds(sportsmen),
                                Person::getIds(trainers));

        updateViewer(trainersV, trainers, group.trainers_ids);
        updateViewer(sportsmenV, sportsmen, group.sportsmen_ids);
    }

    Group currentGroup()
    {
        QString groupName = groupNameField->text();
        QString sportType = sportTypeField->text();

        group.setInList({groupName, sportType});
        return group;
    }

private:
    void setUpUi()
    {
        saveButton = new QPushButton("Сохранить");
        removeButton = new QPushButton("Удалить");
        exitButton = new QPushButton("Выйти");

        groupNameField = new QLineEdit;
        sportTypeField = new QLineEdit;

        QFormLayout *editors = new QFormLayout;
        editors->addRow("Имя группы", groupNameField);
        editors->addRow("Вид спорта", sportTypeField);

        addTrainerButton = new QPushButton("+");
        addSportsmanButton = new QPushButton("+");
        trainersV = new RecordsViewer;
        sportsmenV = new RecordsViewer;

        QGridLayout *peopleViever = new QGridLayout;
        peopleViever->addWidget(new QLabel("Тренеры"), 0, 0);
        peopleViever->addWidget(new QLabel("Спортсмены"), 0, 1);
        peopleViever->addWidget(addTrainerButton, 1, 0);
        peopleViever->addWidget(addSportsmanButton, 1, 1);
        peopleViever->addWidget(trainersV, 2, 0);
        peopleViever->addWidget(sportsmenV, 2, 1);


        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(removeButton);
        buttonLayout->addWidget(exitButton);


        QVBoxLayout *basicLayout = new QVBoxLayout;
        basicLayout->addLayout(editors);
        basicLayout->addLayout(peopleViever);
        basicLayout->addLayout(buttonLayout);

        setLayout(basicLayout);
    }

    void setUpConnects()
    {
        connect(saveButton, &QPushButton::clicked, [=] ()
        {
            emit needSave(currentGroup());
        });
        connect(removeButton, &QPushButton::clicked, [=] ()
        {
            emit needRemove(group.id);
        });
        connect(exitButton, &QPushButton::clicked,
                this, &GroupEditor::needExit);

        connect(addSportsmanButton, &QPushButton::clicked, [=] ()
        {
            addPersonToPreview(sportsmen, &group.sportsmen_ids);
        });
        connect(sportsmenV, &RecordsViewer::rowIsActivated, [=] (int row)
        {
            removePersonFromPreview(row, &group.sportsmen_ids, "Удалить спортсмена из группы?");
        });

        connect(addTrainerButton, &QPushButton::clicked, [=] ()
        {
            addPersonToPreview(trainers, &group.trainers_ids);
        });
        connect(trainersV, &RecordsViewer::rowIsActivated, [=] (int row)
        {
            removePersonFromPreview(row, &group.trainers_ids, "Удалить тренера из группы?");
        });
    }


    void updateViewer(RecordsViewer *peopleViewer,
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

        peopleViewer->updateContent(strindTable, Person::pattern());
    }


    void addPersonToPreview(QList<Person> people, QList<int> *peopleValidIds)
    {
        QList<Person> peopleToShow;
        for (Person pers : people)
        {
            if (!peopleValidIds->contains(pers.id)
                    /*&& group.sportType == pers.sportType*/)
            {
                peopleToShow << pers;
            }
        }

        int row = RecordChooser::getChoosedRow(
                    Person::toStringTable(peopleToShow), this);

        if (row >= 0)
        {
            int id = peopleToShow.at(row).id;
            *peopleValidIds << id;

            updateWhenRunning(trainers, sportsmen);
        }
    }

    void removePersonFromPreview(int row, QList<int> *validIds, QString question = "")
    {
        if (question != "")
        {
            int result = QMessageBox::question(this, " ", question);

            if (result != QMessageBox::Yes)
            {
                return;
            }
        }
        validIds->removeAt(row);
        updateWhenRunning(trainers, sportsmen);
    }
};




























