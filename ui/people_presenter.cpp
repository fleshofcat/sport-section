#include "ui/people_presenter.h"

PeoplePresenter::
PeoplePresenter(QString icon_path, QWidget *parent)
    : QWidget(parent)
{
    this->icon_path = icon_path;
    setUpUi();
    setUpConnections();
}

PeoplePresenter::PeoplePresenter(QWidget *parent)
    : PeoplePresenter("", parent) { }


void PeoplePresenter::
updateContent(QList<Person> people)
{
    this->people = people;

    peopleViewer->updateContent(Person::toPreviewTable(people), Person::getPreviewPattern());

    if (widgetStack->currentIndex() == 1)
    {
        updateRunningEditor(people);
    }
}

void PeoplePresenter::
editPerson(Person pers, QString personsGroupName)
{
    personEditor->updateContent(pers);
    personEditor->setGroupLimit(personsGroupName);

    showEditor();
}

void PeoplePresenter::
setUpUi()
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
}

void PeoplePresenter::
setUpConnections()
{
    connect(createButton, &QPushButton::clicked,
            [=] () { editPerson(Person()); });
    connect(peopleViewer, &RecordsViewer::rowIsActivated,
            [=] (int row) { emit needEdit(people.at(row)); });

    connect(personEditor, &PersonEditor::needSave, [=] (Person pers)
    {
        showPeoplePreview();
        emit needSave(pers);
    });
    connect(personEditor, &PersonEditor::needRemove, [=] (int id)
    {
        showPeoplePreview();
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
                showPeoplePreview();
                emit needSave(outputPers);
            }
        }

        showPeoplePreview();
    });
}

void PeoplePresenter::
showEditor()
{
    widgetStack->setCurrentIndex(1);
}

void PeoplePresenter::
showPeoplePreview()
{
    widgetStack->setCurrentIndex(0);
}

void PeoplePresenter::
updateRunningEditor(QList<Person> people)
{
    if (!people.contains(personEditor->getOldPerson()) // if edited person is not exist
            && personEditor->getOldPerson().id != 0)   // and he is not new person
    {
        int result = QMessageBox::question(this, " ",
                "Редактируемый человек был изменен извне:\n"
                "'" + personEditor->getCurrentPerson().getPreviewProperty().join(" ") + "'\n"
                "Продолжить редактирование?");

        if (result == QMessageBox::No)
        {
             showPeoplePreview();
        }
    }
}

void PeoplePresenter::
resizeEvent(QResizeEvent *resizeEvent)
{
    widgetStack->resize(resizeEvent->size());
}

















