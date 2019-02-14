#pragma once

#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>

class PropertyEditor : public QWidget
{
Q_OBJECT

    QPushButton *saveButton = new QPushButton("Сохранить");
    QPushButton *removeButton = new QPushButton("удалить");
    QPushButton *exitButton = new QPushButton("Выйти");

    QList<QLineEdit*> editors;

    QVBoxLayout *basicLayout = nullptr;

signals:
    void saveIsRequred(QList<QString> record);
    void removeIsRequred();
    void exitIsRequred();

public:
    PropertyEditor(QList<QString> pattern,
                   QList<QString> record = {},
                   QWidget *parent = nullptr)
        : QWidget (parent)
    {
        setUpUi(pattern, record);

        connect(saveButton, &QPushButton::pressed, this, &PropertyEditor::on_saveButton);
        connect(removeButton, &QPushButton::pressed, this, &PropertyEditor::removeIsRequred);
        connect(exitButton, &QPushButton::pressed, this, &PropertyEditor::exitIsRequred);

    }

    QList<QString> getInList()
    {
        QList<QString> savedList;

        for (QLineEdit *editor : editors)
        {
            savedList << editor->text();
        }

        return savedList;
    }

    void updateContent(QList<QString> pattern,
                       QList<QString> record = {})
    {
        for (QLineEdit *editor : editors)
        {
            delete editor;
        } editors.clear();

        if (basicLayout != nullptr)
        {
            delete basicLayout;
            basicLayout = nullptr;
        }

//        setLayout(nullptr);

        setUpUi(pattern, record);
    }

private:
    void setUpUi(QList<QString> pattern,
                 QList<QString> record = {})
    {
        QFormLayout *formLayout = new QFormLayout;

        if (pattern.count() == record.count())
        {
            for (int i = 0; i < pattern.count(); i++)
            {
                editors << new QLineEdit(record.at(i));
                formLayout->addRow(pattern.at(i), editors.last());
            }
        }
        else
        {
            for (QString field : pattern)
            {
                editors << new QLineEdit();
                formLayout->addRow(field, editors.last());
            }
        }

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(removeButton);
        buttonLayout->addWidget(exitButton);

        basicLayout = new QVBoxLayout;
        basicLayout->addItem(formLayout);
        basicLayout->addItem(buttonLayout);

        setLayout(basicLayout);
        show();
    }

private slots:
    void on_saveButton()
    {
        emit saveIsRequred(getInList());
    }

};




























