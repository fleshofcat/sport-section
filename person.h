#pragma once

#include <QString>

class Person
{
public:
    Person(QString firstName, QString lastName,
           QString birthday, QString sportType, bool isTrainer)
    {
        this->firstName = firstName;
        this->lastName = lastName;
        this->birthday = birthday;
        this->sportType = sportType;
        this->isTrainer = isTrainer;
    }

    Person() {}


    bool isFull()
    {
        return !firstName.isEmpty()
                && !lastName.isEmpty()
                && !sportType.isEmpty()
                && !birthday.isEmpty();
    }

    int id = 0;
    QString firstName;
    QString lastName;
    QString birthday;
    QString sportType;
    bool isTrainer;
};


class Lesson
{
public:
    Lesson(){}
    Lesson(Person trainer, Person child)
    {
        this->trainer = trainer;
        this->child = child;
    }

    int id = 0;
    Person trainer;
    Person child;
};



