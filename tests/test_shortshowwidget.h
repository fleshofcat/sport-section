#pragma once

#include <QtTest/QtTest>
#include "ui/widgets/string_table_viewer.h"

class TestStringListViewer : public QObject
{
    Q_OBJECT

private slots:
    void test_constructor()
    {
        /*
        QList<QString> pattern = {"first", "second", "name"};

        QList<QList<QString>> data;

        data.append({"qaz", "wsx", "edc"});
        data.append({"rfv", "tgb", "yhn"});
        data.append({"ujm", "ik,", "ol."});

//        StringTableViewer viewer(pattern, data);

        auto viewer = new StringTableViewer(pattern, data);

        QSignalSpy spy(viewer, &StringTableViewer::createRecordIsRequred);


        QTest::mouseClick(viewer->table, Qt::LeftButton, Qt::KeyboardModifiers(), );



        QCOMPARE(spy.count(), 1);

        auto arguments = spy.takeFirst();

        QCOMPARE(arguments.at(0).toList().at(0), "qaz");


*/
    }

};






