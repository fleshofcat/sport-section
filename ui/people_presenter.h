#pragma once

#include <QResizeEvent>
#include <QStackedWidget>
#include <QMessageBox>

#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>

#include "common/person.h"
#include "ui/person_editor.h"

class PeoplePresenter : public QWidget
{
    Q_OBJECT

    QString icon_path;
    QList<Person> people;

    QPushButton *createButton;

    QTableView *peopleViewer;
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

        int columns = Person::pattern().count();
        int rows = people.count();
        auto stringTable = Person::toStringTable(people);


        QStandardItemModel *model = new QStandardItemModel(rows, columns);

        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < columns; c++)
            {
                QModelIndex index = model->index(r, c);
                model->setData(index, stringTable.at(r).at(c));
            }
            model->setVerticalHeaderItem(r, new QStandardItem(QIcon(icon_path), ""));
        }
        model->setHorizontalHeaderLabels(Person::pattern());
        peopleViewer->setModel(model);
    }

    void showWarning(QString warning)
    {
        QMessageBox::warning(this, "Предупреждение", warning);
    }

private:
    void setUpUi()
    {
        createButton = new QPushButton("+");

        peopleViewer = new QTableView;
        peopleViewer->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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
        connect(peopleViewer, &QTableView::clicked, [=] (QModelIndex index)
        {
            if (index.row() >= 0)
            {
                setEditMode(people.at(index.row()));
            }
        });

        connect(editor, &PersonEditor::needSave,   this, &PeoplePresenter::on_needSave);
        connect(editor, &PersonEditor::needRemove, this, &PeoplePresenter::on_needRemove);
        connect(editor, &PersonEditor::needExit,   this, &PeoplePresenter::setPreviewMode);
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




























