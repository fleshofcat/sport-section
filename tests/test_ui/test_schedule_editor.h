#pragma once

#include <QtTest/QtTest>
#include "ui/schedule_editor.h"

class TestScheduleEditor : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        qRegisterMetaType<Schedule>("Schedule");
    }

    void test_saveButton()
    {
        Schedule sch(Schedule::Event::EMPTY, "", "climbing");
        sch.group_ids << 1 << 2;

        ScheduleEditor editor(sch, testGroups());

        QSignalSpy spySave(&editor, &ScheduleEditor::needSave);
        QSignalSpy spyRemove(&editor, &ScheduleEditor::needRemove);
        QSignalSpy spyExit(&editor, &ScheduleEditor::needExit);

        // test save
        emit editor.saveButton->clicked();

        QCOMPARE(spySave.count(), 1);

        // test remove
        emit editor.removeButton->clicked();

        QCOMPARE(spyRemove.count(), 1);

        // exit remove
        emit editor.exitButton->clicked();

        QCOMPARE(spyExit.count(), 1);


        // conclusion
        QCOMPARE(spySave.count(), 1);
        QCOMPARE(spyRemove.count(), 1);
        QCOMPARE(spyExit.count(), 1);
    }

public:
    void test_removeGroup() // manualy test
    {
        Schedule sch(Schedule::Event::EMPTY, "", "climbing");
        sch.group_ids << 1;

        ScheduleEditor editor(sch, testGroups());

        QSignalSpy spySave(&editor, &ScheduleEditor::needSave);
        QSignalSpy spyRemove(&editor, &ScheduleEditor::needRemove);
        QSignalSpy spyExit(&editor, &ScheduleEditor::needExit);

        // test
        emit editor.groupsViewer->recordActivated(0);
        emit editor.saveButton->clicked();

        QCOMPARE(spySave.first().first().value<Schedule>().group_ids.count(), 0);

        QCOMPARE(spySave.count(), 1);
        QCOMPARE(spyRemove.count(), 0);
        QCOMPARE(spyExit.count(), 0);
    }

    void test_addGroup() // manual testing
    {
        Schedule sch(Schedule::Event::EMPTY, "", "climbing");
        sch.group_ids << 1;

        ScheduleEditor editor(sch, testGroups());

        QSignalSpy spySave(&editor, &ScheduleEditor::needSave);
        QSignalSpy spyRemove(&editor, &ScheduleEditor::needRemove);
        QSignalSpy spyExit(&editor, &ScheduleEditor::needExit);

        // test
        emit editor.groupsViewer->createRecordActivated();
        emit editor.saveButton->clicked();

        QCOMPARE(spySave.first().first().value<Schedule>().group_ids.count(), 2);

        QCOMPARE(spySave.count(), 1);
        QCOMPARE(spyRemove.count(), 0);
        QCOMPARE(spyExit.count(), 0);
    }

private:
    QList<Group> testGroups()
    {
        Group q({"q", "q"});
        Group gr(Group::pattern());
        Group gr_2({"Группа мошеннег", "мошенник"});

        q.id = 1;
        gr.id = 2;
        gr_2.id = 3;

        return {q, gr, gr_2};
    }

};







































