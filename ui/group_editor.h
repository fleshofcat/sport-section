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

    QList<Person> sportsmen;
    QList<Person> trainers;
    Group group;

    QPushButton *saveButton;
    QPushButton *removeButton;
    QPushButton *exitButton;

    QLineEdit *groupNameField;
    QLineEdit *sportTypeField;

    RecordsWidget *trainersViewer;
    RecordsWidget *sportsmenViewer;

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

        connect(saveButton, &QPushButton::clicked, this, &GroupEditor::on_saveGroup);
        connect(removeButton, &QPushButton::clicked, this, &GroupEditor::on_removeGroup);
        connect(exitButton, &QPushButton::clicked, this, &GroupEditor::needExit);

        connect(sportsmenViewer, &RecordsWidget::createRecordActivated, this, &GroupEditor::on_addSportsmen);
        connect(sportsmenViewer, &RecordsWidget::recordActivated, this, &GroupEditor::on_removeSportsmen);

        connect(trainersViewer, &RecordsWidget::createRecordActivated, this, &GroupEditor::on_addTrainer);
        connect(trainersViewer, &RecordsWidget::recordActivated, this, &GroupEditor::on_removeTrainer);

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

        updateViewer(trainersViewer, trainers, group.trainers_ids);
        updateViewer(sportsmenViewer, sportsmen, group.sportsmen_ids);
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

        trainersViewer = new RecordsWidget;
        sportsmenViewer = new RecordsWidget;

        QGridLayout *peopleViever = new QGridLayout;
        peopleViever->addWidget(new QLabel("Тренеры"), 0, 0);
        peopleViever->addWidget(new QLabel("Спортсмены"), 0, 1);
        peopleViever->addWidget(trainersViewer, 1, 0);
        peopleViever->addWidget(sportsmenViewer, 1, 1);


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
//        auto palette = saveButton->palette();
//        palette.setColor(QPalette::Button, QColor(Qt::blue));
//        saveButton->setAutoFillBackground(true);
//        saveButton->setPalette(palette);

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
        group.setInList({groupNameField->text(), sportTypeField->text()});
        emit needSave(group);
    }

    void on_removeGroup()
    {
        emit needRemove(this->group.id);
    }
};




























