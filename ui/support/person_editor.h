#pragma once

#include <QPushButton>
#include <QLabel>
#include <QFormLayout>
#include <QDateTimeEdit>
#include <QLineEdit>

#include "common/person.h"

class PersonEditor : public QWidget
{
    Q_OBJECT

    Person person;
    Person oldPerson;

    QLabel *ratingView;
    QLabel *eventsNumberView;

    QPushButton *saveButton = new QPushButton("Сохранить");
    QPushButton *removeButton = new QPushButton("Удалить");
    QPushButton *exitButton = new QPushButton("Выйти");

    // editors

    QLineEdit *firstNameEdit;
    QLineEdit *secondNameEdit;
    QLineEdit *lastNameEdit;
    QDateEdit *birthdayEdit;
    QLineEdit *sportTypeEdit;
    QLineEdit *phoneNumberEdit;

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
        this->oldPerson = person;

        setGroupLimit(""); // снимает лимит группы
        updateEditorFields(person);
    }

    void setGroupLimit(QString groupName = "")
    {
        removeButton->setEnabled(groupName.isEmpty());
        sportTypeEdit->setReadOnly(!groupName.isEmpty());

        if (!groupName.isEmpty())
        {
            removeButton->setToolTip(
                        "Человека нельзя удалить, пока он находится "
                        "в группе '" + groupName + "'");

            sportTypeEdit->setToolTip("Вид спорта нельзя изменять "
                                      "пока человек находится в группе '" + groupName + "'");
        }
        else
        {
            removeButton->setToolTip("");
            sportTypeEdit->setToolTip("");
        }
    }

    Person getCurrentPerson()
    {
        person.setFullData(firstNameEdit->text(),
                           secondNameEdit->text(),
                           lastNameEdit->text(),
                           birthdayEdit->date(),
                           sportTypeEdit->text(),
                           phoneNumberEdit->text());
        return person;
    }

    Person getOldPerson()
    {
        return oldPerson;
    }

private:
    void setUpUi()
    {
        birthdayEdit = new QDateEdit;
        birthdayEdit->setCalendarPopup(true);
        birthdayEdit->setDisplayFormat("dd.MM.yyyy");

        firstNameEdit = new QLineEdit;
        secondNameEdit = new QLineEdit;
        lastNameEdit = new QLineEdit;
        sportTypeEdit = new QLineEdit;
        phoneNumberEdit = new QLineEdit;

        ratingView = new QLabel;
        eventsNumberView = new QLabel;

        QFormLayout *editorsLayout = new QFormLayout;
        editorsLayout->addRow("Имя",            firstNameEdit);
        editorsLayout->addRow("Отчество",       secondNameEdit);
        editorsLayout->addRow("Фамилия",        lastNameEdit);
        editorsLayout->addRow("День рождения",  birthdayEdit);
        editorsLayout->addRow("Спорт",          sportTypeEdit);
        editorsLayout->addRow("Номер телефона", phoneNumberEdit);

        editorsLayout->addRow("Рейтинг",     ratingView);
        editorsLayout->addRow("Мероприятий", eventsNumberView);


        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(removeButton);
        buttonLayout->addWidget(exitButton);


        QVBoxLayout *basicLayout = new QVBoxLayout;
        basicLayout->addLayout(editorsLayout);
        basicLayout->addItem(buttonLayout);

        setLayout(basicLayout);
    }

    void updateEditorFields(Person pers)
    {
        firstNameEdit->setText(pers.firstName);
        secondNameEdit->setText(pers.secondName);
        lastNameEdit->setText(pers.lastName);
        phoneNumberEdit->setText(pers.phoneNumber);
        sportTypeEdit->setText(pers.sportType);

        birthdayEdit->setDate(QDate());
        birthdayEdit->setDate(pers.getBirthdayDate());

        ratingView->setText(QString::number(pers.rating));
        eventsNumberView->setText(QString::number(pers.eventsNumber));
    }

private slots:
    void on_save()
    {
        emit needSave(getCurrentPerson());
    }

    void on_remove()
    {
        emit needRemove(this->person.id);
    }
};




























