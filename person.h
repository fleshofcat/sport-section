#pragma once

#include <QString>

class Person
{
public:
    Person() {}

    Person(QString firstName, QString lastName,
           QString birthday, QString sportType, bool isTrainer)
    {
        this->firstName = firstName;
        this->lastName = lastName;
        this->birthday = birthday;
        this->sportType = sportType;
        this->isTrainer = isTrainer;
    }

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



