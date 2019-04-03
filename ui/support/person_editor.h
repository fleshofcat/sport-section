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
    explicit PersonEditor(QWidget *parent = nullptr);
    explicit PersonEditor(Person person, QWidget *parent = nullptr);

    void updateContent(Person person = Person());
    Person getCurrentPerson();
    Person getOldPerson();
    void setGroupLimit(QString groupName = "");

private:
    void setUpUi();
    void updateEditorFields(Person pers);

private slots:
    void on_save();
    void on_remove();
};




























