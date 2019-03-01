#pragma once

#include <QResizeEvent>
#include <QStackedWidget>
#include <QMessageBox>

#include "common/person.h"
#include "ui/person_editor.h"
#include "ui/widgets/records_widget.h"

class PeoplePresentor : public QWidget
{
    Q_OBJECT

    QList<Person> people;

    QStackedWidget *widgets;
    RecordsWidget *viewer;
    PersonEditor *editor;

signals:
    void savePerson(Person person);
    void removePerson(int id);

public:
    PeoplePresentor(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setUpUi();
    }


    void updateContent(QList<Person> people)
    {
        this->people = people;

        viewer->updateData(
                    Person::getPattern(),
                    Person::personListToStringTable(people));
    }

    void showWarning(QString warning)
    {
        QMessageBox::warning(this, "Предупреждение", warning);
    }

private:
    void setUpUi()
    {
        viewer = new RecordsWidget;
        editor = new PersonEditor;

        widgets = new QStackedWidget(this);
        widgets->addWidget(viewer);
        widgets->addWidget(editor);

        connect(viewer, &RecordsWidget::createRecordActivate, this, &PeoplePresentor::on_createRecord);
        connect(viewer, &RecordsWidget::recordActivated,   this, &PeoplePresentor::on_editRecord);

        connect(editor, &PersonEditor::needSave,   this, &PeoplePresentor::on_needSave);
        connect(editor, &PersonEditor::needRemove, this, &PeoplePresentor::on_needRemove);
        connect(editor, &PersonEditor::needExit,   this, &PeoplePresentor::setPreviewMode);
    }

private slots:
    void setEditMode(Person pers = Person())
    {
        editor->updateContent(pers);
        widgets->setCurrentIndex(1);
    }

    void setPreviewMode()
    {
        editor->updateContent();
        widgets->setCurrentIndex(0);
    }

    void on_createRecord()
    {
        setEditMode();
    }

    void on_editRecord(int row)
    {
        if (row >= 0)
        {
            setEditMode(people.at(row));
        }
    }

    void on_needSave(Person pers)
    {
        emit savePerson(pers);
        setPreviewMode();
    }

    void on_needRemove(int id)
    {
        emit removePerson(id);
        setPreviewMode();
    }

public:
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        widgets->resize(resizeEvent->size());
    }
};




























