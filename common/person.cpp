#include "common/person.h"


void Person::setFirstName(QString newFirstName)
{
    this->firstName = newFirstName;
}
QString Person::getFirstName()
{
    return firstName;
}


void Person::setSecondName(QString newSecondName)
{
    secondName = newSecondName;
}
QString Person::getSecondName()
{
    return secondName;
}


void Person::setLastName(QString newLastName)
{
    lastName = newLastName;
}
QString Person::getLastName()
{
    return lastName;
}


void Person::setSportType(QString newSportType)
{
    sportType = newSportType;
}
QString Person::getSportType()
{
    return sportType;
}


void Person::setPhoneNumber(QString newPhoneNumber)
{
    phoneNumber = newPhoneNumber;
}
QString Person::getPhoneNumber()
{
    return phoneNumber;
}


void Person::setBirthday(QDate newBirthday)
{
    birthday = newBirthday;
}
void Person::setBirthday(QString newBirthday)
{
    setBirthday(QDate::fromString(newBirthday, "dd.MM.yyyy"));
}
QDate Person::getBirthdayInDate()
{
    return birthday;
}
QString Person::getBirthdayInString()
{
    return getBirthdayInDate().toString("dd.MM.yyyy");
}


void Person::setRating(int newRating)
{
    rating = newRating;
}
int Person::getRating()
{
    return rating;
}
void Person::increaseRating(int increaseValue)
{
    setRating(getRating() + increaseValue);
}


void Person::setEventsNumber(int newNumber)
{
    eventsNumber = newNumber;
}
int Person::getEventsNumber()
{
    return eventsNumber;
}
void Person::increaseEventNumber(int increaseValue)
{
    setEventsNumber(getEventsNumber() + increaseValue);
}


QList<QString> Person::getPreviewProperty()
{
    return {getFirstName(), getSecondName(), getLastName(), getSportType()};
}
QList<QString> Person::getPreviewPattern()
{
    return {"Имя", "Отчество", "Фамилия", "Спорт"};
}

void Person::setEditableProperty(QList<QString> personData)
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
QList<QString> Person::getEditableProperty()
{
    return {getFirstName(), getSecondName(),
                getLastName(), getBirthdayInString(),
                getSportType(), getPhoneNumber()};
}
QList<QString> Person::getEditablePattern()
{
    return {"Имя", "Отчество", "Фамилия", "День рождения", "Спорт", "Номер телефона"};
}

void Person::setFullProperty(QList<QString> personData)
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
QList<QString> Person::getFullProperty()
{
    return {getFirstName(), getSecondName(),
                getLastName(), getBirthdayInString(),
                getSportType(), getPhoneNumber(),
                QString::number(getRating()),
                QString::number(getEventsNumber())};
}
QList<QString> Person::getFullPattern()
{
    return {"Имя", "Отчество", "Фамилия", "День рождения", "Спорт", "Номер телефона",
        "Рейтинг", "Мероприятий"};
}

QList<QList<QString>>
Person::toPreviewTable(QList<Person> people)
{
    QList<QList<QString>> stringTable;
    for (Person pers : people)
    {
        stringTable << pers.getPreviewProperty();
    }

    return stringTable;
}

QList<int> Person::getIds(QList<Person> people)
{
    QList<int> ids;
    for (Person pers : people)
    {
        ids << pers.id;
    }

    return ids;
}

int Person::getAccumRating(QList<Person> people)
{
    int summRating = 0;
    for (Person pers : people)
    {
        summRating += pers.getRating();
    }

    return summRating;
}

QList<Person>
Person::getFreePeople(QList<Person> allPeople,
                      QList<Person> existingPeople)
{
    QList<Person> freePeople;
    for (Person pers : allPeople)
    {
        if (!existingPeople.contains(pers))
            freePeople << pers;
    }

    return freePeople;
}

QList<Person>
Person::getPeopleBySportType(QList<Person> allPeople, QString sport)
{
    QList<Person> ret;
    for (Person pers : allPeople)
    {
        if (pers.getSportType() == sport)
            ret << pers;
    }

    return ret;
}

QList<QList<QVariant>>
Person::getStatsTable(QList<Person> people)
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

QList<QString> Person::getStatsPattern()
{
    auto ret = Person::getPreviewPattern();
    ret << "Рейтинг";
    ret << "Мероприятий";
    return ret;
}



bool operator== (const Person &p1, const Person &p2)
{
    return (p1.id == p2.id)
            && Person(p1).getFullProperty() == Person(p2).getFullProperty();
}

bool operator!= (const Person &p1, const Person &p2)
{
    return !(p1 == p2);
}


