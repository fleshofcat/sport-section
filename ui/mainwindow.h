#pragma once

#include <QResizeEvent> // для корректной обработки изменения размера окна приложения
#include <QInputDialog> // для диалоговых окон с пользователями
#include <QMessageBox>  // для вывода предупреждений пользователю

#include "ui_mainwindow.h"  // автогенерируемый файл для отображения графики
#include "common/common_objects.h" // файл с общими объектами (Person, Schedule)
#include "ui/personeditor.h"

namespace Ui {      // помещение данного класса в пространство имен Ui
class MainWindow;
}

// класс MainWindow/ГлавноеОкно является классом-прослойкой
// между пользователем и программой
//
// он показывает пользователю информацию
// принимает и обрабатывает его запросы
class MainWindow : public QMainWindow
{
    Q_OBJECT    // обязательный макрос

    friend class TestMainWindow; // класс для тестирования данного класса // TO RM
signals:
    void savePersonIsRequred(Person savedPerson);       // сигналы, испускаемые
    void removePersonIsRequred(int id);                 // этим объектом обрабатывают
                                                        // добавление, редактирование,
    void addScheduleRequred(Schedule newSchedule);      // удаление людей и расписаний
    void removeScheduleRequred(Schedule removedSchedule);
    void editScheduleRequred(Schedule editedSchedule);

private:
    PersonEditor *personEditor = nullptr;
    Ui::MainWindow *ui;         // объект для прямого взаимодействия с пользователем
    QList<Person> children;     // объект для хранения детей
    QList<Person> trainers;     // объект для хранения тренеров
    QList<Schedule> schedule;   // объект для хранения расписаний


public:
    // код который будет выполняться при создании объекта от этого класса
    // имеет 1 не обязательный системный параметр
    explicit MainWindow(QWidget *parent = nullptr)
        : QMainWindow(parent)
    {
        ui = new Ui::MainWindow;    // создание объекта взаимодействия с пользователем

        ui->setupUi(this);      // поднять графический интерфейс

        // ниже устанавливается режим показа таблиц
        // они будет показывать элементы в режиме авторастягивания

        ui->childrenTable->horizontalHeader()->setVisible(true);
        ui->childrenTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        ui->trainerTable->horizontalHeader()->setVisible(true);
        ui->trainerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        ui->scheduleTable->horizontalHeader()->setVisible(true);
        ui->scheduleTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


        connect(ui->childrenTable, &QTableWidget::cellPressed, this, &MainWindow::on_childrenTablePressed);

//        connect(this, &MainWindow:, personEditor, &PersonEditor::re);
        setStatusBar(nullptr);
    }


    // метод с помощью которого данные от бд
    // будут загружаться в данный класс и отображаться пользователю
    void update(QList<Person> children, QList<Person> trainers, QList<Schedule> schedule)
    {
        // данные из последнего апдейта будут храниться
        // в локальных переменных этого класса
        this->children = children;
        this->trainers = trainers;
        this->schedule = schedule;

        // обновление внешнего вида программы
        updateChildren(children);
        updateTrainers(trainers);
//        updateSchedule(schedule, children, trainers);
    }


private:
    // метод для обновления списка детей пользователю
    void updateChildren(QList<Person> children)
    {
        ui->childrenTable->clear(); // сброс старых
        ui->childrenTable->setRowCount(0);  // данных перед обнавлением
        ui->childrenTable->setColumnCount(0);

        ui->childrenTable->setRowCount(children.count() + 1);  // данных перед обнавлением
        ui->childrenTable->setColumnCount(Person::getPattern().count());


        for (int c = 0; c < Person::getPattern().count(); c++)
        {
            ui->childrenTable->setHorizontalHeaderItem(
                        c, new QTableWidgetItem(Person::getPattern().at(c)));
        }


        for (int r = 0; r < children.count(); r++)
        {
            for (int c = 0; c < Person::getPattern().count(); c++)
            {
                Person pers = children.at(r);

                ui->childrenTable->setItem(
                            r, c, new QTableWidgetItem(pers.getInList().at(c)));
            }
        }


        ui->childrenTable->setSpan(ui->childrenTable->rowCount() - 1, 0, 1, ui->childrenTable->columnCount());

        ui->childrenTable->setItem(ui->childrenTable->rowCount() - 1, 0,
                                   new QTableWidgetItem("+"));
        ui->childrenTable->item(ui->childrenTable->rowCount() - 1, 0)->setTextAlignment(Qt::AlignCenter);
    }


    // метод для обновления списка тренеров пользователю
    void updateTrainers(QList<Person> trainers)
    {
        ui->trainerTable->clearContents();  // сброс старых
        ui->trainerTable->setRowCount(0);   // данных перед обнавлением


        // индивидуально "распаковать" каждого человка и показать пользователю
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


    // метод для обновления списка расписаний пользователю
    void updateSchedule(QList<Schedule> schedule,
                        QList<Person> children, QList<Person> trainers)
    {
        ui->scheduleTable->clearContents(); // сброс старых
        ui->scheduleTable->setRowCount(0);  // данных перед обнавлением


        // индивидуально "распаковать" каждое расписание и показать пользователю
        for (Schedule sched : schedule)
        {
            Person *trainer = getPersonFromList(sched.trainer_id, trainers);           // ребенок и тренер
            Person *child = getPersonFromList(sched.child_id, children);               // у которых занятие

            ui->scheduleTable->setRowCount(ui->scheduleTable->rowCount() + 1);  // создание дополнительного рва

            ui->scheduleTable->setItem(ui->scheduleTable->rowCount() - 1, 0,    // запись id занятия
                                       new QTableWidgetItem(QString::number(sched.id)));

            ui->scheduleTable->setItem(ui->scheduleTable->rowCount() - 1, 1,    // запись тренера
                    new QTableWidgetItem(trainer->firstName + " " + trainer->lastName));

            ui->scheduleTable->setItem(ui->scheduleTable->rowCount() - 1, 2,    // запись ребенка
                    new QTableWidgetItem(child->firstName + " " + child->lastName));
        }
    }

    void createPerson(PersonEditor::Who who)
    {
        personEditor = new PersonEditor(who, ui->centralWidget);
        setUpPersioEditorSetting();
    }

    void editPerson(Person pers)
    {
        personEditor = new PersonEditor(pers, ui->centralWidget);
        setUpPersioEditorSetting();
    }

    void setUpPersioEditorSetting()
    {
        ui->tabWidget->hide();
        personEditor->resize(ui->centralWidget->size());

        connect(personEditor, &PersonEditor::savePerson, this, &MainWindow::savePersonIsRequred);
        connect(personEditor, &PersonEditor::removePerson, this, &MainWindow::removePersonIsRequred);
        connect(personEditor, &PersonEditor::exitPersonEditor, this, &MainWindow::on_exitPersonEditor);
    }


private slots:

    void on_childrenTablePressed(int row, int)
    {
        if (ui->childrenTable->rowCount() == row + 1)
        {
            createPerson(PersonEditor::Who::CHILD);
        }
        else
        {
            editPerson(this->children.at(row));
        }

        setUpPersioEditorSetting();
//        ui->childrenTable->hide();
    }


    void on_exitPersonEditor()
    {
        delete personEditor;
        personEditor = nullptr;

        ui->tabWidget->show();

//        ui->trainerTable->show();
//        ui->childrenTable->show();
    }










    // обработчик кнопки добавить расписание
    void on_addScheduleButton_clicked()
    {
        // Ввод ребенка для расписания
        int child_id;// = getIdFromUser("Введите id ребенка");

        if (child_id <= 0) return; // проверка на отмену

        if (isChildExists(child_id) == false) // проверка что ребенок существует
        {
            showMessage("Такого ребенка не существует");
            return;
        }


        // Ввод тренера для расписания
        int trainer_id;// = getIdFromUser("Введите id тренера");

        if (trainer_id <= 0) return; // проверка на отмену

        if (isTrainerExists(trainer_id) == false) // проверка что тренер существует
        {
            showMessage("Такого тренера не существует");
            return;
        }

        // Создание расписания тренировки и запрос к бд на ее добавление
        Schedule sched(child_id, trainer_id);
        emit addScheduleRequred(sched);
    }


    // обработчик кнопки удалить расписание
    void on_removeScheduleButton_clicked()
    {
        // получение id удаляемого расписания
        int sched_id;// = getIdFromUser("Введите id удаляемой записи");

        // проверка на отмену
        if (sched_id <= 0) return;


        // ели расписание существует
        // сделать запрос на его удаление
        if (isScheduleExistsById(sched_id))
        {
            Schedule removeSched;
            removeSched.id = sched_id;

            emit removeScheduleRequred(removeSched);
            return;
        }

        // если расписания нет, то предупреждение
        showMessage("Такой записи не существует");
    }


    // обработчик кнопки редактировать расписание
    void on_editScheduleButton_clicked()
    {
        // получение id изменяемого расписания
        int sched_id;// = getIdFromUser("Введите id изменяемой записи");

        if (sched_id <= 0) // проверка на отмену
        {
            return;
        }
        else if (isScheduleExistsById(sched_id) == false)
        {
            // если расписания не существует то предупреждение
            showMessage("Такой записи не существует");
            return;
        }



        // получение id ребенка

        int child_id;// = getIdFromUser("Введите id ребенка");

        if (child_id <= 0) // проверка на отмену
        {
            return;
        }
        else if (isChildExists(child_id) == false)
        {
            // если ребенка не существует то предупреждение
            showMessage("Такого ребенка не существует");
            return;
        }



        // получение id тренера

        int trainer_id;// = getIdFromUser("Введите id тренера");

        if (trainer_id <= 0) // проверка на отмену
        {
            return;
        }
        else if (isTrainerExists(trainer_id) == false)
        {
            // если тренера не существует то предупреждение
            showMessage("Такого тренера не существует");
            return;
        }

        // редактирование расписания тренировки
        Schedule editedSched(child_id, trainer_id);
        editedSched.id = sched_id;
        emit editScheduleRequred(editedSched);  // запрос на редактирования в базе
    }

private:
    // метод для вывода сообщения пользователю
    void showMessage(QString message)
    {
        QMessageBox::warning(this, "Сообщение",
                             message, QMessageBox::Ok);
    }

    // Проверка что расписание существует
    bool isScheduleExistsById(int sched_id)
    {
        // обычная прогонка по списку расписаний и сверка что они существуют
        for (Schedule sched : this->schedule)
        {
            if (sched.id == sched_id)
            {
                return true;
            }
        }

        return false;
    }


    // Проверка что ребенок существует
    bool isChildExists(int id)
    {
        // поиск в списке детей
        for (Person pers : this->children)
        {
            if (pers.id == id)
            {
                return true;
            }
        }

        return false;
    }


    // Проверка что тренер существует
    bool isTrainerExists(int id)
    {
        // поиск в списке тренеров
        for (Person pers : this->trainers)
        {
            if (pers.id == id)
            {
                return true;
            }
        }

        return false;
    }


    // взять человека из списка
    Person *getPersonFromList(int id, QList<Person> people)
    {
        // человек по id ищется в списке
        // если он есть он возвращается
        for (Person pers : people)
        {
            if (pers.id == id)
            {
                return new Person(pers);
            }
        }

        // если его там нет возвращается пустой указатель
        return nullptr;
    }


public:
    // обработчик события изменения размера окна
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        // виджеты, которым нужно реагировать на изменение размера окна
        // устанавливают новый размер окна

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

        if (personEditor != nullptr)
        {
            personEditor->resize(resizeEvent->size());
        }
    }

    // Деструктор
    // код выполняющийся при удалении этого объекта
    // вручную удаляется ссылка на графический интерфейс
    // т.к. память на него выделялась "вручную"
    ~MainWindow()
    {
        delete ui;
    }

};


