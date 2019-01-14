#include "main_test.h"

bool MainTest::runAllTests()
{
    QTest::qExec(new TestDbManager());

    return true;
}
