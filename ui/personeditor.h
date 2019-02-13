#pragma once

#include <QPushButton>
#include "ui/propertyeditor.h"

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
        EMPTY,
        CHILD,
        TRAINER
    } who = Who::EMPTY;


    PersonEditor(PersonEditor::Who who, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        this->who = who;

        commonConstructor();
    }


    PersonEditor(Person person, QWidget *parent = nullptr)
        : QWidget(parent)
    {        
        this->person = new Person(person);

        commonConstructor();
    }

private:
    void commonConstructor()
    {        
        this->pattern = Person::getPattern();

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
        QList<QString> savedList = propertyEditor->getInList();

        if (savedList.count() == pattern.count())
        {
            if (person != nullptr)
            {
                person->setInList(savedList);
            }
            else
            {
                person = new Person(savedList);

                person->isTrainer = (who == Who::TRAINER) ? true : false;
            }
        }

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

