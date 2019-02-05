#pragma once

#include <QtSql> // для работы с бд

#include "common_objects.h" // общие объекты (Person/Schedule)

// класс ScheduleManager/МенеджерРасписаний
//
// отвечает за хранение расписаний в бд
class ScheduleManager : public QObject
{
    Q_OBJECT // обязательный макрос
    QString tableName = "relations"; // имя таблицы этого модуля

public:
    // конструктор
    // если при создании этого объекта в бд нет нужной ему таблицы
    // он сам создаст ее
    explicit ScheduleManager(QObject *parent = nullptr)
        : QObject(parent)
    {               // create relation table in db if not exist // TODO
        QSqlQuery query("SELECT name FROM sqlite_master"
                        " WHERE name='" + tableName + "'");

        query.next();
        if (query.value(0).toString() != tableName)
        {
            query.exec("CREATE TABLE IF NOT EXISTS " + tableName + "        "
                       " ( id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,   "
                       " trainer_id INTEGER NOT NULL,                       "
                       " child_id INTEGER NOT NULL)                         ");

            qDebug() << "Creating " + tableName + " table inside database";
        }
    }


    // метод добавления расписания
    bool addSchedule(Schedule sched)
    {
        // проверка что расписание полное
        if (sched.isFull() == false)
            return false;

        // запись расписания sql запросом
        QSqlQuery query;
        query.prepare("INSERT INTO " + tableName +
                      "         (trainer_id, child_id)"
                      " VALUES  (:trainer_id, :child_id)");

        query.bindValue(":trainer_id", sched.trainer_id);
        query.bindValue(":child_id", sched.child_id);

        bool ret = query.exec();

        if (!ret)
            qDebug() << "query.exec() in addSchedule failed";

        return ret; // возвращение успеха/не успеха операции
    }


    // метод удаления расписания
    bool removeSchedule(Schedule sched)
    {
        // sql запрос удаления расписания
        QSqlQuery query;

        query.prepare("DELETE FROM " + tableName + " WHERE id = (:id)");
        query.bindValue(":id", sched.id);

        bool ret = query.exec();

        if (!ret)
            qDebug() << "query.exec() in removeSchedule failed";

        return ret; // успешность операции
    }


    // метод обновления расписания в бд по id
    bool replaceScheduleById(Schedule sched)
    {
        // sql запрос на обновление
        QSqlQuery query;
        query.prepare(" UPDATE " + tableName + " SET    "
                      "     trainer_id = (:trainer_id), "
                      "     child_id = (:child_id)      "
                      " WHERE id = (:id)                ");


        query.bindValue(":id", sched.id);
        query.bindValue(":trainer_id", sched.trainer_id);
        query.bindValue(":child_id", sched.child_id);

        bool ret = query.exec();

        if (!ret)
            qDebug() << "query.exec() in replaceScheduleById failed";

        return ret; // успех операции
    }


    // метод получения всех расписаний их бд
    QList<Schedule> *getAllSchedules()
    {
        // выполнение sql запроса
        QSqlQuery query;
        query.prepare("SELECT * FROM " + tableName);

        QList<Schedule> *scheduleList = new QList<Schedule>;

        if (query.exec())        // если запрос прошел успешно
        {
            while (query.next()) // упаковать расписания в список
            {

                Schedule *sched = new Schedule();

                sched->id = query.value("id").toInt();
                sched->trainer_id = query.value("trainer_id").toInt();
                sched->child_id = query.value("child_id").toInt();

                *scheduleList << *sched;
            }
            return scheduleList;  // вернуть полученный список
        }

        qDebug() << "query.exec() in getAllSchedules failed";

        return nullptr;  // если запрос прошел не учпешно, вернуть нулевой указатель
    }
};





