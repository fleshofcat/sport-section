#pragma once

#include <QPushButton>
#include "ui/widgets/property_editor.h"

#include "common/common_objects.h"

class PersonEditor : public QWidget
{
    Q_OBJECT

    Person *person = nullptr;
    QList<QString> pattern;
    QList<QString> personInList;

    PropertyEditor *propertyEditor;

    QPushButton *saveButton = new QPushButton("Сохранить");
    QPushButton *removeButton = new QPushButton("удалить");
    QPushButton *exitButton = new QPushButton("Выйти");

signals:
    void savePerson(Person person);
    void removePerson(int id);
    void exitPersonEditor();

public:
    enum class Who {
        CHILD,
        TRAINER
    };


    PersonEditor(PersonEditor::Who who, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        Person pers;
        pers.isTrainer = (who == Who::TRAINER) ? true : false;

        commonConstructor(pers);
    }


    PersonEditor(Person person, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        commonConstructor(person);
    }

    PersonEditor(QWidget *parent = nullptr)
        : QWidget(parent)
    {

    }


private:
    void commonConstructor(Person pers)
    {
        this->pattern = Person::getPattern();
        this->person = new Person(pers);

        setUpUi();

        connect(saveButton, &QPushButton::clicked, this, &PersonEditor::onSaveButton);
        connect(removeButton, &QPushButton::clicked, this, &PersonEditor::onRemoveButton);
        connect(exitButton, &QPushButton::clicked, this, &PersonEditor::exitPersonEditor);
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



        QHBoxLayout * buttonLayout = new QHBoxLayout;

        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(removeButton);
        buttonLayout->addWidget(exitButton);

        QVBoxLayout *verticalLayout = new QVBoxLayout;

        verticalLayout->addWidget(propertyEditor);
        verticalLayout->addItem(buttonLayout);

        setLayout(verticalLayout);

        show();
    }


    void onSaveButton()
    {
        person->setInList(propertyEditor->getInList());

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

