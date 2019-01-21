#pragma once

#include <QtTest/QtTest>
#include <QtSql>

#include "relation_storage.h"

class TestRelationStorage : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("../record/tests/test_res/sport_people.db");
        QVERIFY(db.open());
    }

    void test_addRelation()
    {
        // prepare
        RelationStorage relations;
        Relation rel(1, 2); // relation don't care about validation

        // run test method
        QVERIFY(relations.addRelation(rel));

        // get examine data
        QSqlQuery query;
        query.exec("SELECT * FROM relations WHERE "
                   "id = (SELECT MAX(id) FROM relations)");
        query.next();

        // compare
        QCOMPARE(rel.trainer_id, query.value("trainer_id").toInt());
        QCOMPARE(rel.child_id, query.value("child_id").toInt());
    }

    void test_removeRelation()
    {
        // preparing

        // create relation for delete
        RelationStorage links;
        Relation rel(1, 2);

        links.addRelation(rel);

        // get id
        QSqlQuery query("SELECT max(id) FROM relations");
        query.next();

        int last_id = query.value(0).toInt();
        qDebug() << "max id = " << last_id;

        rel.id = last_id;

        // examine

        // removeRelation() is deleting by rel.id
        QVERIFY(links.removeRelation(rel));

        query.exec("SELECT max(id) FROM relations");
        query.next();

        // compare rel.id with last id
        QVERIFY(rel.id != query.value(0).toInt());
    }

    void test_replaceRelationById()
    {
        // prepare

        // add basic relation
        RelationStorage links;
        links.addRelation(Relation(1, 2));

        // make relation for update
        Relation rel(3, 4);

        // get new relation id
        QSqlQuery query("SELECT max(id) FROM relations");
        query.next();
        rel.id = query.value(0).toInt();

        // run test method
        links.replaceRelationById(rel);

        // get examine data
        query.prepare("SELECT * FROM relations WHERE id = (:id)");
        query.bindValue(":id", rel.id);
        query.exec();
        query.next();

        // compare
        QCOMPARE(rel.trainer_id, query.value("trainer_id").toInt());
        QCOMPARE(rel.child_id, query.value("child_id").toInt());
    }

    void test_getAllRelations()
    {
        // prepare

        // clean and refill the relations table
        QSqlQuery query("DELETE FROM relations");

        RelationStorage links;

        Relation insertRel_1(1, 2);
        links.addRelation(insertRel_1);

        Relation insertRel_2(3, 4);
        links.addRelation(insertRel_2);

        // run test method
        QList<Relation> *relationList = links.getAllRelations();

        QVERIFY(compareRelations(insertRel_1, relationList->at(0)));
        QVERIFY(compareRelations(insertRel_2, relationList->at(1)));
    }

    void cleanupTestCase()
    {
        {
            QSqlQuery query("DELETE FROM relations");
            QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
            db.close();
        }
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

private:
    bool compareRelations(Relation rel_1, Relation rel_2)
    {
        return     (rel_1.trainer_id == rel_2.trainer_id)
                && (rel_1.child_id == rel_2.child_id);
    }};



