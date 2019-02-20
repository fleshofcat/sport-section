#pragma once    // что бы создавалась только одина копия этого файла для всех целей

#include <QList> // библиотека для работы со листами данных

// класс Person/Человек
// отвечает за хранение данных любого человека
class Person
{
public:
    Person(QList<QString> personData)
    {
        setInList(personData);
    }

    Person() {}


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
            firstName   = personData.takeFirst();
            secondName  = personData.takeFirst();
            lastName    = personData.takeFirst();
            birthday    = personData.takeFirst();
            sportType   = personData.takeFirst();
        }
    }


    QList<QString> getInList()
    {
        return {firstName, secondName, lastName, birthday, sportType};
    }

    QList<QString> static getPattern()
    {
        return {"Имя", "Отчество", "Фамилия", "День рождения", "Спорт"};
    }

    // данные самого человека

    int id = 0;             // id в конечном итоге должна устанавливать сама бд
    bool isTrainer = false; // является ли человек тренером (по умолчанию нет)

    QString firstName;      // имя
    QString secondName;     // Отчество
    QString lastName;       // фамилия
    QString birthday;       // дата рождения
    QString sportType;      // вид спорта
};


// класс Schedule/расписание
// хранение данных о графике тренировок тренеров и детей
class Group
{
public:
    // констуктор мгновенного создания полного объекта
    Group(QList<QString> property)
    {
        setProperty(property);
    }

    // конструктор создания пустого объекта
    Group() {}

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

    QList<int> trainers_ids;
    QList<int> sportsmen_ids;

};


