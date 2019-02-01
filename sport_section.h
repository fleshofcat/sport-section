#pragma once

#include "mainwindow.h"
#include "db_manager.h"

class SportSection : public QObject
{
    Q_OBJECT
private:
//    MainWindow *mw = new MainWindow();
    MainWindow mw;
    DbManager *db = new DbManager("../record/res/sport_people.db", this);

public:
    explicit SportSection(QObject *parent = nullptr)
        : QObject(parent)
    {
        mw.show();
        mw.update(*db->getPeople(), *db->getRecords());

        connect(&mw, &MainWindow::addPersonIsRequred,
                this, &SportSection::addPersonToDb);

        connect(&mw, &MainWindow::removePersonIsRequred,
                this, &SportSection::removePersonFromDb);

        connect(&mw, &MainWindow::editPersonIsRequred,
                this, &SportSection::updatePersonIntoDb);

        connect(&mw, &MainWindow::addRecordIsRequred,
                this, &SportSection::addRecordToDb);

        connect(&mw, &MainWindow::removeRecordIsRequred,
                this, &SportSection::removeRecordFromDb);

        connect(&mw, &MainWindow::editRecordIsRequred,
                this, &SportSection::updateRecordIntoDb);
    }

private slots:
    void addPersonToDb(Person pers)
    {
        if (db->addPerson(pers))
        {
            updateMainWindow(*db->getPeople(), *db->getRecords());
        }
    }

    void removePersonFromDb(Person pers)
    {
        if (db->removePerson(pers))
        {
            updateMainWindow(*db->getPeople(), *db->getRecords());
        }
    }

    void updatePersonIntoDb(Person pers)
    {
        if (db->replacePersonById(pers))
        {
            updateMainWindow(*db->getPeople(), *db->getRecords());
        }
    }

    void addRecordToDb(Record record)
    {
        if (db->addRecord(record))
        {
            updateMainWindow(*db->getPeople(), *db->getRecords());
        }
    }

    void removeRecordFromDb(Record record)
    {
        if (db->removeRecord(record))
        {
            updateMainWindow(*db->getPeople(), *db->getRecords());
        }
    }

    void updateRecordIntoDb(Record record)
    {
        if (db->replaceRecordById(record))
        {
            updateMainWindow(*db->getPeople(), *db->getRecords());
        }
    }

private:
    bool updateMainWindow(QList<Person> people, QList<Record> schedule)
    {
        if (isUpdateValud(people, schedule))
        {
            mw.update(people, schedule);
            return true;
        }
        qDebug() << "SpSec::updateMainWindow(..) : update is not valid";
        return false;
    }

    bool isUpdateValud(QList<Person> people, QList<Record> schedule)
    {
        for (Record rec : schedule)
        {
            if (isPersonExists(rec.child_id, people) == false
                    || isPersonExists(rec.trainer_id, people) == false)
            {
                return false;
            }
        }
        return true;
    }

    bool isPersonExists(int pers_id, QList<Person> people)
    {
        for (Person pers : people)
        {
            if (pers.id == pers_id)
            {
                return true;
            }
        }
        return false;
    }
};














