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
    QList<Person> allSportsmen;
    QList<Person> allTrainers;
    Group oldGroup;
    Group group;

    // standart buttons
    QPushButton *saveButton;
    QPushButton *removeButton;
    QPushButton *exitButton;

    // editors
    QLineEdit *groupNameField;

    // not editable
    QLineEdit *ratingField;
    QLineEdit *eventCountField;
    QLineEdit *sportTypeField;

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
        this->oldGroup = group;

        setScheduleLimit("");

        groupNameField->setText(group.getGroupName());
        sportTypeField->setText(group.getSportType());

        ratingField->setText(QString::number(group.getRating()));
        eventCountField->setText(QString::number(group.getEventsNumber()));

        updateTrainersView(trainers);
        updateSportsmenView(sportsmen);
    }

    void setScheduleLimit(QString scheduleName = "")
    {
        removeButton->setEnabled(scheduleName.isEmpty());

        if (!scheduleName.isEmpty())
        {
            removeButton->setToolTip("Группу нельзя удалить пока она находится "
                                     "в расписании '" + scheduleName + "'");
        }
        else
        {
            removeButton->setToolTip("");
        }
    }

    void updateTrainersView(QList<Person> trainers)
    {
        this->allTrainers = trainers;
        trainersViewer->updateContent(Person::toPreviewTable(group.trainers),
                                      Person::getPreviewPattern());

        sportTypeField->setText(group.getSportType());
    }

    void updateSportsmenView(QList<Person> sportsmen)
    {
        this->allSportsmen = sportsmen;
        sportsmenViewer->updateContent(Person::toPreviewTable(group.sportsmen),
                                       Person::getPreviewPattern());

        sportTypeField->setText(group.getSportType());
        ratingField->setText(QString::number(group.getRating()));
    }

    Group getCurrentGroup()
    {
        group.setGroupName(groupNameField->text());
        return group;
    }

    Group getOldGroup()
    {
        return oldGroup;
    }

private:
    void setUpUi()
    {
        groupNameField = new QLineEdit;

        sportTypeField = new QLineEdit;
        sportTypeField->setReadOnly(true);
        sportTypeField->setToolTip("Спорт группы определяется людьми в ней");
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
            emit needSave(getCurrentGroup());
        });
        connect(removeButton, &QPushButton::clicked, [=] ()
        {
            emit needRemove(group.id);
        });
        connect(exitButton, &QPushButton::clicked,
                this, &GroupEditor::needExit);

        connect(addSportsmanButton, &QPushButton::clicked, [=] ()
        {
            QList<Person> allSportsmenWithSportType = allSportsmen;
            if (!group.getSportType().isEmpty())
            {
                allSportsmenWithSportType = Person::getPeopleBySportType(
                            allSportsmen, group.getSportType());
            }

            QList<Person> aviableSportsmen = Person::getFreePeople(
                        allSportsmenWithSportType, group.sportsmen);
            auto choosenPerson = choosePerson(aviableSportsmen, sportsmanIconPath);

            if (choosenPerson.id > 0)
            {
                group.sportsmen << choosenPerson;
                updateSportsmenView(allSportsmen);
            }
        });
        connect(sportsmenViewer, &RecordsViewer::rowIsActivated, [=] (int row)
        {
            bool result = askQuestion("Удалить спортсмена '"
                                      + group.sportsmen[row].getFirstName()
                                      + "' из группы?");
            if (result)
            {
                group.sportsmen.removeAt(row);
                updateSportsmenView(allSportsmen);
            }
        });

        connect(addTrainerButton, &QPushButton::clicked, [=] ()
        {
            QList<Person> allTrainersWithSportType = allTrainers;
            if (!group.getSportType().isEmpty())
            {
                allTrainersWithSportType = Person::getPeopleBySportType(
                            allTrainers, group.getSportType());
            }

            QList<Person> aviableTrainers = Person::getFreePeople(allTrainersWithSportType, group.trainers);
            auto choosenPerson = choosePerson(aviableTrainers, trainerIconPath);

            if (choosenPerson.id > 0)
            {
                group.trainers << choosenPerson;
                updateTrainersView(allTrainers);
            }
        });
        connect(trainersViewer, &RecordsViewer::rowIsActivated, [=] (int row)
        {
            bool result = askQuestion("Удалить тренера '"
                                      + group.trainers[row].getFirstName()
                                      + "' из группы?");
            if (result)
            {
                group.trainers.removeAt(row);
                updateTrainersView(allTrainers);
            }
        });
    }

    Person choosePerson(QList<Person> peopleToShow, QString iconPath)
    {
        int row = RecordChooser::getChoosedRow(
                    Person::toPreviewTable(peopleToShow), this, "Доступные люди", iconPath);

        if (row >= 0)
        {
            return peopleToShow.at(row);
        }
        return Person();
    }

    bool askQuestion(QString question)
    {
        int result = QMessageBox::question(this, " ", question);

        if (result == QMessageBox::Yes)
        {
            return true;
        }
        return false;
    }
};




























