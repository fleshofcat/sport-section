#pragma once

#include <QResizeEvent>
#include <QStackedWidget>

#include "common/common_objects.h"
#include "ui/person_editor.h"
#include "ui/widgets/records_widget.h"

class PeopleTab : public QWidget
{
    Q_OBJECT

    QList<Person> people;// = nullptr;

    QStackedWidget *widgetStack;
    RecordsWidget *recordsViewer;
    PersonEditor *personEditor;

signals:
    void savePerson(Person person);
    void removePerson(int id, Person::Who who);

public:
    Person::Who who = Person::Who::EMPTY;


    PeopleTab(Person::Who who, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        this->who = who;

        setUpUi();
    }


    void updateContent(QList<Person> people)
    {
        this->people = people;

        recordsViewer->updateData(
                    Person::getPattern(),
                    Person::personListToStringTable(people));
    }

private:
    void setUpUi()
    {
        recordsViewer = new RecordsWidget;
        personEditor = new PersonEditor;

        widgetStack = new QStackedWidget(this);

        widgetStack->addWidget(recordsViewer);
        widgetStack->addWidget(personEditor);


        connect(recordsViewer, &RecordsWidget::createRecordIsRequred,
                this, &PeopleTab::on_createRecord);

        connect(recordsViewer, &RecordsWidget::editRecordIsRequred,
                this, &PeopleTab::on_editRecord);



        connect(personEditor, &PersonEditor::saveIsRequred,
                this, &PeopleTab::on_savePerson);

        connect(personEditor, &PersonEditor::removeIsRequred,
                this, &PeopleTab::on_removePerson);

        connect(personEditor, &PersonEditor::exitIsRequred,
                this, &PeopleTab::on_editorExit);
    }

private slots:
    void on_createRecord()
    {
        personEditor->updateContent(Person());
        widgetStack->setCurrentIndex(1);
    }

    void on_editRecord(int row)
    {
        if (row >= 0)
        {
            personEditor->updateContent(people.at(row));
            widgetStack->setCurrentIndex(1);
        }
    }

    void on_savePerson(Person person)
    {
        person.who = who;
        emit savePerson(person);
        on_editorExit();
    }

    void on_removePerson(int id)
    {
        emit removePerson(id, who);
        on_editorExit();
    }

    void on_editorExit()
    {
        personEditor->dropContent();
        widgetStack->setCurrentIndex(0);
    }

public:
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        widgetStack->resize(resizeEvent->size());
    }
};




























