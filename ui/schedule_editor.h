#pragma once

#include <QPushButton>

#include "common/common_objects.h"

#include "ui/widgets/string_table_viewer.h"
#include "ui/widgets/property_editor.h"


class ScheduleEditor : public QWidget
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


    ScheduleEditor(QWidget *parent = nullptr)
        : QWidget(parent)
    { }




private:

};

