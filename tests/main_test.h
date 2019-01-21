#pragma once

#include <QtTest/QtTest>

#include "test_people_storage.h"
#include "test_relation_storage.h"

class MainTest : QObject
{
    Q_OBJECT
public:
    MainTest() {}

    static void runAllTests(int argc, char *argv[])
    {
        QTest::qExec(new TestPeopleStorage, argc, argv);
        QTest::qExec(new TestRelationStorage, argc, argv);
    }

};
