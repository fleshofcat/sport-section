#pragma once

#include <QLabel>
#include <QMessageBox>

#include "common/common_objects.h"
#include "ui/widgets/property_editor.h"
#include "ui/widgets/record_viewer.h"
#include "ui/widgets/record_chooser.h"

class GroupEditor : public QWidget
{
    Q_OBJECT

    QList<Person> sportsmen;
    QList<Person> trainers;
    Group group;

    PropertyEditor *propertyEditor;

    RecordsViewer *trainersViewer;
    RecordsViewer *sportsmenViewer;

signals:
    void saveIsRequred(Group group);
    void removeIsRequred(int id);
    void exitIsRequred();

public:
    GroupEditor(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();
        updateContent({}, {}, Group());
    }

    GroupEditor(QList<Person> trainers,
                QList<Person> sportsmen,
                Group group,
                QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();
        updateContent(trainers, sportsmen, group);
    }

    void updateContent(QList<Person> trainers,
                       QList<Person> sportsmen,
                       Group group)
    {
        this->group = group;
        this->trainers = trainers;
        this->sportsmen = sportsmen;

        propertyEditor->updateContent(Group::getPattern(), group.getInList());


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
        propertyEditor = new PropertyEditor;

        trainersViewer = new RecordsViewer;
        sportsmenViewer = new RecordsViewer;

        QVBoxLayout *trainersViewerLayout = new QVBoxLayout;
        trainersViewerLayout->addWidget(new QLabel("Тренеры"));
        trainersViewerLayout->addWidget(trainersViewer);

        QVBoxLayout *sportsmenViewerLayout = new QVBoxLayout;
        sportsmenViewerLayout->addWidget(new QLabel("Спортсмены"));
        sportsmenViewerLayout->addWidget(sportsmenViewer);


        QHBoxLayout *peopleViewerLayout = new QHBoxLayout;
        peopleViewerLayout->addItem(trainersViewerLayout);
        peopleViewerLayout->addItem(sportsmenViewerLayout);

        QVBoxLayout *basicLayout = new QVBoxLayout;
        basicLayout->addWidget(propertyEditor);
        basicLayout->addItem(peopleViewerLayout);

        setLayout(basicLayout);


        connect(propertyEditor, &PropertyEditor::saveIsRequred,
                this, &GroupEditor::on_saveGroup);

        connect(propertyEditor, &PropertyEditor::removeIsRequred,
                this, &GroupEditor::on_removeGroup);

        connect(propertyEditor, &PropertyEditor::exitIsRequred,
                this, &GroupEditor::exitIsRequred);




        connect(sportsmenViewer, &RecordsViewer::createRecordIsRequred,
                this, &GroupEditor::on_addSportsmen);

        connect(sportsmenViewer, &RecordsViewer::editRecordIsRequred,
                this, &GroupEditor::on_removeSportsmen);

        connect(trainersViewer, &RecordsViewer::createRecordIsRequred,
                this, &GroupEditor::on_addTrainer);

        connect(trainersViewer, &RecordsViewer::editRecordIsRequred,
                this, &GroupEditor::on_removeTrainer);
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

        peopleViewer->updateData(Person::getPattern(), strindTable);
    }

private slots:
    void on_addSportsmen()
    {
        int row = RecordChooser::getChoosedRow(
                    peopleToStringTable(sportsmen), this);

        if (row >= 0)
        {
            int id = sportsmen.at(row).id;

            if (!group.sportsmen_ids.contains(id))
            {
                group.sportsmen_ids << id;
            }

            updateContent(trainers, sportsmen, group);
        }
    }

    void on_removeSportsmen(int row)
    {
        int result = QMessageBox::question(
                    this, "",
                    "Удалить спортсмена?",
                    QMessageBox::Yes, QMessageBox::No);

        if (result == QMessageBox::Yes)
        {
            if (row >= 0)
            {
                int id = sportsmen.at(row).id;
                group.sportsmen_ids.removeAll(id);

                updateContent(trainers, sportsmen, group);
            }
        }
    }

    void on_addTrainer()
    {
        int row = RecordChooser::getChoosedRow(
                    peopleToStringTable(trainers), this);

        if (row >= 0)
        {
            int id = trainers.at(row).id;

            if (!group.trainers_ids.contains(id))
            {
                group.trainers_ids << id;
            }

            updateContent(trainers, sportsmen, group);
        }
    }

    void on_removeTrainer(int row)
    {
        int result = QMessageBox::question(
                    this, "",
                    "Удалить тренера?",
                    QMessageBox::Yes, QMessageBox::No);

        if (result == QMessageBox::Yes)
        {
            if (row >= 0)
            {
                int id = trainers.at(row).id;
                group.trainers_ids.removeAll(id);

                updateContent(trainers, sportsmen, group);
            }
        }
    }


    void on_saveGroup(QList<QString> groupInList)
    {
        group.setInList(groupInList);

        emit saveIsRequred(group);
    }

    void on_removeGroup()
    {
        emit removeIsRequred(this->group.id);
    }

private:
    QList<QList<QString>> peopleToStringTable(QList<Person> people)
    {
        QList<QList<QString>> peopleTable;
        for (Person pers : people)
        {
            peopleTable << pers.getInList();
        }

        return peopleTable;
    }

};




























