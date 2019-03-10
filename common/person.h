#pragma once

#include <QList>

// класс Person/Человек
// отвечает за хранение данных любого человека
class Person
{
public:
    int id = 0;             // id в конечном итоге должна устанавливать сама бд

    int rating = 0;
    int eventsNumber = 0;

    QString firstName;      // имя
    QString secondName;     // Отчество
    QString lastName;       // фамилия
    QString birthday;       // дата рождения
    QString sportType;      // вид спорта
    QString phoneNumber;    // TODO now is not influence

    Person(QList<QString> fullPersonData)
    {
        setFullList(fullPersonData);
    }
    Person(int id)
    {
        this->id = id;
    }
    Person() {}

    QList<QString> getPreviewList()
    {
        return {firstName, secondName, lastName, sportType};
    }
    static QList<QString> getPreviewPattern()
    {
        return {"Имя", "Отчество", "Фамилия", "Спорт"};
    }

    void setEditableList(QList<QString> personData)
    {
        if (personData.count() == getEditablePattern().count())
        {
            firstName   = personData.takeFirst();
            secondName  = personData.takeFirst();
            lastName    = personData.takeFirst();
            birthday    = personData.takeFirst();
            sportType   = personData.takeFirst();
            phoneNumber = personData.takeFirst();
        }
    }
    QList<QString> getEditableList()
    {
        return {firstName, secondName, lastName, birthday, sportType, phoneNumber};
    }
    static QList<QString> getEditablePattern()
    {
        return {"Имя", "Отчество", "Фамилия", "День рождения", "Спорт", "Номер телефона"};
    }

    void setFullList(QList<QString> personData)
    {
        if (personData.count() == getFullPattern().count())
        {
            firstName   = personData.takeFirst();
            secondName  = personData.takeFirst();
            lastName    = personData.takeFirst();
            birthday    = personData.takeFirst();
            sportType   = personData.takeFirst();
            phoneNumber = personData.takeFirst();
            rating      = personData.takeFirst().toInt();
            eventsNumber = personData.takeFirst().toInt();
        }
    }
    QList<QString> getFullList()
    {
        return {firstName, secondName, lastName, birthday, sportType, phoneNumber,
                    QString::number(rating), QString::number(eventsNumber)};
    }
    static QList<QString> getFullPattern()
    {
        return {"Имя", "Отчество", "Фамилия", "День рождения", "Спорт", "Номер телефона",
            "Рейтинг", "Количество мероприятий"};
    }

    static QList<QList<QString>>
    toPreviewStringTable(QList<Person> people)
    {
        QList<QList<QString>> stringTable;
        for (Person pers : people)
        {
            stringTable << pers.getPreviewList();
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

    static int getPeopleRating(QList<Person> people)
    {
        int summRating = 0;
        for (Person pers : people)
        {
            summRating += pers.rating;
        }

        return summRating;
    }

    static QList<Person> getFreePeople(QList<Person> allPeople, QList<Person> existingPeople)
    {
        QList<Person> freePeople;
        for (Person pers : allPeople)
        {
            if (!existingPeople.contains(pers))
                freePeople << pers;
        }

        return freePeople;
    }


    friend bool operator== (const Person &p1, const Person &p2);
    friend bool operator!= (const Person &p1, const Person &p2);
};

Q_DECLARE_METATYPE(Person)




















