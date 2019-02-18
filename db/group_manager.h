#pragma once

#include <QtSql> // для работы с бд

#include "common/common_objects.h" // общие объекты (Person/Schedule)

// класс ScheduleManager/МенеджерРасписаний
//
// отвечает за хранение расписаний в бд
class GroupManager : public QObject
{
    Q_OBJECT
    QString tableName;

public:
    // конструктор
    // если при создании этого объекта в бд нет нужной ему таблицы
    // он сам создаст ее
    explicit GroupManager(
            QString tableName,
            QObject *parent = nullptr)
        : QObject(parent)
    {
        this->tableName = tableName;

        touchGroupTable(tableName);
    }

    bool addGroup(Group group)
    {
        QSqlQuery query("SELECT MAX(id) FROM " + tableName);
        if (query.next())
        {
            group.id = query.record().value(0).toInt() + 1;
        }


        query.prepare("INSERT INTO " + tableName +
                      "         (id, group_name, sport_type)    "
                      " VALUES  (:id, :group_name, :sport_type) ");

        query.addBindValue(group.id);
        for (QString field : group.getProperty())
        {
            query.addBindValue(field);
        }

        if (query.exec() == false)
        {
            qDebug() << query.lastError().text();
            return false;
        }



        for (int trainer_id : group.trainers_id)
        {
            query.prepare("INSERT INTO groups_trainers      "
                          "     (group_id, trainer_id)      "
                          " VALUES (:group_id, :trainer_id) ");

            query.addBindValue(group.id);
            query.addBindValue(trainer_id);

            if (query.exec() == false)
            {
                qDebug() << query.lastError().text();
                return false;
            }
        }

        for (int sportsman_id : group.children_id)
        {
            query.prepare("INSERT INTO groups_sportsmen         "
                          "     (group_id, trainer_id)          "
                          " VALUES (:group_id, :sportsman_id)   ");

            query.addBindValue(group.id);
            query.addBindValue(sportsman_id);

            if (query.exec() == false)
            {
                qDebug() << query.lastError().text();
                return false;
            }

        }

        return true;
    }


    void setRelationToTrainer()
    {

    }


    /*
    // метод добавления расписания
    bool addGroup(Group sched)
    {
        // проверка что расписание полное
        if (sched.isFull() == false)
            return false;

        // запись расписания sql запросом
        QSqlQuery query;
//        query.prepare("INSERT INTO " + tableName +
//                      "         (trainer_id, child_id)"
//                      " VALUES  (:trainer_id, :child_id)");

//        query.bindValue(":trainer_id", sched.trainer_id);
//        query.bindValue(":child_id", sched.child_id);

        bool ret = query.exec();

        if (!ret)
            qDebug() << "query.exec() in addSchedule failed";

        return ret; // возвращение успеха/не успеха операции
    }

    // метод удаления расписания
    bool removeSchedule(Group sched)
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
    bool replaceScheduleById(Group sched)
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
    QList<Group> *getAllSchedules()
    {
        // выполнение sql запроса
        QSqlQuery query;
        query.prepare("SELECT * FROM " + tableName);

        QList<Group> *scheduleList = new QList<Group>;

        if (query.exec())        // если запрос прошел успешно
        {
            while (query.next()) // упаковать расписания в список
            {

                Group *sched = new Group();

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
    */

    void touchGroupTable(QString tableName)
    {
        QSqlQuery query("SELECT name FROM sqlite_master"
                        " WHERE name='" + tableName + "'");

        query.next();
        if (query.value(0).toString() != tableName)
        {
            query.exec("CREATE TABLE IF NOT EXISTS " + tableName + " (  " +
                       " id INTEGER PRIMARY KEY AUTOINCREMENT, UNIQUE   "
                       " group_name TEXT NOT NULL,                      "
                       " sport_type TEXT NOT NULL)                      ");

            qDebug() << "Creating " + tableName + " table inside database";
        }
    }
};





