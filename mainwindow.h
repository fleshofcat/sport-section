#pragma once

#include <QResizeEvent> // TODO provide code review here
#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QtDebug>
#include <QInputDialog>
#include <QMessageBox>

#include "ui_mainwindow.h"
#include "db_manager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class TestMainWindow;
signals:
    void addPersonIsRequred(Person newPerson);
    void removePersonIsRequred(Person personForRemove);

private:
    Ui::MainWindow *ui;
    DbManager *db = new DbManager("../record/res/sport_people.db", this);
    QList<Person> people;
    QList<Lesson> lessons;

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
//        update(*db->getPeople(), *db->getLessons());

    }


    void update(QList<Person> people, QList<Lesson> lessons)
    {
        this->people = people;
        this->lessons = lessons;

        // it must check that data clear before update

        updateChildren(getChildrenFromPeople(people));
        updateTrainers(getTrainersFromPeople(people));
        updateSchedule(lessons);
    }

private slots:
    void on_addChildButton_clicked()
    {
        Person newPerson;

        newPerson.firstName = QInputDialog::getText(this, "",
            "Введите имя", QLineEdit::Normal);

        if (newPerson.firstName.isEmpty())
            return;

        newPerson.lastName = QInputDialog::getText(this, "",
            "Введите фамилию", QLineEdit::Normal);

        if (newPerson.lastName.isEmpty())
            return;

        newPerson.birthday = QInputDialog::getText(this, "",
            "Введите дату рождения", QLineEdit::Normal);

        if (newPerson.birthday.isEmpty())
            return;

        newPerson.sportType = QInputDialog::getText(this, "",
            "Введите вид спорта", QLineEdit::Normal);

        if (newPerson.sportType.isEmpty())
            return;

        newPerson.isTrainer = false;

        if (newPerson.isFull())
        {
            emit addPersonIsRequred(newPerson);
        }
    }

    void on_removeChildButton_clicked()
    {
        Person personForRemove;

        personForRemove.id = QInputDialog::getInt(
                    this, "",
                    "Введите id удаляемого ребенка",
                    QLineEdit::Normal);

        QList<Person> children = getChildrenFromPeople(this->people);

        for (Person child : children)
        {
            if (personForRemove.id == child.id)
            {
                emit removePersonIsRequred(personForRemove);
                return;
            }
        }
        QMessageBox::warning(
                    this, "Сообщение",
                    "Такого ребенка не существует",
                    QMessageBox::Ok);
    }

private:

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

    QList<Person> getTrainersFromPeople(QList<Person> people)
    {
        QList<Person> trainers;

        for (Person pers : people)
        {
            if (pers.isTrainer == true)
            {
                trainers << pers;
            }
        }
        return trainers;
    }

    QList<Person> getChildrenFromPeople(QList<Person> people)
    {
        QList<Person> children;

        for (Person pers : people)
        {
            if (pers.isTrainer == false)
            {
                children << pers;
            }
        }
        return children;
    }

public:

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

    ~MainWindow()
    {
        delete ui;
    }

};


