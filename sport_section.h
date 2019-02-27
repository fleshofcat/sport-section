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
        QString path = QDir("../record/res/").exists() ?
                    "../record/res/sport_people.db"
                  : "sport_people.db";

        db.touchDb(path);
        mw.show(); // активация пользовательского интерфейса

        // обновление объекта пользовательского интерфейса данными из бд
        updateMainWindow();

        // установка связей между запросами модуля пользовательского интерфейса
        // и обработчиков этих запросов
        connect(&mw, &MainWindow::savePerson,
                this, &SportSection::savePersonToDb);

        connect(&mw, &MainWindow::removePersonIs,
                this, &SportSection::removePersonFromDb);



        connect(&mw, &MainWindow::saveGroup,
                this, &SportSection::saveGroupToDb);

        connect(&mw, &MainWindow::removeGroup,
                this, &SportSection::removeGroupFromDb);
    }

private slots:
    // обработчик запроса добавления и обновления человека в бд
    void savePersonToDb(Person pers)
    {
        if (db.savePerson(pers)) // попытка добавить человека в бд
        {
            updateMainWindow(); // обновление интерфейса
        }
    }


    // обработчик запроса удаления человека из бд
    void removePersonFromDb(int id, Person::Who who)
    {
        if (db.removePerson(id, who)) // попытка удалить человека из бд
        {
            updateMainWindow();     // обновление интерфейса
        }
    }

    // обработчик запроса добавления и обновления человека в бд
    void saveGroupToDb(Group group)
    {
        if (db.saveGroup(group)) // попытка добавить человека в бд
        {
            updateMainWindow(); // обновление интерфейса
        }
    }


    // обработчик запроса удаления человека из бд
    void removeGroupFromDb(int id)
    {
        if (db.removeGroup(id)) // попытка удалить человека из бд
        {
            updateMainWindow();     // обновление интерфейса
        }
    }


private:
    // обновление главного окна пользовательского интерфейса
    void updateMainWindow()
    {
        auto sportsmen = db.getSportsmen();
        auto trainers = db.getTrainers();
        auto groups = db.getGroups();

        mw.updateContent(sportsmen,
                         trainers,
                         groups);
    }

};



























