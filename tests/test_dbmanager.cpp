#include "test_dbmanager.h"

void TestDbManager::initTestCase()
{
    // database connection
//    DbManager db("../record/tests/test_res/people.db");
    db = new DbManager("../record/tests/test_res/people.db", this);
}

void TestDbManager::test_addPerson()
{

}


