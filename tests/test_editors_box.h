#pragma once

#include <QtTest/QtTest>
#include "ui/widgets/editors_box.h"

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
        EditorsBox *edBox = new EditorsBox({"q", "w", "e"}, {});
//        EditorsBox edBox({"q", "w", "e"}, {}); // 7
//        EditorsBox edBox({"q", "w"}, {"e"}); // 5
//        EditorsBox edBox({"q"}, {"e"}); // 3
//        EditorsBox edBox; // 1

        edBox->show();

    }

};






