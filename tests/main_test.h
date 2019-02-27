#pragma once

#include <QtTest/QtTest>

#include "test_db/test_people_storage.h"
#include "test_db/test_group_manager.h"
#include "test_db/test_db_manager.h"
#include "test_group_editor.h"
#include "test_editors_box.h"

class MainTest : QObject
{
    Q_OBJECT
public:
    MainTest() {}

    static void runAllTests(int argc, char *argv[])
    {
//        QTest::qExec(new TestPeopleStorage, argc, argv);
//        QTest::qExec(new TestGroupStorage, argc, argv);
//        QTest::qExec(new TestDbManager, argc, argv);
        QTest::qExec(new TestGroupEditor, argc, argv);
//        QTest::qExec(new TestEditorsBox, argc, argv);
    }

};
