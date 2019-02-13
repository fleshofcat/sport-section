#pragma once

#include <QFormLayout>
#include <QLineEdit>

class PropertyEditor : public QWidget
{
Q_OBJECT

    QList<QString> pattern;
    QList<QString> record;

    QList<QLineEdit*> editors;

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

private:
    void setUpUi()
    {
        QFormLayout *editorLayout = new QFormLayout;

        if (pattern.count() == record.count())
        {
            for (int i = 0; i < pattern.count(); i++)
            {
                editors << new QLineEdit(record.at(i));
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

        setLayout(editorLayout);
        show();
    }


private slots:
};




























