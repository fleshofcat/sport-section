#pragma once

#include <QtTest/QtTest>
#include "ui/groups_editor.h"
#include "db/db_manager.h"

class TestGroupEditor : public QObject
{
    Q_OBJECT

public:
    void manualTest()
    {
        DbManager db("../record/res/sport_people.db");

        auto groupEditor = new GroupEditor();
        groupEditor->show();

        groupEditor->updateContent(
                    db.getTrainers(),
                    db.getSportsmen(),
                    db.getGroups()[0]);

    }

private slots:
    void test_editGroup()
    {
        auto groupEditor = new GroupEditor();
        groupEditor->show();

        qRegisterMetaType<Group>("Group");

        QSignalSpy spy(groupEditor, &GroupEditor::saveIsRequred);

        // fill data
        groupEditor->updateContent(
                    getTestTrainers(),
                    getTestSportsmen(),
                    getTestGroups()[0]);


        groupEditor->saveButton->clicked();

        QCOMPARE(spy.count(), 1);
    }

    void test_updateContent()
    {
        auto traiers = getTestTrainers();
        auto sportsmen = getTestSportsmen();
        auto groups = getTestGroups();



        GroupEditor editor(traiers, sportsmen, groups.at(0));
    }

private:
    QList<Person> getTestTrainers()
    {
        Person artem({
                         "Артем",
                         "Александрович",
                         "Оношко",
                         "12.01.1998",
                         "плавание"
                     });
        artem.id = 1;

        Person oleg({
                        "Олег",
                        "Павлович",
                        "Полушин",
                        "хх.хх.1995",
                        "мошенник"
                    });
        oleg.id = 2;

        return {artem, oleg};
    }

    QList<Person> getTestSportsmen()
    {
        Person ivan({
                        "Иван",
                        "Владимирович",
                        "Вытовтов",
                        "10.02.1997",
                        "скалолазание"
                    });
        ivan.id = 5;

        Person vadim({
                         "Вадим",
                         "Александрович",
                         "Сурков",
                         "26.04.1997",
                         "плавание"
                     });
        vadim.id = 3;

        return {ivan, vadim};
    }

    QList<Group> getTestGroups()
    {
        Group group({"Группа 1", "плавание"});
        group.trainers_ids << 3 << 5;
        group.sportsmen_ids << 1 << 2;

        Group empty_group({"q", "q"});

        return {group, empty_group};
    }

    bool isGroupEqual(Group expected, Group actual)
    {
        return expected.id == actual.id
                && isListsEqual(expected.getInList(), actual.getInList())
                && isListsEqual(expected.trainers_ids, actual.trainers_ids)
                && isListsEqual(expected.sportsmen_ids, actual.sportsmen_ids);
    }

    template <class T>
    bool isListsEqual(T expected, T actual)
    {
        for (int i = 0; i < expected.count(); ++i)
        {
            if (expected.at(i) != actual.at(i))
            {
                return false;
            }
        }
        return true;
    }
};






























