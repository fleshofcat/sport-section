#include "main_test.h"

bool MainTest::runAllTests()
{
    TestDbManager test_db;

    QTest::qExec(&test_db);

    return true;
}
