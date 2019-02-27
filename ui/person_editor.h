#pragma once

#include <QPushButton>

#include "common/common_objects.h"
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
    void saveIsRequred(Person person);
    void removeIsRequred(int id);
    void exitIsRequred();

public:
    PersonEditor(QWidget *parent = nullptr)
        : PersonEditor(Person(), parent) { }

    PersonEditor(Person person, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();


        connect(saveButton, &QPushButton::clicked,
                this, &PersonEditor::on_savePerson);

        connect(removeButton, &QPushButton::clicked,
                this, &PersonEditor::on_removePerson);

        connect(exitButton, &QPushButton::clicked,
                this, &PersonEditor::exitIsRequred);


        updateContent(person);
    }

    void updateContent(Person person)
    {
        this->person = person;

        editorsBox->updateContent(Person::getPattern(), person.getInList());
    }

    void dropContent()
    {
        updateContent(Person());
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
    void on_savePerson()
    {
        auto personInList = editorsBox->getInList();

        person.setInList(personInList);

        emit saveIsRequred(person);
    }

    void on_removePerson()
    {
        emit removeIsRequred(this->person.id);
    }
};




























