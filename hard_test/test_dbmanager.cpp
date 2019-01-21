#include "test_dbmanager.h"

void TestDbManager::initTestCase()
{
    // database connection
//    db = new DbManager("../record/hard_test/test_res/people.db", this);
}

void TestDbManager::test_addPerson()
{
    // preparing

    QString dbPath = "../record/hard_test/test_res/people.db";

    Person expectedPers("qwerty", "poiuy", "xx.xx.xxxx", "tst record123");
    DbManager db(dbPath);

//    QSqlDatabase testDb = QSqlDatabase::addDatabase("QSQLITE");
//    testDb.setDatabaseName(dbPath);
//    testDb.open();

    // test execution and result by db.getPerson

    QCOMPARE(db.addPerson(expectedPers), true);

    QSqlQuery query_id("SELECT MAX(id) FROM people");

    int id = -1;
    while (query_id.next())
    {
        id = query_id.value("id").toInt();
        query_id.clear();
    }

    Person * recivedPers = db.getPerson(id);

    QCOMPARE(recivedPers->id, id);
    QCOMPARE(recivedPers->firstName, expectedPers.firstName);
    QCOMPARE(recivedPers->lastName, expectedPers.lastName);
    QCOMPARE(recivedPers->birthday, expectedPers.birthday);
    QCOMPARE(recivedPers->sportType, expectedPers.sportType);

    // clean up
//    testDb.close();
    delete recivedPers;
}


