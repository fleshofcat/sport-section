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

        QList<Person> *people = db->getAllPersons();
        showPeople(people);
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
    }


    void showPeople(QList<Person> *people)
    {

        for (Person pers : *people)
        {
            if (pers.isTrainer == true)
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
            else
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

    }


    ~MainWindow()
    {
        delete ui;
    }

private:
    Ui::MainWindow *ui;
};


