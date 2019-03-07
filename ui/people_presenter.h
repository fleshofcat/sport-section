#pragma once

#include <QResizeEvent>
#include <QStackedWidget>
#include <QMessageBox>

#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>

#include "common/person.h"
#include "ui/support/person_editor.h"
#include "ui/widgets/records_viewer.h"

class PeoplePresenter : public QWidget
{
    Q_OBJECT

    QString icon_path;
    QList<Person> people;

    QPushButton *createButton;

    RecordsViewer *peopleViewer;
    PersonEditor *editor;
    QStackedWidget *widgets;


signals:
    void savePerson(Person person);
    void removePerson(int id);

public:
    PeoplePresenter(QString icon_path, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        this->icon_path = icon_path;
        setUpUi();
    }

    PeoplePresenter(QWidget *parent = nullptr)
        : PeoplePresenter("", parent) { }


    void updateContent(QList<Person> people)
    {
        this->people = people;

        peopleViewer->updateContent(Person::toStringTable(people), Person::pattern());
    }

    void showWarning(QString warning)
    {
        QMessageBox::warning(this, "Предупреждение", warning);
    }

private:
    void setUpUi()
    {
        createButton = new QPushButton("+");

        peopleViewer = new RecordsViewer;
        peopleViewer->setIconPath(icon_path);

        QVBoxLayout *viewerLayout = new QVBoxLayout;
        viewerLayout->addWidget(createButton);
        viewerLayout->addWidget(peopleViewer);

        QWidget *viewer_w = new QWidget;
        viewer_w->setLayout(viewerLayout);

        editor = new PersonEditor;

        widgets = new QStackedWidget(this);
        widgets->addWidget(viewer_w);
        widgets->addWidget(editor);


        connect(createButton, &QPushButton::clicked, this, [=] ()
        {
            setEditMode();
        });
        connect(peopleViewer, &RecordsViewer::rowIsActivated, [=] (int row)
        {
            if (row >= 0)
            {
                setEditMode(people.at(row));
            }
        });

        connect(editor, &PersonEditor::needSave, [=] (Person pers)
        {
            setViewMode();
            emit savePerson(pers);
        });
        connect(editor, &PersonEditor::needRemove, [=] (int id)
        {
            setViewMode();
            emit removePerson(id);
        });
        connect(editor, &PersonEditor::needExit,
                this, &PeoplePresenter::setViewMode);
    }

private slots:
    void setEditMode(Person pers = Person())
    {
        editor->updateContent(pers);
        widgets->setCurrentIndex(1);
    }

    void setViewMode()
    {
        editor->updateContent();
        widgets->setCurrentIndex(0);
    }

public:
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        widgets->resize(resizeEvent->size());
    }
};




























