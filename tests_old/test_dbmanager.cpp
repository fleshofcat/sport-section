#include "test_dbmanager.h"

void TestDbManager::initTestCase()
{
    // database connection
    db = new DbManager("../record/tests/test_res/people.db", this);
}

void TestDbManager::test_addPerson()
{
    DbManager db("../record/tests/test_res/people.db");

    db.addPerson("qwerty", "poiuy", "xx.xx.xxxx", "tst record123");

    QSqlQuery query_id("SELECT MAX(id) FROM people");

    bool query_id_next = query_id.next();
    QCOMPARE(query_id_next, true);

    int id = query_id.value("id").toInt();
    query_id.clear();
//    int id = 22;

    Person * pers = db.getPerson(30);

    int l = 0;
    QCOMPARE(pers->firstName, "qwerty");
    QCOMPARE(pers->lastName, "poiuy");
    QCOMPARE(pers->birthday, "xx.xx.xxxx");
    QCOMPARE(pers->description, "tst record123");

    delete pers;
}


