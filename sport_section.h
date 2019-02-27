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
        connect(&mw, &MainWindow::saveSportsmen, this, &SportSection::on_saveSportsman);
        connect(&mw, &MainWindow::removeSportsmen, this, &SportSection::on_removeSportsman);

        connect(&mw, &MainWindow::saveTrainer, this, &SportSection::on_saveTrainer);
        connect(&mw, &MainWindow::removeTrainer, this, &SportSection::on_removeTrainer);

        connect(&mw, &MainWindow::saveGroup, this, &SportSection::on_saveGroup);
        connect(&mw, &MainWindow::removeGroup, this, &SportSection::on_removeGroup);
    }

private slots:
    // обработчик запроса добавления и обновления человека в бд
    void on_saveSportsman(Person pers)
    {
        if (db.saveSportsman(pers)) // попытка добавить человека в бд
        {
            updateMainWindow(); // обновление интерфейса
        }
    }

    void on_saveTrainer(Person pers)
    {
        if (db.saveTrainer(pers)) // попытка добавить человека в бд
        {
            updateMainWindow(); // обновление интерфейса
        }
    }


    // обработчик запроса удаления человека из бд
    void on_removeSportsman(int id)
    {
        if (db.removeSportsman(id)) // попытка удалить человека из бд
        {
            updateMainWindow();     // обновление интерфейса
        }
    }

    void on_removeTrainer(int id)
    {
        if (db.removeTrainer(id)) // попытка удалить человека из бд
        {
            updateMainWindow();     // обновление интерфейса
        }
    }

    // обработчик запроса добавления и обновления человека в бд
    void on_saveGroup(Group group)
    {
        if (db.saveGroup(group)) // попытка добавить человека в бд
        {
            updateMainWindow(); // обновление интерфейса
        }
    }


    // обработчик запроса удаления человека из бд
    void on_removeGroup(int id)
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



























