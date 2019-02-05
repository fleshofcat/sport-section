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
        mw.update(*db->getPeople(), *db->getSchedule());

        connect(&mw, &MainWindow::addPersonIsRequred,
                this, &SportSection::addPersonToDb);

        connect(&mw, &MainWindow::removePersonIsRequred,
                this, &SportSection::removePersonFromDb);

        connect(&mw, &MainWindow::editPersonIsRequred,
                this, &SportSection::updatePersonIntoDb);

        connect(&mw, &MainWindow::addScheduleRequred,
                this, &SportSection::addScheduleToDb);

        connect(&mw, &MainWindow::removeScheduleRequred,
                this, &SportSection::removeScheduleFromDb);

        connect(&mw, &MainWindow::editScheduleRequred,
                this, &SportSection::updateScheduleIntoDb);
    }

private slots:
    void addPersonToDb(Person pers)
    {
        if (db->addPerson(pers))
        {
            updateMainWindow(*db->getPeople(), *db->getSchedule());
        }
    }

    void removePersonFromDb(Person pers)
    {
        if (db->removePerson(pers))
        {
            updateMainWindow(*db->getPeople(), *db->getSchedule());
        }
    }

    void updatePersonIntoDb(Person pers)
    {
        if (db->replacePersonById(pers))
        {
            updateMainWindow(*db->getPeople(), *db->getSchedule());
        }
    }

    void addScheduleToDb(Schedule sched)
    {
        if (db->addSchedule(sched))
        {
            updateMainWindow(*db->getPeople(), *db->getSchedule());
        }
    }

    void removeScheduleFromDb(Schedule sched)
    {
        if (db->removeSchedule(sched))
        {
            updateMainWindow(*db->getPeople(), *db->getSchedule());
        }
    }

    void updateScheduleIntoDb(Schedule sched)
    {
        if (db->replaceScheduleById(sched))
        {
            updateMainWindow(*db->getPeople(), *db->getSchedule());
        }
    }

private:
    bool updateMainWindow(QList<Person> people, QList<Schedule> schedule)
    {
        if (isUpdateValud(people, schedule))
        {
            mw.update(people, schedule);
            return true;
        }
        qDebug() << "SpSec::updateMainWindow(..) : update is not valid";
        return false;
    }

    bool isUpdateValud(QList<Person> people, QList<Schedule> schedule)
    {
        for (Schedule sched : schedule)
        {
            if (isPersonExists(sched.child_id, people) == false
                    || isPersonExists(sched.trainer_id, people) == false)
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














