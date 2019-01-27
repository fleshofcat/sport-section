#pragma once

#include <QtSql>

#include "people_storage.h"
#include "relation_storage.h"

class DbManager : public QObject
{
    Q_OBJECT

    PeopleStorage *people;
    RelationStorage *relations;
public:

    explicit DbManager(QString db_path, QObject *parent = nullptr) : QObject (parent)
    {        
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(db_path);

        if (db.open() == false)
        {
            qDebug() << "Error: Database connection is failed";
            return;
        }
        people = new PeopleStorage(this);
        relations = new RelationStorage(this);
    }

    // people
    bool addPerson(Person pers)
    {
        return people->addPerson(pers);
    }

    bool removePerson(Person pers)
    {
        return people->removePerson(pers);
    }

    bool replacePersonById(Person pers)
    {
        return people->replacePersonById(pers);
    }

    QList<Person> *getChildren()
    {
        QList<Person> peopleList = *people->getAllPeople();
        QList<Person> *children = new QList<Person>;

        for (Person pers : peopleList)
        {
            if (pers.isTrainer == false)
            {
                *children << pers;
            }
        }
        return children;
    }

    QList<Person> *getTrainers()
    {
        QList<Person> peopleList = *people->getAllPeople();
        QList<Person> *trainers = new QList<Person>;

        for (Person pers : peopleList)
        {
            if (pers.isTrainer == true)
            {
                *trainers << pers;
            }
        }
        return trainers;
    }


    // lessons
    bool addLession(Lesson les)
    {
        Relation rel(les.trainer.id, les.child.id);
        return relations->addRelation(rel);
    }

    bool removeLession(Lesson les)
    {
        Relation rel;
        rel.id = les.id;
        return relations->removeRelation(rel);
    }

    bool replaceLessonById(Lesson les)
    {
        Relation rel;
        rel.id = les.id;
        rel.trainer_id = les.trainer.id;
        rel.child_id = les.child.id;
        return relations->replaceRelationById(rel);
    }

    QList<Lesson> *getLessons()
    {
        QList<Relation> *rels = relations->getAllRelations();
        QList<Lesson> *lessons = new QList<Lesson>;

        for (Relation rel : *rels)
        {
            Lesson les;

            les.id = rel.id;
            les.trainer = people->getPerson(rel.trainer_id);
            les.child = people->getPerson(rel.child_id);

            *lessons << les;
        }

        return lessons;
    }

    ~DbManager()
    {
        {
            QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
            db.close();
        }
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }
};




















