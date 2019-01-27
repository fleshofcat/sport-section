#pragma once

#include <QResizeEvent> // TODO provide code review here
#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QtDebug>

#include "ui_mainwindow.h"
#include "db_manager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class TestMainWindow;
private:
    DbManager *db = new DbManager("../record/res/sport_people.db", this);

public:
    explicit MainWindow(QWidget *parent = nullptr) :
        QMainWindow(parent)
    {
        ui = new Ui::MainWindow;
        ui->setupUi(this);

        ui->childrenTable->horizontalHeader()->setVisible(true);
        ui->childrenTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        ui->trainerTable->horizontalHeader()->setVisible(true);
        ui->trainerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        ui->relationTable->horizontalHeader()->setVisible(true);
        ui->relationTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



//        if (people != nullptr && relations != nullptr) // TODO test return *nullptr;


    }


    void resizeEvent(QResizeEvent *resizeEvent)
    {
        ui->centralWidget->setMaximumSize(resizeEvent->size());
        ui->centralWidget->resize(resizeEvent->size());

        ui->tabWidget->setMaximumSize(resizeEvent->size());
        ui->tabWidget->resize(resizeEvent->size());

        ui->childrenTable->setMaximumSize(resizeEvent->size());
        ui->childrenTable->resize(resizeEvent->size());

        ui->trainerTable->setMaximumSize(resizeEvent->size());
        ui->trainerTable->resize(resizeEvent->size());

        ui->relationTable->setMaximumSize(resizeEvent->size());
        ui->relationTable->resize(resizeEvent->size());
    }


    void updateChildren(QList<Person> children)
    {
        ui->childrenTable->clearContents();
        ui->childrenTable->setRowCount(0);

        for (Person pers : children)
        {
            ui->childrenTable->setRowCount(ui->childrenTable->rowCount() + 1);


            ui->childrenTable->setItem(ui->childrenTable->rowCount() - 1, 0,
                                       new QTableWidgetItem(QString::number(pers.id)));



            ui->childrenTable->setItem(ui->childrenTable->rowCount() - 1, 1,
                                       new QTableWidgetItem(pers.firstName));

            ui->childrenTable->setItem(ui->childrenTable->rowCount() - 1, 2,
                                       new QTableWidgetItem(pers.lastName));

            ui->childrenTable->setItem(ui->childrenTable->rowCount() - 1, 3,
                                       new QTableWidgetItem(pers.birthday));

            ui->childrenTable->setItem(ui->childrenTable->rowCount() - 1, 4,
                                       new QTableWidgetItem(pers.sportType));

        }
    }

    void updateTrainers(QList<Person> trainers)
    {
        ui->trainerTable->clearContents();
        ui->trainerTable->setRowCount(0);

        for (Person pers : trainers)
        {
            ui->trainerTable->setRowCount(ui->trainerTable->rowCount() + 1);


            ui->trainerTable->setItem(ui->trainerTable->rowCount() - 1, 0,
                                      new QTableWidgetItem(QString::number(pers.id)));

            ui->trainerTable->setItem(ui->trainerTable->rowCount() - 1, 1,
                                      new QTableWidgetItem(pers.firstName));

            ui->trainerTable->setItem(ui->trainerTable->rowCount() - 1, 2,
                                      new QTableWidgetItem(pers.lastName));

            ui->trainerTable->setItem(ui->trainerTable->rowCount() - 1, 3,
                                      new QTableWidgetItem(pers.birthday));

            ui->trainerTable->setItem(ui->trainerTable->rowCount() - 1, 4,
                                      new QTableWidgetItem(pers.sportType));
        }
    }


    void updateSchedule(QList<Lesson> lessons)
    {
        ui->relationTable->clearContents();
        ui->relationTable->setRowCount(0);

        for (Lesson less : lessons)
        {
            ui->relationTable->setRowCount(ui->relationTable->rowCount() + 1);

            ui->relationTable->setItem(ui->relationTable->rowCount() - 1, 0,
                                      new QTableWidgetItem(QString::number(less.id)));

            ui->relationTable->setItem(ui->relationTable->rowCount() - 1, 1,
                    new QTableWidgetItem(less.trainer.firstName + " " + less.trainer.lastName));

            ui->relationTable->setItem(ui->relationTable->rowCount() - 1, 2,
                    new QTableWidgetItem(less.child.firstName + " " + less.child.lastName));
        }
    }


    ~MainWindow()
    {
        delete ui;
    }

private:
    Ui::MainWindow *ui;
};


