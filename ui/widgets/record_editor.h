#pragma once

#include <QPushButton>

#include "ui/widgets/editors_box.h"

class RecordEditor : public EditorsBox
{
    Q_OBJECT

    QPushButton *saveButton = new QPushButton("Сохранить");
    QPushButton *removeButton = new QPushButton("удалить");
    QPushButton *exitButton = new QPushButton("Выйти");

signals:
    void saveIsRequred(QList<QString> record);
    void removeIsRequred();
    void exitIsRequred();

public:
    RecordEditor(QWidget *parent = nullptr)
        : RecordEditor({}, {}, parent) {}

    RecordEditor(QList<QString> pattern,
                     QList<QString> record = {},
                     QWidget *parent = nullptr)
              : EditorsBox(pattern, record, parent)
    {
        setUpUi();

        connect(saveButton, &QPushButton::pressed,
                this, &RecordEditor::on_saveButton);

        connect(removeButton, &QPushButton::pressed,
                this, &RecordEditor::removeIsRequred);

        connect(exitButton, &QPushButton::pressed,
                this, &RecordEditor::exitIsRequred);
    }

private:
    void setUpUi()
    {
        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(removeButton);
        buttonLayout->addWidget(exitButton);

        basicLayout->addLayout(buttonLayout);
    }

private slots:
    void on_saveButton()
    {
        emit saveIsRequred(getInList());
    }
};








































