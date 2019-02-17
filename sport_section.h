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
private:
    MainWindow mw; // пользовательский интерфейс

    // создание модуля бд
    DbManager *db = new DbManager("../record/res/sport_people.db", this);

public:
    // конструктор, выполняющийся при создании объекта
    explicit SportSection(QObject *parent = nullptr)
        : QObject(parent)
    {
        mw.show(); // активация пользовательского интерфейса


        // обновление объекта пользовательского интерфейса данными из бд
        updateMainWindow();


        // установка связей между запросами модуля пользовательского интерфейса
        // и обработчиков этих запросов
        connect(&mw, &MainWindow::savePersonIsRequred,
                this, &SportSection::savePersonToDb);

        connect(&mw, &MainWindow::removePersonIsRequred,
                this, &SportSection::removePersonFromDb);
    }

private slots:

    // обработчик запроса добавления и обновления человека в бд
    void savePersonToDb(Person pers)
    {
        if (pers.id <= 0)
        {
            if (db->addPerson(pers)) // попытка добавить человека в бд
            {
                updateMainWindow(); // обновление интерфейса
            }
        }
        else
        {
            if (db->replacePersonById(pers)) // попытка обновить человека в бд
            {
                updateMainWindow();          // обновление интерфейса
            }
        }
    }


    // обработчик запроса удаления человека из бд
    void removePersonFromDb(int id)
    {
        Person pers;
        pers.id = id;

        if (db->removePerson(pers)) // попытка удалить человека из бд
        {
            updateMainWindow();     // обновление интерфейса
        }
    }




    // обработчик запроса добавления расписания в бд
    void addScheduleToDb(Group sched)
    {
        if (db->addSchedule(sched)) // попытка добавить расписание в бд
        {
            updateMainWindow();     // обновление интерфейса
        }
    }


    // обработчик запроса удаления расписания из бд
    void removeScheduleFromDb(Group sched)
    {
        if (db->removeSchedule(sched))  // попытка удаления расписания из бд
        {
            updateMainWindow();         // // обновление интерфейса
        }
    }


    // обработчик запроса обновления расписания в бд
    void updateScheduleIntoDb(Group sched)
    {
        if (db->replaceScheduleById(sched)) // попытка обновить расписание в бд
        {
            updateMainWindow();             // обновление интерфейса
        }
    }

private:

    // обновление главного окна пользовательского интерфейса
    void updateMainWindow()
    {
        mw.updateUi(*db->getChildren(),   // обновление пользовательского интерфейса
                    *db->getTrainers(),   // данными из бд
                    *db->getSchedules());
    }

};












