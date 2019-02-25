#pragma once

#include <QResizeEvent> // для корректной обработки изменения размера окна приложения
#include <QInputDialog> // для диалоговых окон с пользователями
#include <QMessageBox>  // для вывода предупреждений пользователю

#include "ui/people_tab.h"
#include "ui/groups_tab.h"


// класс MainWindow/ГлавноеОкно является классом-прослойкой
// между пользователем и программой
//
// он показывает пользователю информацию
// принимает и обрабатывает его запросы
class MainWindow : public QWidget
{
    Q_OBJECT    // обязательный макрос

signals:
    void savePerson(Person savedPerson);       // сигналы, испускаемые
    void removePersonIs(int id, bool isTrainer); // этим объектом обрабатывают
    void saveGroup(Group group);
    void removeGroup(int id);

private:
    QTabWidget *tabs;

    PeopleTab *childrenTab = nullptr;
    PeopleTab *trainersTab = nullptr;
    GroupsTab *groupTab = nullptr;


    QList<Person> children;     // объект для хранения детей
    QList<Person> trainers;     // объект для хранения тренеров
    QList<Group> groups;

public:
    // код который будет выполняться при создании объекта от этого класса
    // имеет 1 не обязательный системный параметр
    explicit MainWindow(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();

        connect(childrenTab, &PeopleTab::savePerson, this, &MainWindow::savePerson);
        connect(childrenTab, &PeopleTab::removePerson, this, &MainWindow::removePersonIs);

        connect(trainersTab, &PeopleTab::savePerson, this, &MainWindow::savePerson);
        connect(trainersTab, &PeopleTab::removePerson, this, &MainWindow::removePersonIs);

        connect(groupTab, &GroupsTab::saveGroup, this, &MainWindow::saveGroup);
        connect(groupTab, &GroupsTab::removeGroup, this, &MainWindow::removeGroup);
    }


    // метод с помощью которого данные от бд
    // будут загружаться в данный класс и отображаться пользователю
    void updateContent(QList<Person> children,
                       QList<Person> trainers,
                       QList<Group> groups)
    {   
        this->children = children;
        this->trainers = trainers;
        this->groups = groups;

        childrenTab->updateContent(children);
        trainersTab->updateContent(trainers);
        groupTab->updateContent(children, trainers, groups);
    }


private:
    void setUpUi()
    {
        this->resize(800, 400);

        childrenTab = new PeopleTab(Person::Who::CHILDREN);
        trainersTab = new PeopleTab(Person::Who::TRAINERS);
        groupTab = new GroupsTab;

        tabs = new QTabWidget(this);


        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabs->sizePolicy().hasHeightForWidth());
        tabs->setSizePolicy(sizePolicy);


        tabs->addTab(childrenTab, "Спортсмены");
        tabs->addTab(trainersTab, "Тренера");
        tabs->addTab(groupTab, "Группы");
    }


private:
    // метод для вывода сообщения пользователю
    void showMessage(QString message)
    {
        QMessageBox::warning(this, "Сообщение",
                             message, QMessageBox::Ok);
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


