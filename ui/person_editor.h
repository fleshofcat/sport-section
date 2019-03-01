#pragma once

#include <QPushButton>

#include "common/person.h"
#include "ui/widgets/editors_box.h"

class PersonEditor : public QWidget
{
    Q_OBJECT

    Person person;

    QPushButton *saveButton = new QPushButton("Сохранить");
    QPushButton *removeButton = new QPushButton("Удалить");
    QPushButton *exitButton = new QPushButton("Выйти");

    EditorsBox *editorsBox;

signals:
    void needSave(Person person);
    void needRemove(int id);
    void needExit();

public:
    PersonEditor(QWidget *parent = nullptr)
        : PersonEditor(Person(), parent) { }

    PersonEditor(Person person, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();

        connect(saveButton,   &QPushButton::clicked, this, &PersonEditor::on_save);
        connect(removeButton, &QPushButton::clicked, this, &PersonEditor::on_remove);
        connect(exitButton,   &QPushButton::clicked, this, &PersonEditor::needExit);

        updateContent(person);
    }

    void updateContent(Person person = Person())
    {
        this->person = person;
        editorsBox->updateContent(Person::getPattern(), person.getInList());
    }

    Person getPerson()
    {
        person.setInList(editorsBox->getInList());
        return person;
    }

private:
    void setUpUi()
    {
        editorsBox = new EditorsBox;

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(removeButton);
        buttonLayout->addWidget(exitButton);


        QVBoxLayout *basicLayout = new QVBoxLayout;
        basicLayout->addWidget(editorsBox);
        basicLayout->addItem(buttonLayout);

        setLayout(basicLayout);
    }

private slots:
    void on_save()
    {
        person.setInList(editorsBox->getInList());
        emit needSave(person);
    }

    void on_remove()
    {
        emit needRemove(this->person.id);
    }
};




























