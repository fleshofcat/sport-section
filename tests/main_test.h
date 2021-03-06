#pragma once

#include <QtTest/QtTest>

#include "test_db/test_group_manager.h"
#include "test_db/test_db_manager.h"
#include "test_db/test_schedule_manager.h"

#include "test_ui/test_schedule_editor.h"

class MainTest : QObject
{
    Q_OBJECT
public:
    MainTest() {}

    static void runAllTests(int argc, char *argv[])
    {
        QTest::qExec(new TestDbManager, argc, argv);
        QTest::qExec(new TestGroupManager, argc, argv);
        QTest::qExec(new TestScheduleManager, argc, argv);

        QTest::qExec(new TestScheduleEditor, argc, argv);
    }
};

