#pragma once    // что бы создавалась только одина копия этого файла для всех целей

#include <QList> // библиотека для работы со листами данных

// класс Person/Человек
// отвечает за хранение данных любого человека
class Person
{
public:
    // конструктор для мгновенного создания полного объекта
    Person(QString firstName, QString lastName,
           QString birthday, QString sportType, bool isTrainer)
    {
        this->firstName = firstName;
        this->lastName = lastName;
        this->birthday = birthday;
        this->sportType = sportType;
        this->isTrainer = isTrainer;
    }

    Person(QList<QString> personData)
    {
        setInList(personData);
    }

    // конструктор для создания пустого объекта
    Person() {}


    // проверка что объект
    // полон для добавления в бд
    bool isFull()
    {
        return !firstName.isEmpty()
                && !lastName.isEmpty()
                && !sportType.isEmpty()
                && !birthday.isEmpty();
    }

    void setInList(QList<QString> personData)
    {
        if (personData.count() == getPattern().count())
        {
            firstName = personData.at(0);
            lastName = personData.at(1);
            birthday = personData.at(2);
            sportType = personData.at(3);
        }
    }


    QList<QString> getInList()
    {
        return {firstName, lastName, birthday, sportType};
    }

    QList<QString> static getPattern()
    {
        return {"Имя", "Фамилия","День рождения", "Спорт"};
    }

    // данные самого человека

    int id = 0;             // id в конечном итоге должна устанавливать сама бд
    QString firstName;      // имя
    QString lastName;       // фамилия
    QString birthday;       // дата рождения
    QString sportType;      // вид спорта
    bool isTrainer = false; // является ли человек тренером (по умолчанию нет)
};


// класс Schedule/расписание
// хранение данных о графике тренировок тренеров и детей
class Group
{
public:
    // констуктор мгновенного создания полного объекта
    Group(int child_id, int trainer_id)
    {
        this->child_id = child_id;
        this->trainer_id = trainer_id;
    }

    // конструктор создания пустого объекта
    Group() {}

    // проверка что объект
    // достаточно заполнен для добавления в бд
    bool isFull()
    {
        return (trainer_id > 0)
                && (child_id > 0);
    }

    void setProperty(QList<QString> property)
    {
        groupName = property.at(0);
        sportType = property.at(1);
    }

    QList<QString> getProperty()
    {
        return {groupName, sportType};
    }

    QList<QString> getPattern()
    {
        return {"Группа", "Спорт"};
    }

    int id = 0;         // присваивается в бд
    QString groupName;
    QString sportType;

    int trainer_id = 0;
    int child_id = 0;

    QList<int> trainers_id;
    QList<int> children_id;

};


