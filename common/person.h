#pragma once

#include <QList>

// класс Person/Человек
// отвечает за хранение данных любого человека
class Person
{
public:
    int id = 0;             // id в конечном итоге должна устанавливать сама бд

    QString firstName;      // имя
    QString secondName;     // Отчество
    QString lastName;       // фамилия
    QString birthday;       // дата рождения
    QString sportType;      // вид спорта

    Person(QList<QString> personData)
    {
        setInList(personData);
    }
    Person() {}

    void setInList(QList<QString> personData)
    {
        if (personData.count() == pattern().count())
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

    static QList<QString> pattern()
    {
        return {"Имя", "Отчество", "Фамилия", "День рождения", "Спорт"};
    }

    static QList<QList<QString>>
    toStringTable(QList<Person> people)
    {
        QList<QList<QString>> stringTable;
        for (Person pers : people)
        {
            stringTable << pers.getInList();
        }

        return stringTable;
    }

    static QList<int> getIds(QList<Person> people)
    {
        QList<int> ids;
        for (Person pers : people)
        {
            ids << pers.id;
        }

        return ids;
    }

    friend bool operator== (const Person &p1, const Person &p2);
    friend bool operator!= (const Person &p1, const Person &p2);
};

Q_DECLARE_METATYPE(Person)




















