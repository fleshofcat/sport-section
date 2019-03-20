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
    PersonEditor *personEditor;
    QStackedWidget *widgetStack;

signals:
    void needSave(Person person);
    void needRemove(int id);
    void needEdit(Person person);

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

        peopleViewer->updateContent(Person::toPreviewStringTable(people), Person::getPreviewPattern());

        if (widgetStack->currentIndex() == 1)
        {
            updateRunningEditor(people);
        }
    }

    void editPerson(Person pers, QString personsGroup)
    {
        showEditor(pers, personsGroup);
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

        personEditor = new PersonEditor;

        widgetStack = new QStackedWidget(this);
        widgetStack->addWidget(viewer_w);
        widgetStack->addWidget(personEditor);


        connect(createButton, &QPushButton::clicked,
                [=] () { showEditor(); });
        connect(peopleViewer, &RecordsViewer::rowIsActivated,
                [=] (int row) { emit needEdit(people.at(row)); });

        connect(personEditor, &PersonEditor::needSave, [=] (Person pers)
        {
            showPeople();
            emit needSave(pers);
        });
        connect(personEditor, &PersonEditor::needRemove, [=] (int id)
        {
            showPeople();
            emit needRemove(id);
        });
        connect(personEditor, &PersonEditor::needExit, [=] ()
        {
            auto inputPers = personEditor->getOldPerson();
            auto outputPers = personEditor->getCurrentPerson();

            if (inputPers != outputPers)
            {
                auto result = QMessageBox::question(this, " ",
                                "У вас есть несохраненные изменения:\n"
                                "Сохранить?");

                if (result == QMessageBox::Yes)
                {
                    showPeople();
                    emit needSave(outputPers);
                }
            }

            showPeople();
        });
    }

private slots:
    void showEditor(Person pers = Person(), QString personsGroupName = "")
    {
        personEditor->updateContent(pers);
        personEditor->setGroupLimit(personsGroupName);

        widgetStack->setCurrentIndex(1);
    }

    void showPeople()
    {
//        personEditor->updateContent();
        widgetStack->setCurrentIndex(0);
    }

    void updateRunningEditor(QList<Person> people)
    {
        if (!people.contains(personEditor->getOldPerson()) // if edited person is not exist
                && personEditor->getOldPerson().id != 0)   // and he is not new person
        {
            int result = QMessageBox::question(this, " ",
                    "Редактируемый человек был изменен извне:\n"
                    "'" + personEditor->getCurrentPerson().getPreviewList().join(" ") + "'\n"
                    "Продолжить редактирование?");

            if (result == QMessageBox::No)
            {
                 showPeople();
            }
        }
    }

public:
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        widgetStack->resize(resizeEvent->size());
    }
};




























