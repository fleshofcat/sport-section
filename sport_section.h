#pragma once

#include "ui/main_window.h" // файл графического интерфейса
#include "db/db_manager.h" // файл модуля бд

// класс SportSection/СпортивнаяСекция
//
// главный класс приложения, осущесствляющий логику
// связывания остальных элементов
class SportSection : public QObject
{
    Q_OBJECT // обязательный макрос

    MainWindow mw;
    DbManager db;

public:
    // конструктор, выполняющийся при создании объекта
    SportSection(QObject *parent = nullptr)
        : QObject(parent)
    {
        QString path = QDir("../sport-section/res/").exists() ?
                    "../sport-section/res/sport_people.db"
                  : "sport_people.db";

        db.touchDb(path);
        mw.show(); // активация пользовательского интерфейса

        // обновление объекта пользовательского интерфейса данными из бд
        updateMainWindow();

        // установка связей между запросами модуля пользовательского интерфейса
        // и обработчиков этих запросов
        connect(&mw, &MainWindow::needSaveSportsman, [=] (Person pers)
        {
            if (db.saveSportsman(pers))
            {
                updateMainWindow();
            }
        });
        connect(&mw, &MainWindow::needRemoveSportsman, [=] (int id)
        {
            if (db.removeSportsman(id))
            {
                updateMainWindow();
            }
        });

        connect(&mw, &MainWindow::needSaveTrainer, this, [=] (Person pers)
        {
            if (db.saveTrainer(pers))
            {
                updateMainWindow();
            }
        });
        connect(&mw, &MainWindow::needRemoveTrainer, this, [=] (int id)
        {
            if (db.removeTrainer(id))
            {
                updateMainWindow();
            }
        });


        connect(&mw, &MainWindow::needSaveGroup, [=] (Group group)
        {
            if (db.saveGroup(group))
            {
                updateMainWindow();
            }
        });
        connect(&mw, &MainWindow::needRemoveGroup, [=] (int id)
        {
            if (db.removeGroup(id))
            {
                updateMainWindow();
            }
        });


        connect(&mw, &MainWindow::needSaveSchedule, [=] (Schedule sch)
        {
            if (db.saveSchedule(sch))
            {
                updateMainWindow();
            }
        });
        connect(&mw, &MainWindow::needRemoveSchedule, [=] (int id)
        {
            if (db.removeSchedule(id))
            {
                updateMainWindow();
            }
        });

        connect(&mw, &MainWindow::needMakeDoneSchedule, [=] (Schedule sch)
        {
            if (db.hardSaveClosedSchedule(sch))
            {
                db.removeSchedule(sch.id);
                updateMainWindow();
            }
        });
    }

private:
    // обновление главного окна пользовательского интерфейса
    void updateMainWindow()
    {
        auto sportsmen = db.getSportsmen();
        auto trainers = db.getTrainers();
        auto groups = db.getGroups();
        auto schedules = db.getSchedules();
        auto closedSchedules = db.getClosedSchedule();

        mw.updateContent(sportsmen,
                         trainers,
                         groups,
                         schedules,
                         closedSchedules);
    }

};



























