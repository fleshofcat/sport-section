#pragma once

#include <QLabel>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

#include "common/group.h"
#include "common/person.h"

#include "ui/widgets/records_viewer.h"
#include "ui/widgets/record_chooser.h"

class GroupEditor : public QWidget
{
    Q_OBJECT

    friend class TestGroupEditor;

    QString trainerIconPath;
    QString sportsmanIconPath;

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
    // not editable
    QLineEdit *ratingField;
    QLineEdit *eventCountField;

    // people in group
    QPushButton *addTrainerButton;
    QPushButton *addSportsmanButton;
    RecordsViewer *trainersViewer;
    RecordsViewer *sportsmenViewer;

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

    void setTrainerIconPath(QString trainerIconPath)
    {
        this->trainerIconPath = trainerIconPath;
        trainersViewer->setIconPath(trainerIconPath);
    }

    void setSportsmanIconPath(QString sportsmanIconPath)
    {
        this->sportsmanIconPath = sportsmanIconPath;
        sportsmenViewer->setIconPath(sportsmanIconPath);
    }

    void updateContent(QList<Person> trainers,
                       QList<Person> sportsmen,
                       Group group = Group())
    {
        this->group = group;

        groupNameField->setText(group.groupName);
        sportTypeField->setText(group.sportType);

        ratingField->setText(QString::number(double(group.getGroupRating())));
        eventCountField->setText(QString::number(group.eventNumber));

        updateWhenRunning(trainers, sportsmen);
    }

    void updateWhenRunning(QList<Person> trainers,
                           QList<Person> sportsmen)
    {
        this->trainers = trainers;
        this->sportsmen = sportsmen;

        trainersViewer->updateContent(Person::toPreviewStringTable(group.trainers),
                                      Person::getPreviewPattern());
        sportsmenViewer->updateContent(Person::toPreviewStringTable(group.sportsmen),
                                       Person::getPreviewPattern());
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
        groupNameField = new QLineEdit;
        sportTypeField = new QLineEdit;

        ratingField = new QLineEdit;
        ratingField->setReadOnly(true);
        eventCountField = new QLineEdit;
        eventCountField->setReadOnly(true);

        QFormLayout *editors = new QFormLayout;
        editors->addRow("Имя группы", groupNameField);
        editors->addRow("Вид спорта", sportTypeField);
        editors->addRow("Рейтинг", ratingField);
        editors->addRow("Мероприятий", eventCountField);

        addTrainerButton = new QPushButton("+");
        addSportsmanButton = new QPushButton("+");
        trainersViewer = new RecordsViewer;

        sportsmenViewer = new RecordsViewer;

        QGridLayout *peopleViever = new QGridLayout;
        peopleViever->addWidget(new QLabel("Тренеры"), 0, 0);
        peopleViever->addWidget(new QLabel("Спортсмены"), 0, 1);
        peopleViever->addWidget(addTrainerButton, 1, 0);
        peopleViever->addWidget(addSportsmanButton, 1, 1);
        peopleViever->addWidget(trainersViewer, 2, 0);
        peopleViever->addWidget(sportsmenViewer, 2, 1);


        saveButton = new QPushButton("Сохранить");
        removeButton = new QPushButton("Удалить");
        exitButton = new QPushButton("Выйти");

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
            addPersonToPreview(sportsmen, &group.sportsmen, sportsmanIconPath);
        });
        connect(sportsmenViewer, &RecordsViewer::rowIsActivated, [=] (int row)
        {
            removePersonFromPreview(row, &group.sportsmen, "Удалить спортсмена из группы?");
        });

        connect(addTrainerButton, &QPushButton::clicked, [=] ()
        {
            addPersonToPreview(trainers, &group.trainers, trainerIconPath);
        });
        connect(trainersViewer, &RecordsViewer::rowIsActivated, [=] (int row)
        {
            removePersonFromPreview(row, &group.trainers, "Удалить тренера из группы?");
        });
    }

    void addPersonToPreview(QList<Person> all_people, QList<Person> *existing_people, QString iconPath = "")
    {
        QList<Person> peopleToShow;
        for (Person pers : all_people)
        {
            if (!existing_people->contains(pers)
                    /*&& group.sportType == pers.sportType*/)
            {
                peopleToShow << pers;
            }
        }

        int row = RecordChooser::getChoosedRow(
                    Person::toPreviewStringTable(peopleToShow), this, "Доступные люди", iconPath);

        if (row >= 0)
        {
            *existing_people << peopleToShow.at(row);

            updateWhenRunning(trainers, sportsmen);
        }
    }

    void removePersonFromPreview(int row, QList<Person> *existingPeople, QString question = "")
    {
        if (question != "")
        {
            int result = QMessageBox::question(this, " ", question);

            if (result != QMessageBox::Yes)
            {
                return;
            }
        }
        existingPeople->removeAt(row);
        updateWhenRunning(trainers, sportsmen);
    }
};




























