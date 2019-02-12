#pragma once

#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

#include "common/common_objects.h"

class PersonEditor : public QWidget
{
    Q_OBJECT

    Person *person = nullptr;
    QList<QString> pattern;
    QList<QString> personInList;

    QList<QLineEdit*> editors;

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
        QFormLayout *editorLayout = new QFormLayout;


        if (person != nullptr)
        {
            for (int i = 0; i < pattern.count(); i++)
            {
                editors << new QLineEdit(person->getInList().at(i));
                editorLayout->addRow(pattern.at(i), editors.last());
            }
        }
        else
        {
            for (QString field : pattern)
            {
                editors << new QLineEdit();
                editorLayout->addRow(field, editors.last());
            }
        }

        QHBoxLayout * buttonLayout = new QHBoxLayout;

        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(removeButton);
        buttonLayout->addWidget(exitButton);

        QVBoxLayout *verticalLayout = new QVBoxLayout;

        verticalLayout->addItem(editorLayout);
        verticalLayout->addItem(buttonLayout);

        setLayout(verticalLayout);

        show();
    }


    void onSaveButton()
    {
        QList<QString> savedList;

        for (QLineEdit *editor : editors)
        {
            savedList << editor->text();
        }

        if (person != nullptr)
        {
            person->setInList(savedList);
        }
        else
        {
            person = new Person(savedList);

            person->isTrainer = (who == Who::TRAINER) ? true : false;
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

