#include "ui/support/person_editor.h"


PersonEditor::PersonEditor(QWidget *parent)
    : PersonEditor(Person(), parent) { }

PersonEditor::PersonEditor(Person person, QWidget *parent)
    : QWidget(parent)
{
    setUpUi();

    connect(saveButton,   &QPushButton::clicked, this, &PersonEditor::on_save);
    connect(removeButton, &QPushButton::clicked, this, &PersonEditor::on_remove);
    connect(exitButton,   &QPushButton::clicked, this, &PersonEditor::needExit);

    updateContent(person);
}

void PersonEditor::updateContent(Person person)
{
    this->person = person;
    this->oldPerson = person;

    setGroupLimit(""); // снимает лимит группы
    updateEditorFields(person);
}

void PersonEditor::setGroupLimit(QString groupName)
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

Person PersonEditor::getCurrentPerson()
{
    person.setFirstName(firstNameEdit->text());
    person.setSecondName(secondNameEdit->text());
    person.setLastName(lastNameEdit->text());
    person.setBirthday(birthdayEdit->text());
    person.setSportType(sportTypeEdit->text());
    person.setPhoneNumber(phoneNumberEdit->text());

    return person;
}

Person PersonEditor::getOldPerson()
{
    return oldPerson;
}


void PersonEditor::setUpUi()
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

void PersonEditor::updateEditorFields(Person pers)
{
    firstNameEdit->setText(pers.getFirstName());
    secondNameEdit->setText(pers.getSecondName());
    lastNameEdit->setText(pers.getLastName());
    phoneNumberEdit->setText(pers.getPhoneNumber());
    sportTypeEdit->setText(pers.getSportType());

    birthdayEdit->setDate(QDate());
    birthdayEdit->setDate(pers.getBirthdayInDate());

    ratingView->setText(QString::number(pers.getRating()));
    eventsNumberView->setText(QString::number(pers.getEventsNumber()));
}

void PersonEditor::on_save()
{
    emit needSave(getCurrentPerson());
}

void PersonEditor::on_remove()
{
    emit needRemove(this->person.id);
}
