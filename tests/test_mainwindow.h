#pragma once

#include <QtTest/QtTest>
#include "mainwindow.h"

class TestMainWindow : public QObject
{
    Q_OBJECT

private slots:
    void test_updateChildren()
    {
        // prepare

        // make the data
        QList<Person> testChildren = getTestChildren();

        // test method
        MainWindow mw;
        mw.show();

        mw.updateChildren(testChildren);

        // compare

        QCOMPARE("Артем", mw.ui->childrenTable->takeItem(0, 1)->text());
        QCOMPARE("1", mw.ui->childrenTable->takeItem(0, 0)->text());
        QCOMPARE("Полушин", mw.ui->childrenTable->takeItem(1, 2)->text());
        QCOMPARE("сломлен но не отрезан", mw.ui->childrenTable->takeItem(2, 4)->text());
    }

    void test_updateTrainers()
    {
        // prepare

        // make the data
        QList<Person> testTrainers = getTestTrainers();

        // test method
        MainWindow mw;
        mw.show();

        mw.updateTrainers(testTrainers);

        // compare
        QCOMPARE("Иван", mw.ui->trainerTable->takeItem(0, 1)->text());
        QCOMPARE("5", mw.ui->trainerTable->takeItem(0, 0)->text());
        QCOMPARE("реп", mw.ui->trainerTable->takeItem(1, 4)->text());
    }

    void test_updateSchedule()
    {
        // make children

        Person oleg("Олег", "Полушин", "хх.хх.1995", "мошенник", false);
        oleg.id = 2;

        Person artem("Артем", "Оношко", "12.01.1997", "лох", false);
        artem.id = 1;

        QList<Person> children;
        children << oleg;
        children << artem;


        // make trainers

        Person ivan("Иван", "Вытовтов", "10.02.1997", "лох", true);
        ivan.id = 5;

        Person vadim("Вадим", "Сурков", "26.04.1997", "реп", true);
        vadim.id = 3;

        QList<Person> trainers;
        trainers << ivan;
        trainers << vadim;


        // make Schedule

        Schedule first_training(oleg.id, ivan.id);
        first_training.id = 5;

        Schedule second_training(artem.id, vadim.id);
        second_training.id = 1;

        QList<Schedule> schedule;
        schedule << first_training;
        schedule << second_training;


        MainWindow mw;
        mw.show();

        // run
        mw.updateSchedule(schedule, children, trainers);


        QCOMPARE(mw.ui->scheduleTable->item(0, 1)->text(), "Иван Вытовтов");
        QCOMPARE(mw.ui->scheduleTable->item(0, 2)->text(), "Олег Полушин");
        QCOMPARE(mw.ui->scheduleTable->item(1, 1)->text(), "Вадим Сурков");
        QCOMPARE(mw.ui->scheduleTable->item(1, 2)->text(), "Артем Оношко");


        // test 2 - it must only update (not add)

        Person kirya("Кирилл", "Лукьяновский", "05.06.1997", "что-то поломалось", false);
        kirya.id = 6;
        children << kirya;

        Schedule third_training(kirya.id, ivan.id);

        QList<Schedule> schedule_for_update;
        schedule_for_update << third_training;

        // run
        mw.updateSchedule(schedule_for_update, children, trainers);

        // compare
        QCOMPARE(mw.ui->scheduleTable->item(0, 1)->text(), "Иван Вытовтов");
        QCOMPARE(mw.ui->scheduleTable->item(0, 2)->text(), "Кирилл Лукьяновский");
    }

    void test_update()
    {
        MainWindow mw;
        mw.show();

        // run test method
        mw.update(getTestPeople(), getTestSchedule());

        // compare
        // trainers
        QCOMPARE("Иван", mw.ui->trainerTable->takeItem(0, 1)->text());
        QCOMPARE("5", mw.ui->trainerTable->takeItem(0, 0)->text());
        QCOMPARE("реп", mw.ui->trainerTable->takeItem(1, 4)->text());

        // children
        QCOMPARE("Артем", mw.ui->childrenTable->takeItem(0, 1)->text());
        QCOMPARE("1", mw.ui->childrenTable->takeItem(0, 0)->text());
        QCOMPARE("Полушин", mw.ui->childrenTable->takeItem(1, 2)->text());
        QCOMPARE("сломлен но не отрезан", mw.ui->childrenTable->takeItem(2, 4)->text());

        // schedule
        QCOMPARE(mw.ui->scheduleTable->item(0, 1)->text(), "Иван Вытовтов");
        QCOMPARE(mw.ui->scheduleTable->item(0, 2)->text(), "Олег Полушин");
        QCOMPARE(mw.ui->scheduleTable->item(1, 1)->text(), "Вадим Сурков");
        QCOMPARE(mw.ui->scheduleTable->item(1, 2)->text(), "Артем Оношко");


        // test 2. It must only update(not add)

        Person kirya("Кирилл", "Лукьяновский", "05.06.1997", "что-то поломалось", false);
        kirya.id = 6;

        QList<Person> peopleForUpdate;
        peopleForUpdate << kirya;

        QList<Schedule> scheduleForUpdate;

        // run test method
        mw.update(peopleForUpdate, scheduleForUpdate);

        // compare
        // trainers
        QCOMPARE(0, mw.ui->trainerTable->rowCount());

        // children
        QCOMPARE(1, mw.ui->childrenTable->rowCount());
        QCOMPARE("6", mw.ui->childrenTable->takeItem(0, 0)->text());
        QCOMPARE("Кирилл", mw.ui->childrenTable->takeItem(0, 1)->text());
        QCOMPARE("что-то поломалось", mw.ui->childrenTable->takeItem(0, 4)->text());

        // schedule
        QCOMPARE(0, mw.ui->scheduleTable->rowCount());
    }

   // signals and slots tests implemented via the ass
   // and it isn't using by default

#if 0 // manual testing
    void test_signal_addPersonIsRequred()
    {
        MainWindow mw;
        mw.show();

        qRegisterMetaType<Person>("Person");

        QSignalSpy spy(&mw, &MainWindow::addPersonIsRequred);

        // it must provoke the signal if all dialogs will be filled
//        QTest::mouseClick(mw.ui->addChildButton, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0));
        QTest::mouseClick(mw.ui->addTrainerButton, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0));


        QCOMPARE(spy.count(), 1);
    }
#endif

#if 0 // manual testing
    void test_signal_removePersonIsRequred()
    {
        MainWindow mw;
        mw.show();

        mw.update(getTestPeople(), getTestSchedule());

        qRegisterMetaType<Person>("Person");

        QSignalSpy spy(&mw, &MainWindow::removePersonIsRequred);

        // it must provoke the signal

//        QTest::mouseClick(mw.ui->removeChildButton, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0));
        QTest::mouseClick(mw.ui->removeTrainerButton, Qt::LeftButton, Qt::NoModifier, QPoint(0, 0));

        // check the signal is emitted
        QCOMPARE(spy.count(), 1);
    }
#endif

#if 0 // manual testing
    void test_signal_editPersonIsRequred()
    {
        MainWindow mw;
        mw.update(getTestPeople(), getTestSchedule());

        mw.show();

        qRegisterMetaType<Person>("Person");

        QSignalSpy spy(&mw, &MainWindow::editPersonIsRequred);

        // it must provoke the signal

        //        QTest::mouseClick(mw.ui->editChildButton, Qt::LeftButton,
        //                          Qt::NoModifier, QPoint(0, 0));

        QTest::mouseClick(mw.ui->editTrainerButton, Qt::LeftButton,
                          Qt::NoModifier, QPoint(0, 0));

        // check the signal is emitted
        QCOMPARE(spy.count(), 1);
    }

#endif


private:
    QList<Person> getTestPeople()
    {
        QList<Person> children = getTestChildren();
        QList<Person> trainers = getTestTrainers();

        QList<Person> people;

        people << children;
        people << trainers;

        return people;
    }

    QList<Person> getTestTrainers()
    {
        QList<Person> testTrainers;

        Person ivan("Иван", "Вытовтов", "10.02.1997", "лох", true);
        ivan.id = 5;
        testTrainers << ivan;

        Person vadim("Вадим", "Сурков", "26.04.1997", "реп", true);
        vadim.id = 4;
        testTrainers << vadim;

        return testTrainers;
    }

    QList<Person> getTestChildren()
    {
        QList<Person> testChildren;

        Person artem("Артем", "Оношко", "12.01.1997", "лох", false);
        artem.id = 1;
        testChildren << artem;

        Person oleg("Олег", "Полушин", "хх.хх.1995", "мошенник", false);
        oleg.id = 2;
        testChildren << oleg;

        Person kirill("Кирилл", "Лукьяновский", "05.06.1997", "сломлен но не отрезан", false);
        kirill.id = 3;
        testChildren << kirill;

        return testChildren;
    }

    QList<Schedule> getTestSchedule()
    {
        QList<Schedule> schedule;

        Schedule sched(2, 5);
        Schedule sched_2(1, 4);

        schedule << sched;
        schedule << sched_2;

        return schedule;
    }

};





