#pragma once

#include "common/common_objects.h"
#include "ui/widgets/records_widget.h"

class PeopleTab : public QWidget
{
    Q_OBJECT

    QList<Person> people;
    RecordsWidget *peopleTable = nullptr;

signals:
    void savePerson(Person person);
    void removePerson(int id, bool isTrainer);

public:
    enum class Who {
        EMPTY,
        CHILDREN,
        TRAINERS
    } who = Who::EMPTY;


    PeopleTab(PeopleTab::Who who, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        this->who = who;

        setUpUi();

        connect(peopleTable, &RecordsWidget::createRecordIsRequred,
                this, &PeopleTab::on_createPerson);

        connect(peopleTable, &RecordsWidget::saveRecordIsRequred,
                this, &PeopleTab::on_savePerson);

        connect(peopleTable, &RecordsWidget::removeRecordIsRequred,
                this, &PeopleTab::on_removePerson);
    }


    void updateContent(Person pattern, QList<Person> people)
    {
        this->people = people;


        QList<QList<QString>> stringTable;

        for (Person pers : people)
        {
            stringTable << pers.getInList();
        }

        peopleTable->updateData(pattern.getInList(), stringTable);
    }

private:
    void setUpUi()
    {
        peopleTable = new RecordsWidget(this);

        peopleTable->resize(size());
        peopleTable->show();
    }

private slots:
    void on_createPerson(QList<QString> personInList)
    {
        Person pers(personInList);
        pers.isTrainer =
                (this->who == PeopleTab::Who::TRAINERS)
                ? true
                : false;

        emit savePerson(pers);
    }


    void on_savePerson(int row, QList<QString> personInList)
    {
        Person pers = people.at(row);

        pers.setInList(personInList);
        emit savePerson(pers);
    }


    void on_removePerson(int row)
    {
        Person pers = this->people.at(row);
        emit removePerson(pers.id, pers.isTrainer);
    }

public:
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        peopleTable->resize(resizeEvent->size());
    }


};






























