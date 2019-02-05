#pragma once

#include <QResizeEvent> // для корректной обработки изменения размера окна приложения
#include <QInputDialog> // для диалоговых окон с пользователями
#include <QMessageBox>  // для вывода предупреждений пользователю

#include "ui_mainwindow.h"  // автогенерируемый файл для отображения графики
#include "common_objects.h" // файл с общими объектами (Person, Schedule)

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
    void addPersonIsRequred(Person newPerson);          // сигналы, испускаемые
    void removePersonIsRequred(Person removedPerson);   // этим объектом
    void editPersonIsRequred(Person editedPerson);      // они обрабатывают
                                                        // добавление, редактирование,
    void addScheduleRequred(Schedule newSchedule);      // удаление людей и расписаний
    void removeScheduleRequred(Schedule removedSchedule);
    void editScheduleRequred(Schedule editedSchedule);

private:
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
        updateSchedule(schedule, children, trainers);
    }


private:
    // метод для обновления списка детей пользователю
    void updateChildren(QList<Person> children)
    {
        ui->childrenTable->clearContents(); // сброс старых
        ui->childrenTable->setRowCount(0);  // данных перед обнавлением


        // индивидуально "распаковать" каждого человка и показать пользователю
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


private slots:
    // обработчик кнопки добавить ребенка
    void on_addChildButton_clicked()
    {
        // создается ребенок и заполняется данными от пользователя
        // с помощью метода getPersonDataFromUser/получитьДанныеЧеловекаОтПользователя
        Person newChild = getPersonDataFromUser();

        // помечается что новый человек это ребенок
        newChild.isTrainer = false;

        // если минимально необходимые данные заполнены корректно
        // посылается сигнал для добавления пользователя в бд
        if (newChild.isFull())
        {
            emit addPersonIsRequred(newChild);
        }
        else  // если каких-либо данных не хватает, выводится сообщение об ошибке
        {
            QMessageBox::warning(this, "Сообщение",
                "Данные введены не полностью", QMessageBox::Ok);
        }
    }

    // обработчик кнопки удалить ребенка
    void on_removeChildButton_clicked()
    {
        // от пользователя берется id удаляемого ребенка
        // с помощью метода getIdFromUser/получитьIdОтПользователя
        int child_id = getIdFromUser("Введите id удаляемого ребенка");

        // проверка отменил ли пользователь действие
        if (child_id <= 0)
            return;


        // проверка что человек существует
        // если это так, то происходит запрос на его удаление
        if (isChildExists(child_id))
        {
            Person childForRemove;
            childForRemove.id = child_id;

            emit removePersonIsRequred(childForRemove); // сигнал для бд удалить указанного человека
            return;
        }

        QMessageBox::warning(this, "Сообщение",
            "Такого ребенка не существует", QMessageBox::Ok);
    }

    // обработчик кнопки редактировать ребенка
    void on_editChildButton_clicked()
    {
        // от пользователя берется id удаляемого ребенка
        int child_id = getIdFromUser("Введите id изменяемого ребенка");

        // проверка отменил ли пользователь действие
        if (child_id <= 0)
            return;


        // проверка что пользователь существует
        // если это так то произвести запрос на редактирование
        if (isChildExists(child_id))
        {
            // создание и заполнение пользователя
            Person editChild = getPersonDataFromUser();
            editChild.id = child_id;
            editChild.isTrainer = false;


            // проверка что пользователь заполнен
            if (editChild.isFull())
            {
                emit editPersonIsRequred(editChild); // запрос на редактирование
                return;
            }
        }
        else        // если ребенка с заданным id не существует
        {           // показывается предупреждение

            QMessageBox::warning(this, "Сообщение",
                "Такого ребенка не существует", QMessageBox::Ok);
        }
    }


    // обработчик кнопки добавить тренера
    void on_addTrainerButton_clicked()
    {
        // создается и заполняется объект тренера
        Person newPerson = getPersonDataFromUser();

        // ставится отметка что это тренер
        newPerson.isTrainer = true;


        // если он заполнен корректно производится запрос на его добавление
        if (newPerson.isFull())
        {
            emit addPersonIsRequred(newPerson); // сам запрос
        }
        else        // если пользователь заполнен не полностью
        {           // выводится предупреждение и действие не происходит
            QMessageBox::warning(this, "Сообщение",
                "Данные введены не полностью", QMessageBox::Ok);
        }
    }


    // обработчик кнопки удалить тренера
    void on_removeTrainerButton_clicked()
    {
        // взятие id тренера
        int trainer_id = getIdFromUser("Введите id удаляемого тренера");

        // проверка на отмену действия от пользователя
        if (trainer_id <= 0)
            return;


        // если человек с данным id существует
        // запросить его удаление
        if (isTrainerExists(trainer_id))
        {
            Person removeTrainer;
            removeTrainer.id = trainer_id;

            emit removePersonIsRequred(removeTrainer); // запрос
            return;
        }

        // если такого человека не существует то вывести предупреждение
        QMessageBox::warning(this, "Сообщение",
            "Такого тренера не существует", QMessageBox::Ok);
    }


    // обработчик кнопки редактировать тренера
    void on_editTrainerButton_clicked()
    {
        // взять id тренера
        int trainer_id = getIdFromUser("Введите id изменяемого тренера");

        // проверка на отмену от пользователя
        if (trainer_id <= 0) return;


        // если человек с данным id существует
        // попробовать запросить обновление этого человека в бд
        if (isTrainerExists(trainer_id))
        {
            Person editTrainer = getPersonDataFromUser();
            editTrainer.id = trainer_id;
            editTrainer.isTrainer = true;

            // если человек заполнен
            // провести запрос
            if (editTrainer.isFull())
            {
                emit editPersonIsRequred(editTrainer); // запрос
                return;
            }
        }
        else        // если тренера с заданным id не существует
        {           // показывается предупреждение

            QMessageBox::warning(this, "Сообщение",
                "Такого тренера не существует", QMessageBox::Ok);
        }
    }


    // обработчик кнопки добавить расписание
    void on_addScheduleButton_clicked()
    {
        // Ввод ребенка для расписания
        int child_id = getIdFromUser("Введите id ребенка");

        if (child_id <= 0) return; // проверка на отмену

        if (isChildExists(child_id) == false) // проверка что ребенок существует
        {
            QMessageBox::warning(this, "Сообщение",
                                 "Такого ребенка не существует", QMessageBox::Ok);
            return;
        }


        // Ввод тренера для расписания
        int trainer_id = getIdFromUser("Введите id тренера");

        if (trainer_id <= 0) return; // проверка на отмену

        if (isTrainerExists(trainer_id) == false) // проверка что тренер существует
        {
            QMessageBox::warning(this, "Сообщение",
                                 "Такого тренера не существует", QMessageBox::Ok);
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
        int sched_id = getIdFromUser("Введите id удаляемой записи");

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
        QMessageBox::warning(this, "Сообщение",
            "Такой записи не существует", QMessageBox::Ok);
    }


    // обработчик кнопки редактировать расписание
    void on_editScheduleButton_clicked()
    {
        // получение id изменяемого расписания
        int sched_id = getIdFromUser("Введите id изменяемой записи");

        if (sched_id <= 0) // проверка на отмену
        {
            return;
        }
        else if (isScheduleExistsById(sched_id) == false)
        {
            // если расписания не существует то предупреждение
            QMessageBox::warning(this, "Сообщение",
                                 "Такой записи не существует", QMessageBox::Ok);
            return;
        }



        // получение id ребенка

        int child_id = getIdFromUser("Введите id ребенка");

        if (child_id <= 0) // проверка на отмену
        {
            return;
        }
        else if (isChildExists(child_id) == false)
        {
            // если ребенка не существует то предупреждение
            QMessageBox::warning(this, "Сообщение",
                                 "Такого ребенка не существует", QMessageBox::Ok);
            return;
        }



        // получение id тренера

        int trainer_id = getIdFromUser("Введите id тренера");

        if (trainer_id <= 0) // проверка на отмену
        {
            return;
        }
        else if (isTrainerExists(trainer_id) == false)
        {
            // если тренера не существует то предупреждение
            QMessageBox::warning(this, "Сообщение",
                                 "Такого тренера не существует", QMessageBox::Ok);
            return;
        }

        // редактирование расписания тренировки
        Schedule editedSched(child_id, trainer_id);
        editedSched.id = sched_id;
        emit editScheduleRequred(editedSched);  // запрос на редактирования в базе
    }


private:

    // метод взятия базовых данных от человека
    Person getPersonDataFromUser()
    {
        // создается объект человека
        Person retPerson;


        // заполняется имя
        retPerson.firstName = QInputDialog::getText(this, "",
            "Введите имя", QLineEdit::Normal);

        // если значиние не установилось, выйти на месте
        if (retPerson.firstName.isEmpty())
            return retPerson;



        // заполняется фамилия
        retPerson.lastName = QInputDialog::getText(this, "",
            "Введите фамилию", QLineEdit::Normal);

        // если значиние не установилось, выйти на месте
        if (retPerson.lastName.isEmpty())
            return retPerson;



        // заполняется дата рождения
        retPerson.birthday = QInputDialog::getText(this, "",
            "Введите дату рождения", QLineEdit::Normal);

        if (retPerson.birthday.isEmpty())
            return retPerson;



        // заполняется вид спорта
        retPerson.sportType = QInputDialog::getText(this, "",
            "Введите вид спорта", QLineEdit::Normal);

        return retPerson; // возвращение прлученного человека
    }


    // получение id от пользователя
    //
    // если не ввести при использовании вопроситеотную фразу для пользователя
    // программа спросит фразой по усолчанию - "Введите id"
    int getIdFromUser(QString message = "Введите id")
    {
        // переменная для результата ввода
        // по умолчанию false
        bool success = false;
        int id = -1;

        id = QInputDialog::getInt(this, "", message,
                    QLineEdit::Normal, 1, 0xfffffff, 1, &success);

        // если запрос произошел успешно, возвращается полученное число
        if (success)
        {
            return id;
        }
        // если запрос к пользователю произошел не удачно
        // возвращается -1
        return -1;
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


