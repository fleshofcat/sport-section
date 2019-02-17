#pragma once

#include <QResizeEvent> // для корректной обработки изменения размера окна приложения
#include <QInputDialog> // для диалоговых окон с пользователями
#include <QMessageBox>  // для вывода предупреждений пользователю

#include "ui/person_editor.h"

#include "ui/people_tab.h"


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
//    void addScheduleRequred(Schedule newSchedule);      // удаление людей и расписаний

private:
    QTabWidget *tabs;

    PeopleTab *childrenTable = nullptr;
    PeopleTab *trainersTable = nullptr;

//    RecordsViewer *scheduleTable;

    QList<Person> children;     // объект для хранения детей
    QList<Person> trainers;     // объект для хранения тренеров
    QList<Group> schedule;   // объект для хранения расписаний


public:
    // код который будет выполняться при создании объекта от этого класса
    // имеет 1 не обязательный системный параметр
    explicit MainWindow(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();

        connect(childrenTable, &PeopleTab::savePerson, this, &MainWindow::savePersonIsRequred);
        connect(childrenTable, &PeopleTab::removePerson, this, &MainWindow::removePersonIsRequred);

        connect(trainersTable, &PeopleTab::savePerson, this, &MainWindow::savePersonIsRequred);
        connect(trainersTable, &PeopleTab::removePerson, this, &MainWindow::removePersonIsRequred);

    }


    // метод с помощью которого данные от бд
    // будут загружаться в данный класс и отображаться пользователю
    void updateUi(QList<Person> children, QList<Person> trainers, QList<Group> schedule)
    {
        // данные из последнего апдейта будут храниться
        // в локальных переменных этого класса
        this->children = children;
        this->trainers = trainers;
        this->schedule = schedule;

        // обновление внешнего вида программы
        childrenTable->updateContent(children);
        trainersTable->updateContent(trainers);
    }


private:
    void setUpUi()
    {
        this->resize(764, 378);

        childrenTable = new PeopleTab(PeopleTab::Who::CHILDREN);
        trainersTable = new PeopleTab(PeopleTab::Who::TRAINERS);

        tabs = new QTabWidget(this);


        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabs->sizePolicy().hasHeightForWidth());
        tabs->setSizePolicy(sizePolicy);


        tabs->addTab(childrenTable, "Спортсмены");
        tabs->addTab(trainersTable, "Тренера");
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
        for (Group sched : this->schedule)
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


    // Проверка что человек существует
    bool isPersonExist(int id, QList<Person> people)
    {
        // поиск в списке тренеров
        for (Person pers : people)
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
    }
};


