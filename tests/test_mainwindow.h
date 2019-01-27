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
        QList<Person> testTrainers;

        Person ivan("Иван", "Вытовтов", "10.02.1997", "лох", true);
        ivan.id = 5;
        testTrainers << ivan;

        Person vadim("Вадим", "Сурков", "26.04.1997", "реп", true);
        vadim.id = 2;
        testTrainers << vadim;


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
        // test 1 - test recording

        // make the data
        QList<Lesson> lessions;

        // make first lession
        Person ivan("Иван", "Вытовтов", "10.02.1997", "лох", true);
        ivan.id = 5;
        Person oleg("Олег", "Полушин", "хх.хх.1995", "мошенник", false);
        oleg.id = 2;
        Lesson me(ivan, oleg);
        me.id = 10;

        lessions << me;

        // make second lession
        Person vadim("Вадим", "Сурков", "26.04.1997", "реп", true);
        vadim.id = 3;
        Person artem("Артем", "Оношко", "12.01.1997", "лох", false);
        artem.id = 1;
        Lesson vad(vadim, artem);
        vad.id = 11;

        lessions << vad;

        MainWindow mw;
        mw.show();

        // run
        mw.updateSchedule(lessions);


        QCOMPARE(mw.ui->relationTable->item(0, 1)->text(), "Иван Вытовтов");
        QCOMPARE(mw.ui->relationTable->item(0, 2)->text(), "Олег Полушин");
        QCOMPARE(mw.ui->relationTable->item(1, 1)->text(), "Вадим Сурков");
        QCOMPARE(mw.ui->relationTable->item(1, 2)->text(), "Артем Оношко");


        // test 2 - it must only update (not add)

        // make the data
        QList<Lesson> lessions_for_update;

        Person kirya("Кирилл", "Лукьяновский", "05.06.1997", "что-то поломалось", false);
        kirya.id = 6;

        lessions_for_update << Lesson(ivan, kirya);

        // run
        mw.updateSchedule(lessions_for_update);


        QCOMPARE(mw.ui->relationTable->item(0, 1)->text(), "Иван Вытовтов");
        QCOMPARE(mw.ui->relationTable->item(0, 2)->text(), "Кирилл Лукьяновский");
    }
};





