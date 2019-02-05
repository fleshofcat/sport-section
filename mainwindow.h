#pragma once

#include <QResizeEvent>
#include <QInputDialog>
#include <QMessageBox>

#include "ui_mainwindow.h"
#include "common_objects.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class TestMainWindow;
signals:
    void addPersonIsRequred(Person newPerson);
    void removePersonIsRequred(Person removedPerson);
    void editPersonIsRequred(Person editedPerson);

    void addScheduleRequred(Schedule newSchedule);
    void removeScheduleRequred(Schedule removedSchedule);
    void editScheduleRequred(Schedule editedSchedule);

private:
    Ui::MainWindow *ui;
    QList<Person> children;
    QList<Person> trainers;
    QList<Schedule> schedule;


public:
    explicit MainWindow(QWidget *parent = nullptr)
        : QMainWindow(parent)
    {
        ui = new Ui::MainWindow;

        ui->setupUi(this);

        ui->childrenTable->horizontalHeader()->setVisible(true);
        ui->childrenTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        ui->trainerTable->horizontalHeader()->setVisible(true);
        ui->trainerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        ui->scheduleTable->horizontalHeader()->setVisible(true);
        ui->scheduleTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    }


    void update(QList<Person> people, QList<Schedule> schedule)
    {
        QList<QList<Person>> splittedPeople = splitPeopleByChildrenTrainers(people);

        QList<Person> children = splittedPeople[0];
        QList<Person> trainers = splittedPeople[1];


        this->children = children;
        this->trainers = trainers;
        this->schedule = schedule;

        updateChildren(children);
        updateTrainers(trainers);
        updateSchedule(schedule, children, trainers);
    }


private slots:
    void on_addChildButton_clicked()
    {
        Person newPerson = getPersonDataFromUi();

        newPerson.isTrainer = false;

        if (newPerson.isFull())
        {
            emit addPersonIsRequred(newPerson);
        }
        else
        {
            QMessageBox::warning(this, "Сообщение",
                "Данные введены не полностью", QMessageBox::Ok);
        }
    }


    void on_removeChildButton_clicked()
    {
        int child_id = getIdFromUi("Введите id удаляемого ребенка");

        // проверка на отмену
        if (child_id <= 0)
            return;


        if (isPersonExistsById(child_id , this->children))
        {
            Person childForRemove;
            childForRemove.id = child_id;

            emit removePersonIsRequred(childForRemove);
            return;
        }

        QMessageBox::warning(this, "Сообщение",
            "Такого ребенка не существует", QMessageBox::Ok);
    }


    void on_editChildButton_clicked()
    {
        int child_id = getIdFromUi("Введите id изменяемого ребенка");

        // проверка на отмену
        if (child_id <= 0)
            return;


        if (isPersonExistsById(child_id , this->children))
        {
            Person editChild = getPersonDataFromUi();
            editChild.id = child_id;
            editChild.isTrainer = false;


            if (editChild.isFull())
            {
                emit editPersonIsRequred(editChild);
                return;
            }
        }
        else        // если ребенка с заданным id не существует
        {           // показывается предупреждение

            QMessageBox::warning(this, "Сообщение",
                "Такого ребенка не существует", QMessageBox::Ok);
        }
    }



    void on_addTrainerButton_clicked()
    {
        Person newPerson = getPersonDataFromUi();

        newPerson.isTrainer = true;

        if (newPerson.isFull())
        {
            emit addPersonIsRequred(newPerson);
        }
        else
        {
            QMessageBox::warning(this, "Сообщение",
                "Данные введены не полностью", QMessageBox::Ok);
        }
    }


    void on_removeTrainerButton_clicked()
    {
        int trainer_id = getIdFromUi("Введите id удаляемого тренера");

        // проверка на отмену
        if (trainer_id <= 0)
            return;


        if (isPersonExistsById(trainer_id , this->trainers))
        {
            Person removeTrainer;
            removeTrainer.id = trainer_id;

            emit removePersonIsRequred(removeTrainer);
            return;
        }

        QMessageBox::warning(this, "Сообщение",
            "Такого тренера не существует", QMessageBox::Ok);
    }


    void on_editTrainerButton_clicked()
    {
        int trainer_id = getIdFromUi("Введите id изменяемого тренера");

        // проверка на отмену
        if (trainer_id <= 0) return;


        if (isPersonExistsById(trainer_id , this->trainers))
        {
            Person editTrainer = getPersonDataFromUi();
            editTrainer.id = trainer_id;
            editTrainer.isTrainer = true;

            if (editTrainer.isFull())
            {
                emit editPersonIsRequred(editTrainer);
                return;
            }
        }
        else        // если тренера с заданным id не существует
        {           // показывается предупреждение

            QMessageBox::warning(this, "Сообщение",
                "Такого тренера не существует", QMessageBox::Ok);
        }
    }



    void on_addScheduleButton_clicked()
    {
        // Ввод ребенка
        int child_id = getIdFromUi("Введите id ребенка");

        if (child_id <= 0) return; // проверка на отмену

        if (isPersonExistsById(child_id, this->children) == false) // проверка что ребенок существует
        {
            QMessageBox::warning(this, "Сообщение",
                                 "Такого ребенка не существует", QMessageBox::Ok);
            return;
        }


        // Ввод тренера
        int trainer_id = getIdFromUi("Введите id тренера");

        if (trainer_id <= 0) return; // проверка на отмену

        if (isPersonExistsById(trainer_id, this->trainers) == false) // проверка что тренер существует
        {
            QMessageBox::warning(this, "Сообщение",
                                 "Такого тренера не существует", QMessageBox::Ok);
            return;
        }

        // Создание записи тренировки
        Schedule sched(child_id, trainer_id);
        emit addScheduleRequred(sched);
    }


    void on_removeScheduleButton_clicked()
    {
        int sched_id = getIdFromUi("Введите id удаляемой записи");

        // проверка на отмену
        if (sched_id <= 0) return;


        if (isScheduleExistsById(sched_id , this->schedule))
        {
            Schedule removeSched;
            removeSched.id = sched_id;

            emit removeScheduleRequred(removeSched);
            return;
        }

        QMessageBox::warning(this, "Сообщение",
            "Такой записи не существует", QMessageBox::Ok);
    }


    void on_editScheduleButton_clicked()
    {
        // получение id изменяемой записи
        int sched_id = getIdFromUi("Введите id изменяемой записи");

        if (sched_id <= 0) // проверка на отмену
        {
            return;
        }
        else if (isScheduleExistsById(sched_id, this->schedule) == false)
        {
            QMessageBox::warning(this, "Сообщение",
                                 "Такой записи не существует", QMessageBox::Ok);
            return;
        }



        // получение id ребенка

        int child_id = getIdFromUi("Введите id ребенка");

        if (child_id <= 0) // проверка на отмену
        {
            return;
        }
        else if (isPersonExistsById(child_id, this->children) == false)
        {
            QMessageBox::warning(this, "Сообщение",
                                 "Такого ребенка не существует", QMessageBox::Ok);
            return;
        }



        // получение id тренера

        int trainer_id = getIdFromUi("Введите id тренера");

        if (trainer_id <= 0) // проверка на отмену
        {
            return;
        }
        else if (isPersonExistsById(trainer_id, this->trainers) == false)
        {
            QMessageBox::warning(this, "Сообщение",
                                 "Такого тренера не существует", QMessageBox::Ok);
            return;
        }

        // редактирование записи тренировки
        Schedule editedSched(child_id, trainer_id);
        editedSched.id = sched_id;
        emit editScheduleRequred(editedSched);
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

    // обновление расписания
    void updateSchedule(QList<Schedule> schedule,
                        QList<Person> children, QList<Person> trainers)
    {
        ui->scheduleTable->clearContents(); // удаление старых элементов
        ui->scheduleTable->setRowCount(0);  // из таблицы


        // для каждой записи в массиве расписаний выполняются действия
        for (Schedule sched : schedule)
        {
            Person *trainer = getPersonById(sched.trainer_id, trainers);           // ребенок и тренер
            Person *child = getPersonById(sched.child_id, children);               // у которых занятие

            ui->scheduleTable->setRowCount(ui->scheduleTable->rowCount() + 1);  // создание дополнительного рва

            ui->scheduleTable->setItem(ui->scheduleTable->rowCount() - 1, 0,    // запись id занятия
                                       new QTableWidgetItem(QString::number(sched.id)));

            ui->scheduleTable->setItem(ui->scheduleTable->rowCount() - 1, 1,    // запись тренера
                    new QTableWidgetItem(trainer->firstName + " " + trainer->lastName));

            ui->scheduleTable->setItem(ui->scheduleTable->rowCount() - 1, 2,    // запись ребенка
                    new QTableWidgetItem(child->firstName + " " + child->lastName));
        }
    }


    QList<QList<Person>> splitPeopleByChildrenTrainers(QList<Person> people)
    {
        QList<Person> children;
        QList<Person> trainers;

        for (Person pers : people)
        {
            if (pers.isTrainer == true)
            {
                trainers << pers;
            }
            else
            {
                children << pers;
            }
        }
        QList<QList<Person>> splittedPeople;

        splittedPeople << children;
        splittedPeople << trainers;

        return splittedPeople;
    }


    Person getPersonDataFromUi() // TO REDO
    {
        Person retPerson;

        retPerson.firstName = QInputDialog::getText(this, "",
            "Введите имя", QLineEdit::Normal);

        if (retPerson.firstName.isEmpty())
            return retPerson;

        retPerson.lastName = QInputDialog::getText(this, "",
            "Введите фамилию", QLineEdit::Normal);

        if (retPerson.lastName.isEmpty())
            return retPerson;

        retPerson.birthday = QInputDialog::getText(this, "",
            "Введите дату рождения", QLineEdit::Normal);

        if (retPerson.birthday.isEmpty())
            return retPerson;

        retPerson.sportType = QInputDialog::getText(this, "",
            "Введите вид спорта", QLineEdit::Normal);

        return retPerson;
    }


    int getIdFromUi(QString message = "Введите id")
    {
        Person pers;
        int id = -1;
        bool success;

        id = QInputDialog::getInt(this, "", message,
                    QLineEdit::Normal, 1, 0xfffffff, 1, &success);

        if (success)
        {
            return id;
        }

        return -1;
    }


    bool isScheduleExistsById(int sched_id, QList<Schedule> schedule)
    {
        for (Schedule sched : schedule)
        {
            if (sched.id == sched_id)
            {
                return true;
            }
        }

        return false;
    }


    bool isPersonExistsById(int pers_id, QList<Person> people)
    {
        for (Person pers : people)
        {
            if (pers.id == pers_id)
            {
                return true;
            }
        }

        return false;
    }


    Person *getPersonById(int id, QList<Person> people)
    {
        for (Person pers : people)
        {
            if (pers.id == id)
            {
                return new Person(pers);
            }
        }

        return nullptr;
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

        ui->scheduleTable->setMaximumSize(resizeEvent->size());
        ui->scheduleTable->resize(resizeEvent->size());
    }

    ~MainWindow()
    {
        delete ui;
    }

};


