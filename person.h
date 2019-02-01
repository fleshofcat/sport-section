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
    bool isTrainer = false;
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

class Record
{
public:
    Record(int child_id, int trainer_id)
    {
        this->child_id = child_id;
        this->trainer_id = trainer_id;
    }
    Record() {}

    bool isFull()
    {
        return (trainer_id > 0)
                && (child_id > 0);
    }

    int id = 0;
    int trainer_id = 0;
    int child_id = 0;

};


