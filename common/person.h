#pragma once

#include <QList>
#include <QDate>

class Person
{
    int rating = 0;
    int eventsNumber = 0;

    QString firstName;
    QString secondName;
    QString lastName;
    QDate   birthday;
    QString sportType;
    QString phoneNumber;    

public:
    int id = 0;             // id is setting in db

    explicit Person(int id) : id(id) { }
    explicit Person() { }

    void setFirstName(QString newFirstName);
    QString getFirstName();

    void setSecondName(QString newSecondName);
    QString getSecondName();

    void setLastName(QString newLastName);
    QString getLastName();

    void setSportType(QString newSportType);
    QString getSportType();

    void setPhoneNumber(QString newPhoneNumber);
    QString getPhoneNumber();

    void setBirthday(QDate newBirthday);
    void setBirthday(QString newBirthday);
    QDate getBirthdayInDate();
    QString getBirthdayInString();

    void setRating(int newRating);
    int getRating();
    void increaseRating(int increaseValue);

    void setEventsNumber(int newNumber);
    int getEventsNumber();
    void increaseEventNumber(int increaseValue);



    QList<QString> getPreviewProperty();
    static QList<QString> getPreviewPattern();

    void setEditableProperty(QList<QString> personData);
    QList<QString> getEditableProperty();
    static QList<QString> getEditablePattern();

    void setFullProperty(QList<QString> personData);
    QList<QString> getFullProperty();
    static QList<QString> getFullPattern();

    static QList<QList<QString>>
    toPreviewTable(QList<Person> people);

    static QList<int> getIds(QList<Person> people);

    static int getAccumRating(QList<Person> people);

    static QList<Person>
    getFreePeople(QList<Person> allPeople,
                  QList<Person> existingPeople);

    static QList<Person>
    getPeopleBySportType(QList<Person> allPeople, QString sport);

    static QList<QList<QVariant>>
    getStatsTable(QList<Person> people);

    static QList<QString> getStatsPattern();

    friend bool operator== (const Person &p1, const Person &p2);
    friend bool operator!= (const Person &p1, const Person &p2);
};

Q_DECLARE_METATYPE(Person)




















