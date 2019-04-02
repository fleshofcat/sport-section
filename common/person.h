#pragma once

#include <QList>
#include <QDate>

// класс Person/Человек
// отвечает за хранение данных любого человека
class Person
{
    int rating = 0;
    int eventsNumber = 0;

    QString firstName;      // имя
    QString secondName;     // Отчество
    QString lastName;       // фамилия
    QDate   birthday;       // дата рождения
    QString sportType;      // вид спорта
    QString phoneNumber;    // номер телефона

public:
    int id = 0;             // id в конечном итоге должна устанавливать сама бд

    Person(int id)
    {
        this->id = id;
    }
    Person() {}

    void setFirstName(QString newFirstName)
    {
        this->firstName = newFirstName;
    }
    QString getFirstName()
    {
        return firstName;
    }

    void setSecondName(QString newSecondName)
    {
        secondName = newSecondName;
    }
    QString getSecondName()
    {
        return secondName;
    }

    void setLastName(QString newLastName)
    {
        lastName = newLastName;
    }
    QString getLastName()
    {
        return lastName;
    }

    void setSportType(QString newSportType)
    {
        sportType = newSportType;
    }
    QString getSportType()
    {
        return sportType;
    }

    void setPhoneNumber(QString newPhoneNumber)
    {
        phoneNumber = newPhoneNumber;
    }
    QString getPhoneNumber()
    {
        return phoneNumber;
    }

    void setBirthday(QDate newBirthday)
    {
        birthday = newBirthday;
    }
    void setBirthday(QString newBirthday)
    {
        setBirthday(QDate::fromString(newBirthday, "dd.MM.yyyy"));
    }
    QDate getBirthdayInDate()
    {
        return birthday;
    }
    QString getBirthdayInString()
    {
        return getBirthdayInDate().toString("dd.MM.yyyy");
    }

    void setRating(int newRating)
    {
        rating = newRating;
    }
    int getRating()
    {
        return rating;
    }
    void increaseRating(int increaseValue)
    {
        setRating(getRating() + increaseValue);
    }

    void setEventsNumber(int newNumber)
    {
        eventsNumber = newNumber;
    }
    int getEventsNumber()
    {
        return eventsNumber;
    }
    void increaseEventNumber(int increaseValue)
    {
        setEventsNumber(getEventsNumber() + increaseValue);
    }



    QList<QString> getPreviewProperty()
    {
        return {getFirstName(), getSecondName(), getLastName(), getSportType()};
    }
    static QList<QString> getPreviewPattern()
    {
        return {"Имя", "Отчество", "Фамилия", "Спорт"};
    }

    void setEditableProperty(QList<QString> personData)
    {
        if (personData.count() == getEditablePattern().count())
        {
            setFirstName(personData.takeFirst());
            setSecondName(personData.takeFirst());
            setLastName(personData.takeFirst());
            setBirthday(personData.takeFirst());
            setSportType(personData.takeFirst());
            setPhoneNumber(personData.takeFirst());
        }
    }
    QList<QString> getEditableProperty()
    {
        return {getFirstName(), getSecondName(),
                    getLastName(), getBirthdayInString(),
                    getSportType(), getPhoneNumber()};
    }
    static QList<QString> getEditablePattern()
    {
        return {"Имя", "Отчество", "Фамилия", "День рождения", "Спорт", "Номер телефона"};
    }

    void setFullProperty(QList<QString> personData)
    {
        if (personData.count() == getFullPattern().count())
        {
            setFirstName(personData.takeFirst());
            setSecondName(personData.takeFirst());
            setLastName(personData.takeFirst());
            setBirthday(personData.takeFirst());
            setSportType(personData.takeFirst());
            setPhoneNumber(personData.takeFirst());

            setRating(personData.takeFirst().toInt());
            setEventsNumber(personData.takeFirst().toInt());
        }
    }
    QList<QString> getFullProperty()
    {
        return {getFirstName(), getSecondName(),
                    getLastName(), getBirthdayInString(),
                    getSportType(), getPhoneNumber(),
                    QString::number(getRating()),
                    QString::number(getEventsNumber())};
    }
    static QList<QString> getFullPattern()
    {
        return {"Имя", "Отчество", "Фамилия", "День рождения", "Спорт", "Номер телефона",
            "Рейтинг", "Мероприятий"};
    }

    static QList<QList<QString>>
    toPreviewTable(QList<Person> people)
    {
        QList<QList<QString>> stringTable;
        for (Person pers : people)
        {
            stringTable << pers.getPreviewProperty();
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

    static int getAccumRating(QList<Person> people)
    {
        int summRating = 0;
        for (Person pers : people)
        {
            summRating += pers.getRating();
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

    static QList<Person> getPeopleBySportType(QList<Person> allPeople, QString sport)
    {
        QList<Person> ret;
        for (Person pers : allPeople)
        {
            if (pers.getSportType() == sport)
                ret << pers;
        }

        return ret;
    }

    static QList<QList<QVariant>>
    getStatsTable(QList<Person> people)
    {
        QList<QList<QVariant>> ret;

        for (auto pers : people)
        {
            QList<QVariant> stats;
            for (QString field : pers.getPreviewProperty())
            {
                stats << QVariant(field);
            }
            stats << QVariant(pers.getRating());
            stats << QVariant(pers.getEventsNumber());

            ret << stats;
        }
        return ret;
    }

    static QList<QString>
    getStatsPattern()
    {
        auto ret = Person::getPreviewPattern();
        ret << "Рейтинг";
        ret << "Мероприятий";
        return ret;
    }

    friend bool operator== (const Person &p1, const Person &p2);
    friend bool operator!= (const Person &p1, const Person &p2);
};

Q_DECLARE_METATYPE(Person)




















