#pragma once

#include <QtTest/QtTest>
#include <src/db/db_manager.h>

class TestDbManager : public QObject
{
    Q_OBJECT

    DbManager *db;

private slots:
    void initTestCase();
    void test_addPerson();
};


