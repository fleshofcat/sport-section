#pragma once

#include "mainwindow.h"
#include "db_manager.h"

class SportSection : public QObject
{
    Q_OBJECT
private:
    MainWindow *mw = new MainWindow;
    DbManager *db = new DbManager("../record/res/sport_people.db", this);

public:
    explicit SportSection(QObject *parent = nullptr)
        : QObject(parent)
    {
        mw->show();
        mw->update(*db->getPeople(), *db->getRecords());

        connect(mw, &MainWindow::addPersonIsRequred,
                this, &SportSection::addPersonToDb);

        connect(mw, &MainWindow::removePersonIsRequred,
                this, &SportSection::removePersonFromDb);

        connect(mw, &MainWindow::editPersonIsRequred,
                this, &SportSection::updatePersonIntoDb);

        connect(mw, &MainWindow::addTrainingRecordIsRequred,
                this, &SportSection::addTrainingRecordToDb);

        connect(mw, &MainWindow::removeTrainingRecordIsRequred,
                this, &SportSection::removeTrainingRecordFromDb);

        connect(mw, &MainWindow::editTrainingRecordIsRequred,
                this, &SportSection::updateTrainingRecordIntoDb);
    }

private slots:
    void addPersonToDb(Person pers)
    {
        db->addPerson(pers);
        mw->update(*db->getPeople(), *db->getRecords());
    }

    void removePersonFromDb(Person pers)
    {
        db->removePerson(pers);
        mw->update(*db->getPeople(), *db->getRecords());
    }

    void updatePersonIntoDb(Person pers)
    {
        db->replacePersonById(pers);
        mw->update(*db->getPeople(), *db->getRecords());
    }

    void addTrainingRecordToDb(Record record)
    {
        db->addRecord(record);
        mw->update(*db->getPeople(), *db->getRecords());
    }

    void removeTrainingRecordFromDb(Record record)
    {
        db->removeRecord(record);
        mw->update(*db->getPeople(), *db->getRecords());
    }

    void updateTrainingRecordIntoDb(Record record)
    {
        db->replaceRecordById(record);
        mw->update(*db->getPeople(), *db->getRecords());
    }
};














