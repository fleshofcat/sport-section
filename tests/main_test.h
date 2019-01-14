#pragma once

#include <QtTest/QtTest>

#include <tests/test_dbmanager.h>

class MainTest : QObject
{
    Q_OBJECT
public:
    MainTest() {}

    static bool runAllTests(); // TODO

};
