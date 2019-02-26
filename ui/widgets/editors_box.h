#pragma once

#include <QFormLayout>
#include <QLineEdit>

class EditorsBox : public QWidget
{
    Q_OBJECT

    QList<QLineEdit*> editors;

//signals:
//    void enterPressed();

protected:
    QWidget *basicWidget;
    QVBoxLayout *basicLayout;

public:
    EditorsBox(QWidget *parent = nullptr)
        : EditorsBox({}, {}, parent) { }

    EditorsBox(QList<QString> pattern,
               QList<QString> record = {},
               QWidget *parent = nullptr)
        : QWidget (parent)
    {
        basicWidget = new QWidget(this);
        basicLayout = new QVBoxLayout();
        basicLayout->addWidget(basicWidget);
        setLayout(basicLayout);



        updateContent(pattern, record);
    }

    void updateContent(QList<QString> pattern,
                    QList<QString> records = {})
    {
        stringListEqualizing(pattern, records);
        dropState();

        QFormLayout *formLayout = new QFormLayout;
        while (pattern.count())
        {
            editors << new QLineEdit(records.takeFirst());
            formLayout->addRow(pattern.takeFirst(), editors.last());

//            connect(editors.last(), &QLineEdit::returnPressed,
//                    this, &EditorsBox::enterPressed);
        }

        basicWidget->setLayout(formLayout);
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
        qDeleteAll(basicWidget->children());
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
























