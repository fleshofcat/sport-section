#pragma once

#include <QResizeEvent> // для корректной обработки изменения размера окна приложения
#include <QInputDialog> // для диалоговых окон с пользователями
#include <QMessageBox>  // для вывода предупреждений пользователю

#include "common/common_objects.h" // файл с общими объектами (Person, Schedule)

#include "ui/widgets/string_table_viewer.h"
#include "ui/person_editor.h"


// класс MainWindow/ГлавноеОкно является классом-прослойкой
// между пользователем и программой
//
// он показывает пользователю информацию
// принимает и обрабатывает его запросы
class MainWindow : public QWidget
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
    QTabWidget *tabs;

    StringTableViewer *childrenTable;
    StringTableViewer *trainersTable;
    StringTableViewer *scheduleTable;

    PersonEditor *personEditor = nullptr;

    QList<Person> children;     // объект для хранения детей
    QList<Person> trainers;     // объект для хранения тренеров
    QList<Schedule> schedule;   // объект для хранения расписаний


public:
    // код который будет выполняться при создании объекта от этого класса
    // имеет 1 не обязательный системный параметр
    explicit MainWindow(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();

        connect(childrenTable, &StringTableViewer::createRecordIsRequred, this, &MainWindow::on_createChild);
        connect(childrenTable, &StringTableViewer::editRecordIsRequred, this, &MainWindow::on_editChild);

        connect(trainersTable, &StringTableViewer::createRecordIsRequred, this, &MainWindow::on_createTrainer);
        connect(trainersTable, &StringTableViewer::editRecordIsRequred, this, &MainWindow::on_editTrainer);

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
    void setUpUi()
    {
        this->resize(764, 378);

        childrenTable = new StringTableViewer;
        trainersTable = new StringTableViewer;
        scheduleTable = new StringTableViewer;

        tabs = new QTabWidget(this);


        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabs->sizePolicy().hasHeightForWidth());
        tabs->setSizePolicy(sizePolicy);


        tabs->addTab(childrenTable, "Спортсмены");
        tabs->addTab(trainersTable, "Тренера");
        tabs->addTab(scheduleTable, "Расписания");

    }

    // метод для обновления списка детей пользователю
    void updateChildren(QList<Person> children)
    {
        QList<QList<QString>> stringTable;

        for (Person pers : children)
        {
            stringTable << pers.getInList();
        }

        childrenTable->updateData(Person::getPattern(), stringTable);
    }


    // метод для обновления списка тренеров пользователю
    void updateTrainers(QList<Person> trainers)
    {
        QList<QList<QString>> stringTable;

        for (Person pers : trainers)
        {
            stringTable << pers.getInList();
        }

        trainersTable->updateData(Person::getPattern(), stringTable);

    }


    // метод для обновления списка расписаний пользователю
    void updateSchedule(QList<Schedule> schedule,
                        QList<Person> children, QList<Person> trainers)
    {
        /*
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
        */
    }



    void openPersonEditorBy(PersonEditor::Who who)
    {
        Person pers;
        pers.isTrainer = (who == PersonEditor::Who::TRAINER) ? true : false;

        openPersonEditorBy(pers);
    }

    void openPersonEditorBy(Person person)
    {
        personEditor = new PersonEditor(person, this);

        tabs->hide();
        personEditor->resize(this->size());

        connect(personEditor, &PersonEditor::savePerson, this, &MainWindow::savePersonIsRequred);
        connect(personEditor, &PersonEditor::removePerson, this, &MainWindow::removePersonIsRequred);
        connect(personEditor, &PersonEditor::exitPersonEditor, this, &MainWindow::on_exitPersonEditor);
    }



private slots:
    void on_createChild()
    {
        openPersonEditorBy(PersonEditor::Who::CHILD);
    }

    void on_editChild(int row)
    {
        openPersonEditorBy(children.at(row));
    }

    void on_createTrainer()
    {
        openPersonEditorBy(PersonEditor::Who::TRAINER);
    }

    void on_editTrainer(int row)
    {
        openPersonEditorBy(trainers.at(row));
    }

    void on_exitPersonEditor()
    {
        delete personEditor;
        personEditor = nullptr;

        tabs->show();
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

        tabs->resize(resizeEvent->size());

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
//        delete ui;
    }

};


