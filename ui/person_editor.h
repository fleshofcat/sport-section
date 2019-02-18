#pragma once

#include "ui/widgets/property_editor.h"

#include "common/common_objects.h"

class PersonEditor : public QWidget
{
    Q_OBJECT

    Person *person = nullptr;
    QList<QString> pattern;

    PropertyEditor *propertyEditor;

signals:
    void savePerson(Person person);
    void removePerson(int id);
    void exitPersonEditor();

public:
    enum class Who {
        CHILD,
        TRAINER
    };


    PersonEditor(Person pattern, PersonEditor::Who who, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        Person person;
        person.isTrainer = (who == Who::TRAINER) ? true : false;

        commonConstructor(pattern, person);
    }


    PersonEditor(Person pattern, Person person, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        commonConstructor(pattern, person);
    }

    PersonEditor(QWidget *parent = nullptr)
        : QWidget(parent) { }


private:
    void commonConstructor(Person pattern, Person pers)
    {
        this->pattern = pattern.getInList();
        this->person = new Person(pers);

        setUpUi();

        connect(propertyEditor, &PropertyEditor::saveIsRequred,
                this, &PersonEditor::onSaveButton);

        connect(propertyEditor, &PropertyEditor::removeIsRequred,
                this, &PersonEditor::onRemoveButton);

        connect(propertyEditor, &PropertyEditor::exitIsRequred,
                this, &PersonEditor::exitPersonEditor);

    }


    void setUpUi()
    {
        if (person != nullptr)
        {
            propertyEditor = new PropertyEditor(pattern, person->getInList());
        }
        else
        {
            propertyEditor = new PropertyEditor(pattern);
        }


        QVBoxLayout *basicLayout = new QVBoxLayout;

        basicLayout->addWidget(propertyEditor);

        setLayout(basicLayout);

        show();
    }


    void onSaveButton(QList<QString> personInList)
    {
        person->setInList(personInList);

        emit savePerson(*person);
        emit exitPersonEditor();
    }


    void onRemoveButton()
    {
        if (person != nullptr)
        {
            emit removePerson(person->id);
        }

        emit exitPersonEditor();
    }
};

