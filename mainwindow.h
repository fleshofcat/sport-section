#pragma once

#include <QResizeEvent> // TODO provide code review here
#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>

#include "ui_mainwindow.h"
#include "db_manager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr) :
        QMainWindow(parent)
    {
        ui = new Ui::MainWindow;
        ui->setupUi(this);

//        ui->childrenTable->horizontalHeader()->     ;setResizeMode( 0, QHeaderView::Stretch );

        ui->childrenTable->horizontalHeader()->setVisible(true);
        ui->childrenTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        ui->trainerTable->horizontalHeader()->setVisible(true);
        ui->trainerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    }


    void resizeEvent(QResizeEvent *resizeEvent)
    {
        ui->centralWidget->setMaximumSize(resizeEvent->size());
        ui->centralWidget->resize(resizeEvent->size());

        ui->tabWidget->setMaximumSize(resizeEvent->size());
        ui->tabWidget->resize(resizeEvent->size());

//                ui->childrenTab->setMaximumSize(resizeEvent->size());
//                ui->childrenTab->resize(resizeEvent->size());

        ui->childrenTable->setMaximumSize(resizeEvent->size());
        ui->childrenTable->resize(resizeEvent->size());

        ui->trainerTable->setMaximumSize(resizeEvent->size());
        ui->trainerTable->resize(resizeEvent->size());
    }


    void fillChildren()
    {
        DbManager storage("../record/res/sport_people.db");

        auto people = storage.getAllPersons();

        QStandardItemModel model;

        for (Person pers: *people)
        {
            model.appendRow({new QStandardItem(pers.id),
                             new QStandardItem(pers.firstName),
                             new QStandardItem(pers.lastName),
                             new QStandardItem(pers.birthday),
                             new QStandardItem(pers.sportType) });
        }


        QStandardItem item;

        model.appendRow(new QStandardItem("qwerty"));
        model.appendRow()
    }


    ~MainWindow()
    {
        delete ui;
    }

private:
    Ui::MainWindow *ui;
};


