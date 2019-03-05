#pragma once

#include <QtTest/QtTest>
#include "ui/schedule_presenter.h"

class TestSchedulePresenter : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        qRegisterMetaType<Schedule>("Schedule");
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

    QList<Schedule> testSchedules()
    {
        Schedule q(Schedule::Event::EMPTY, "00.00.0000", "q");

        Schedule sh(Schedule::Event::TRAINING, "10.02.1997", "climbing");
        sh.groups << Group(1) << Group(2);

        Schedule sh_2(Schedule::Event::EMPTY, "12.12.2023", "the");
        sh_2.groups << Group(3);

        return {q, sh, sh_2};
    }

};







































