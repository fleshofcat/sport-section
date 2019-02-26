#pragma once

#include <QResizeEvent>
#include <QStackedWidget>

#include "common/common_objects.h"
#include "ui/widgets/string_list_editor.h"
#include "ui/widgets/record_viewer.h"

//#include "ui/widgets/records_widget.h"

class PeopleTab : public QWidget
{
    Q_OBJECT

    QList<Person> people;

    int chosen_row = -1;

    QStackedWidget *widgetStack = nullptr;
    RecordsViewer *recordsViewer = nullptr;
    StringListEditor *propertyEditor = nullptr;


signals:
    void savePerson(Person person);
    void removePerson(int id, bool isTrainer);

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
        recordsViewer = new RecordsViewer(this);
        propertyEditor = new StringListEditor(this);

        widgetStack = new QStackedWidget(this);

        widgetStack->addWidget(recordsViewer);
        widgetStack->addWidget(propertyEditor);


        connect(recordsViewer, &RecordsViewer::createRecordIsRequred,
                this, &PeopleTab::on_createRecord);

        connect(recordsViewer, &RecordsViewer::editRecordIsRequred,
                this, &PeopleTab::on_editRecord);



        connect(propertyEditor, &StringListEditor::saveIsRequred,
                this, &PeopleTab::on_save);

        connect(propertyEditor, &StringListEditor::removeIsRequred,
                this, &PeopleTab::on_remove);

        connect(propertyEditor, &StringListEditor::exitIsRequred,
                this, &PeopleTab::on_editorExit);
    }

private slots:
    void on_createRecord()
    {
        this->chosen_row = -1;

        propertyEditor->updateContent(Person::getPattern());

        widgetStack->setCurrentIndex(1);
    }

    void on_editRecord(int row)
    {
        this->chosen_row = row;

        Person pers = people.at(row);

        propertyEditor->updateContent(Person::getPattern(), pers.getInList());

        widgetStack->setCurrentIndex(1);
    }

    void on_save(QList<QString> record)
    {
        if (this->chosen_row >= 0)
        {
            Person pers = people.at(chosen_row);
            pers.setInList(record);

            emit savePerson(pers);
        }
        else
        {
            Person pers(record);
            pers.isTrainer =
                    (this->who == Person::Who::TRAINERS)
                    ? true
                    : false;

            emit savePerson(pers);
        }

        on_editorExit();
    }

    void on_remove()
    {
        if (chosen_row >= 0)
        {
            Person pers = this->people.at(chosen_row);
            emit removePerson(pers.id, pers.isTrainer);
        }

        on_editorExit();
    }

    void on_editorExit()
    {
        chosen_row = -1;

        propertyEditor->updateContent({});

        widgetStack->setCurrentIndex(0);
    }

public:
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        widgetStack->resize(resizeEvent->size());
    }


};






























