#pragma once

#include <QtTest/QtTest>
#include "ui/groups_tab.h"
#include "db/db_manager.h"

class TestGroupEditor : public QObject
{
    Q_OBJECT

public:
    void manualTest()
    {
//        initTestCase();
        DbManager *db = new DbManager("../record/res/sport_people.db", this);

        auto groupsTab = new GroupsTab;
        groupsTab->show();

    //        auto groupEditor = new GroupEditor();
    //        groupEditor->show();

    //        groupEditor->updateContent(
    //                    *db->getTrainers(),
    //                    *db->getSportsmen(),
    //                    db->getGroups()[0]);
    }

private slots:
    void initTestCase()
    {

    }

};






