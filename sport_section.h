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
        if (db->savePerson(pers)) // попытка добавить человека в бд
        {
            updateMainWindow(); // обновление интерфейса
        }
    }


    // обработчик запроса удаления человека из бд
    void removePersonFromDb(int id, bool isTrainer)
    {
        if (db->removePerson(id, isTrainer)) // попытка удалить человека из бд
        {
            updateMainWindow();     // обновление интерфейса
        }
    }


private:

    // обновление главного окна пользовательского интерфейса
    void updateMainWindow()
    {
        mw.updateContent(db->getPersonPattern(),
                         *db->getChildren(),
                         *db->getTrainers());

//        auto children = *db->getChildren();
//        auto trainers = *db->getTrainers();

//        mw.updateContent(db->getPersonPattern(),
//                         children,
//                         trainers);
    }

};



























