#pragma once

#include <QtSql> // для работы с бд

#include "common/common_objects.h"
#include "db/group_people_relations.h"

// класс ScheduleManager/МенеджерРасписаний
//
// отвечает за хранение расписаний в бд
class GroupManager : public QObject
{
    Q_OBJECT

    friend class TestGroupStorage;

    QString groupTable;

    GroupPeopleRelations *trainersRefs;
    GroupPeopleRelations *sportsmenRefs;

public:
    // конструктор
    // если при создании этого объекта в бд нет нужной ему таблицы
    // он сам создаст ее
    explicit GroupManager(QString groupTableName,
                          QString trainerTableName,
                          QString sportsmanTableName,
                          QObject *parent = nullptr)
        : QObject(parent)
    {
        this->groupTable = groupTableName;
        touchGroupTable(groupTableName);

        trainersRefs = new GroupPeopleRelations(
                    groupTableName, trainerTableName);

        sportsmenRefs = new GroupPeopleRelations(
                    groupTableName, sportsmanTableName);
    }

private:
    bool addGroup(Group group)
    {       
        int maxGroupId = getMaxIdFromTable(groupTable);


        if (maxGroupId >= 0)
        {
            group.id = maxGroupId + 1;

            QSqlQuery query;
            query.prepare("INSERT INTO " + groupTable +
                          "         (id, group_name, sport_type)    "
                          " VALUES  (:id, :group_name, :sport_type) ");

            query.addBindValue(group.id);
            bindValueList(query, group.getProperty());

            if (query.exec())
            {
                if (trainersRefs->addLinks(group.id, group.trainers_ids))
                {
                    if (sportsmenRefs->addLinks(group.id, group.sportsmen_ids))
                    {
                        return true;

                    } else
                        qWarning() << "group sportsmen refs was not added";
                } else
                    qWarning() << "group trainers refs was not added";
            } else
                qWarning() << query.lastError().text();
        } else
            qWarning() << "GroupManager::addGroup failed to get maxGroupId";

        return false;
    }


//    bool updateGroup(Group group)
//    {

//    }


    void touchGroupTable(QString groupTable)
    {
        QSqlQuery query("SELECT name FROM sqlite_master"
                        " WHERE name='" + groupTable + "'");

        if (query.next() == false)
        {
            query.exec("CREATE TABLE IF NOT EXISTS " + groupTable + " ( \n" +
                       " id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,   \n"
                       " group_name TEXT NOT NULL,                      \n"
                       " sport_type TEXT NOT NULL                       \n"
                       ")                                                 ");

            if (query.lastError().isValid() == false)
                qDebug() << "Creating " + groupTable + " table inside database";
        }
    }


    static int getMaxIdFromTable(QString table) // возвращает -1 при неудаче
    {
        QSqlQuery query("SELECT MAX(id) FROM " + table);

        if (query.lastError().isValid() == false)
        {
            if (query.next())
            {
                return query.record().value(0).toInt();
            }
        }
        else
        {
            qWarning() << query.lastError().text();
        }
        return -1;
    }

    void bindValueList(QSqlQuery &query, QList<QString> list) // TODO test it
    {
        for (QString field : list)
        {
            query.addBindValue(field);
        }
    }

};



    // TODO to analysis
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



