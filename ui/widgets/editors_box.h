#pragma once

#include <QFormLayout>
#include <QLineEdit>

#include "ui/widgets/property_editor.h"
#include "ui/widgets/record_viewer.h"

class EditorsBox : public QWidget
{
    Q_OBJECT

    QList<QLineEdit*> editors;
    QFormLayout *formLayout;

public:
    EditorsBox(QWidget *parent = nullptr)
        : EditorsBox({}, {}, parent) { }

    EditorsBox(QList<QString> pattern,
               QList<QString> record = {},
               QWidget *parent = nullptr)
        : QWidget (parent)
    {
        updateContent(pattern, record);
    }

    void updateContent(QList<QString> pattern,
                    QList<QString> records = {})
    {
        stringListEqualizing(pattern, records);
        dropState();

        formLayout = new QFormLayout;
        while (pattern.count())
        {
            editors << new QLineEdit(records.takeFirst());
            formLayout->addRow(pattern.takeFirst(), editors.last());
        }

        setLayout(formLayout);
    }

    QList<QString> getInList()
    {
        QList<QString> property;

        for (QLineEdit *editor : editors)
        {
            property << editor->text();
        }

        return property;
    }

protected:
    void dropState()
    {
        qDeleteAll(this->children());
        editors.clear();
    }

private:
    void stringListEqualizing(QList<QString> &pattern,
                              QList<QString> &records)
    {
        if (pattern.count() > records.count())
        {
            int count = pattern.count() - records.count();

            while (count--)
                records << "";
        }
    }
};




























