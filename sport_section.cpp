#include "sport_section.h"

SportSection::
SportSection(QObject *parent)
    : QObject(parent)
{
    QString path = QDir("../sport-section/res/").exists() ?
                "../sport-section/res/sport_people.db"
              : "sport_people.db";

    db.touchDb(path);
    ui.show();

    updateMainWindow();
    setUpConnections();
}

void SportSection::
setUpConnections()
{
    connect(&ui, &Ui::needSaveSportsman, [=] (Person pers)
    {
        if (db.saveSportsman(pers))
        {
            updateMainWindow();
        }
    });
    connect(&ui, &Ui::needRemoveSportsman, [=] (int id)
    {
        if (db.removeSportsman(id))
        {
            updateMainWindow();
        }
    });

    connect(&ui, &Ui::needSaveTrainer, this, [=] (Person pers)
    {
        if (db.saveTrainer(pers))
        {
            updateMainWindow();
        }
    });
    connect(&ui, &Ui::needRemoveTrainer, this, [=] (int id)
    {
        if (db.removeTrainer(id))
        {
            updateMainWindow();
        }
    });


    connect(&ui, &Ui::needSaveGroup, [=] (Group group)
    {
        if (db.saveGroup(group))
        {
            updateMainWindow();
        }
    });
    connect(&ui, &Ui::needRemoveGroup, [=] (int id)
    {
        if (db.removeGroup(id))
        {
            updateMainWindow();
        }
    });


    connect(&ui, &Ui::needSaveSchedule, [=] (Schedule sch)
    {
        if (db.saveSchedule(sch))
        {
            updateMainWindow();
        }
    });
    connect(&ui, &Ui::needRemoveSchedule, [=] (int id)
    {
        if (db.removeSchedule(id))
        {
            updateMainWindow();
        }
    });

    connect(&ui, &Ui::needMakeDoneSchedule, [=] (Schedule sch)
    {
        if (db.hardSaveClosedSchedule(sch))
        {
            db.removeSchedule(sch.id);
            updateMainWindow();
        }
    });
}

void SportSection::
updateMainWindow()
{
    auto sportsmen = db.getSportsmen();
    auto trainers = db.getTrainers();
    auto groups = db.getGroups();
    auto schedules = db.getSchedules();
    auto closedSchedules = db.getClosedSchedule();

    ui.updateSportsmen(sportsmen);
    ui.updateTrainers(trainers);
    ui.updateGroups(groups);
    ui.updateSchedules(schedules);
    ui.updateClosedSchedules(closedSchedules);
}















