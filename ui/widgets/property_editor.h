#pragma once

#include <QFormLayout>
#include <QLineEdit>

class PropertyEditor : public QWidget
{
Q_OBJECT

    QList<QString> pattern;
    QList<QString> record;

    QList<QLineEdit*> editors;

    QFormLayout *layout = nullptr;

public:
    PropertyEditor(QList<QString> pattern,
                   QList<QString> record = {},
                   QWidget *parent = nullptr)
        : QWidget (parent)
    {
        this->pattern = pattern;
        this->record = record;

        setUpUi();
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

    QList<QString> getPattern()
    {
        return pattern;
    }

    void updateContent(QList<QString> pattern,
                       QList<QString> record = {})
    {
        this->pattern = pattern;
        this->record = record;

        for (auto editor : editors)
        {
            delete editor;
        }

        if (layout != nullptr)
        {
            delete layout;
        }

        setLayout(nullptr);

        setUpUi();
    }

private:
    void setUpUi()
    {
        layout = new QFormLayout;

        if (pattern.count() == record.count())
        {
            for (int i = 0; i < pattern.count(); i++)
            {
                editors << new QLineEdit(record.at(i));
                layout->addRow(pattern.at(i), editors.last());
            }
        }
        else
        {
            for (QString field : pattern)
            {
                editors << new QLineEdit();
                layout->addRow(field, editors.last());
            }
        }

        setLayout(layout);
        show();
    }


private slots:
};




























