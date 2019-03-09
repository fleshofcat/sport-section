#pragma once

#include <QtTest/QtTest>
#include "db/people_manager.h"
#include "db/db_manager.h"

class TestPeopleStorage : public QObject
{
    Q_OBJECT

    QString dbPath = "../record/tests/test_res/sport_people.db";

private slots:
//    void initTestCase()
//    {
//        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//        db.setDatabaseName("../record/tests/test_res/sport_people.db");
//        QVERIFY(db.open());
//    }

    void test_savePerson()
    {
        DbManager db(dbPath);

        Person artem;
        artem.setEditableList({"Артем", "Александрович", "Оношко", "12.01.1998", "плавание", "+7 888 888 88"});

        QVERIFY(db.saveSportsman(artem));
    }


};


