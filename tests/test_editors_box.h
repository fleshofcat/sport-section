#pragma once

#include <QtTest/QtTest>
#include "ui/widgets/group_editor.h"
#include "db/db_manager.h"

class TestEditorsBox : public QObject
{
    Q_OBJECT

public:
    void manualTesting()
    {
        test_updateContent();
    }

private slots:
    void test_updateContent()
    {
        auto db = new DbManager("../record/res/sport_people.db", this);

        auto groups = *db->getGroups();

        GroupEditor *edBox = new GroupEditor(*db->getTrainers(),
                                             *db->getSportsmen(),
                                             Group());

//        auto edBox = new EditorsBox({"q", "w", "e"}, {"e"}); // 7


//        auto strLstEd = new StringListEditor({"q", "w", "e"}, {});
//        strLstEd->show();

//        EditorsBox edBox({"q", "w"}, {"e"}); // 5
//        EditorsBox edBox({"q"}, {"e"}); // 3
//        EditorsBox edBox; // 1

        edBox->show();

    }

};






