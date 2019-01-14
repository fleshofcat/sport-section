#pragma once

#include <QString>

class Person
{
public:
    Person() {}
    Person(/*int id, */QString firstName, QString lastName,
           QString birthday, QString description) :
//          id(id),
          firstName(firstName),
          lastName(lastName),
          birthday(birthday),
          description(description) {}

    bool isFull()
    {
        return !firstName.isEmpty()
                && !lastName.isEmpty()
                && !description.isEmpty()
                && !birthday.isEmpty();
//                && !(id == -1);
    }

    int id = -1;
    QString firstName;
    QString lastName;
    QString birthday;
    QString description;
};

