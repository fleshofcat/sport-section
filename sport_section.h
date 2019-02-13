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

//        connect(&mw, &MainWindow::editPersonIsRequred,
//                this, &SportSection::updatePersonIntoDb);

        connect(&mw, &MainWindow::addScheduleRequred,
                this, &SportSection::addScheduleToDb);

        connect(&mw, &MainWindow::removeScheduleRequred,
                this, &SportSection::removeScheduleFromDb);

        connect(&mw, &MainWindow::editScheduleRequred,
                this, &SportSection::updateScheduleIntoDb);
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
    void addScheduleToDb(Schedule sched)
    {
        if (db->addSchedule(sched)) // попытка добавить расписание в бд
        {
            updateMainWindow();     // обновление интерфейса
        }
    }


    // обработчик запроса удаления расписания из бд
    void removeScheduleFromDb(Schedule sched)
    {
        if (db->removeSchedule(sched))  // попытка удаления расписания из бд
        {
            updateMainWindow();         // // обновление интерфейса
        }
    }


    // обработчик запроса обновления расписания в бд
    void updateScheduleIntoDb(Schedule sched)
    {
        if (db->replaceScheduleById(sched)) // попытка обновить расписание в бд
        {
            updateMainWindow();             // обновление интерфейса
        }
    }

private:

    // обновление главного окна пользовательского интерфейса
    bool updateMainWindow()
    {
        QList<Person> *children = db->getChildren();     // извлчение из бд
        QList<Person> *trainers = db->getTrainers();     // списка детей,
        QList<Schedule> *schedules = db->getSchedules(); // тренеров и расписаний


        if (isUpdateValud(children, trainers, schedules))   // если все данные актуальны
        {
            mw.update(*children, *trainers, *schedules);    // обновление пользовательского интерфейса
            return true;                                    // этими данными
        }
        qDebug() << "SpSec::updateMainWindow(..) : update is not valid";
        return false;                                       // если данные не актуальны, возврат false
    }


    // проверка данных на актуальность
    bool isUpdateValud(QList<Person> *children, QList<Person> *trainers, QList<Schedule> *schedule)
    {
        for (Schedule sched : *schedule)                                   // для каждого расписания проверить
        {                                                                  // что все его ссылки актуальны
            if (isPersonExists(sched.child_id, *children) == false
                    || isPersonExists(sched.trainer_id, *trainers) == false)
            {
                return false;                                              // если это так, вернуть true
            }
        }
        return true;                                                       // иначе false
    }


    // проверка что человек существует
    bool isPersonExists(int pers_id, QList<Person> people)
    {
        for (Person pers : people)  // сравнение проверяемого id
        {                           // и всех id в списке
            if (pers.id == pers_id)
            {
                return true;        // если нашлось совпадение вернуть true
            }
        }
        return false;               // иначе false
    }

};












