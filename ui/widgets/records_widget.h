#pragma once

#include <QResizeEvent>

#include "ui/widgets/property_editor.h"
#include "ui/widgets/record_viewer.h"

class RecordsWidget : public QWidget
{
Q_OBJECT

    QList<QString> pattern;
    QList<QList<QString>> stringTable;

    int chosen_row = -1;

    RecordsViewer *recordsViewer = nullptr;
    PropertyEditor *propertyEditor = nullptr;

signals:
    void createRecordIsRequred(QList<QString> record);
    void saveRecordIsRequred(int row, QList<QString> record);
    void removeRecordIsRequred(int row);


public:
    RecordsWidget(QList<QString> pattern,
                      QList<QList<QString>> stringTable,
                      QWidget *parent = nullptr)
        : QWidget (parent)
    {
        constructor();
        updateData(pattern, stringTable);
    }

    RecordsWidget(QWidget *parent = nullptr)
        : QWidget (parent)
    {
        constructor();
    }

    void updateData(QList<QString> pattern,
                QList<QList<QString>> stringTable)
    {
        this->pattern = pattern;
        this->stringTable = stringTable;

        recordsViewer->updateData(pattern, stringTable);
    }

private:
    void constructor()
    {
        setUpUi();

        connect(recordsViewer, &RecordsViewer::createRecordIsRequred,
                this, &RecordsWidget::on_createRecord);

        connect(recordsViewer, &RecordsViewer::editRecordIsRequred,
                this, &RecordsWidget::on_editRecord);



        connect(propertyEditor, &PropertyEditor::saveIsRequred,
                this, &RecordsWidget::on_propertySave_button);

        connect(propertyEditor, &PropertyEditor::removeIsRequred,
                this, &RecordsWidget::on_propertyRemove_button);

        connect(propertyEditor, &PropertyEditor::exitIsRequred,
                this, &RecordsWidget::on_propertyExit_button);

    }

    void setUpUi()
    {
        // устанавливается режим показа таблиц
        // они будет показывать элементы в режиме авторастягивания

        recordsViewer = new RecordsViewer(this);
        recordsViewer->resize(size());
        recordsViewer->show();

        propertyEditor = new PropertyEditor({}, {}, this);
        propertyEditor->hide();
        propertyEditor->resize(size());
    }


private slots:
    void on_createRecord(QList<QString> pattern)
    {
        this->chosen_row = -1;

        propertyEditor->updateContent(pattern);

        propertyEditor->show();
        recordsViewer->hide();
    }

    void on_editRecord(int row)
    {
        this->chosen_row = row;

        propertyEditor->updateContent(pattern, stringTable.at(row));

        propertyEditor->show();
        recordsViewer->hide();
    }



    void on_propertySave_button(QList<QString> record)
    {
        if (this->chosen_row >= 0 && this->chosen_row < stringTable.count())
        {
            emit saveRecordIsRequred(this->chosen_row, record);
        }
        else
        {
            emit createRecordIsRequred(propertyEditor->getInList());
        }

        on_propertyExit_button();
    }

    void on_propertyRemove_button()
    {
        if (this->chosen_row >= 0
                && this->chosen_row < stringTable.count())
        {
            emit removeRecordIsRequred(this->chosen_row);
        }

        on_propertyExit_button();
    }

    void on_propertyExit_button()
    {
        this->chosen_row = -1;

        propertyEditor->updateContent({});

        propertyEditor->hide();
        recordsViewer->show();
    }

public:
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        recordsViewer->resize(resizeEvent->size());
        propertyEditor->resize(resizeEvent->size());
    }

};




























