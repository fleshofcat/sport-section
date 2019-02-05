#pragma once    // что бы создавалась только одина копия этого файла для всех целей

#include <QString> // библиотека для работы со строками

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
class Schedule
{
public:
    // констуктор мгновенного создания полного объекта
    Schedule(int child_id, int trainer_id)
    {
        this->child_id = child_id;
        this->trainer_id = trainer_id;
    }

    // конструктор создания пустого объекта
    Schedule() {}

    // проверка что объект
    // достаточно заполнен для добавления в бд
    bool isFull()
    {
        return (trainer_id > 0)
                && (child_id > 0);
    }

    int id = 0;         // присваивается в бд
    int trainer_id = 0; // id тренера
    int child_id = 0;   // id ребенка

};


