#include "ui/groups_presenter.h"

GroupsPresenter::
GroupsPresenter(QWidget *parent)
    : QWidget(parent)
{
    setUpUi();
    setUpConnections();
}

void GroupsPresenter::
setGroupIconPath(QString groupIconPath)
{
    groupsViewer->setIconPath(groupIconPath);
}

void GroupsPresenter::
setTrainerIconPath(QString trainerIconPath)
{
    groupEditor->setTrainerIconPath(trainerIconPath);
}

void GroupsPresenter::
setSportsmanIconPath(QString sportsmanIconPath)
{
    groupEditor->setSportsmanIconPath(sportsmanIconPath);
}

void GroupsPresenter::
updateContent(QList<Person> sportsmen,
              QList<Person> trainers,
              QList<Group> groups)
{
    this->sportsmen = sportsmen;
    this->trainers = trainers;
    this->groups = groups;

    groupsViewer->updateContent(Group::toStringTable(groups),
                                Group::getPreviewPattern());

    if (widgetStack->currentIndex() == 1)
    {
        updateRunningEditor(sportsmen, trainers, groups);
    }
}

void GroupsPresenter::
editGroup(Group group, QString groupsScheduleName)
{
    showEditor(group, groupsScheduleName);
}

void GroupsPresenter::
setUpUi()
{
    createButton = new QPushButton("+");
    groupsViewer = new RecordsViewer;

    QVBoxLayout *viewerLayout = new QVBoxLayout;
    viewerLayout->addWidget(createButton);
    viewerLayout->addWidget(groupsViewer);

    QWidget *viewerWidget = new QWidget;
    viewerWidget->setLayout(viewerLayout);

    groupEditor = new GroupEditor;

    widgetStack = new QStackedWidget(this);
    widgetStack->addWidget(viewerWidget);
    widgetStack->addWidget(groupEditor);
}

void GroupsPresenter::
setUpConnections()
{
    connect(createButton, &QPushButton::clicked,
            [=] () { showEditor(); });
    connect(groupsViewer, &RecordsViewer::rowIsActivated,
            [=] (int row) { emit needEdit(groups.at(row)); });

    connect(groupEditor, &GroupEditor::needSave, [=] (Group group)
    {
        showGroupsPreview();
        emit needSave(group);
    });
    connect(groupEditor, &GroupEditor::needRemove, [=] (int id)
    {
        showGroupsPreview();
        emit needRemove(id);
    });
    connect(groupEditor, &GroupEditor::needExit, [=] ()
    {
        auto inputGroup  = groupEditor->getOldGroup();
        auto outputGroup = groupEditor->getCurrentGroup();

        if (inputGroup != outputGroup)
        {
            auto result = QMessageBox::question(this, " ",
                                                "У вас есть несохраненные изменения:\n"
                                                "Сохранить?");

            if (result == QMessageBox::Yes)
            {
                showGroupsPreview();
                emit needSave(outputGroup);
            }
        }
        showGroupsPreview();
    });
}

void GroupsPresenter::
showEditor(Group group, QString groupsScheduleName)
{
    QList<Person> availableSportsmen = Group::getFreeSportsmen(groups, sportsmen);
    availableSportsmen << group.sportsmen;

    groupEditor->updateContent(trainers, availableSportsmen, group);
    groupEditor->setScheduleLimit(groupsScheduleName);

    widgetStack->setCurrentIndex(1);
}

void GroupsPresenter::
showGroupsPreview()
{
    groupEditor->updateContent(QList<Person>(), QList<Person>(), Group());
    widgetStack->setCurrentIndex(0);
}

void GroupsPresenter::
updateRunningEditor(QList<Person> sportsmen,
                    QList<Person> trainers,
                    QList<Group> groups)
{
    if (groups.contains(groupEditor->getOldGroup())
            || groupEditor->getOldGroup().id == 0)
    {
        groupEditor->updateTrainersView(trainers);
        groupEditor->updateSportsmenView(sportsmen);
    }
    else
    {
        int result =
                QMessageBox::question(
                    this, " ",
                    "Редактируемая группа была изменена извне:\n"
                    "'" + groupEditor->getCurrentGroup().getPreviewProperty().join(" ") + "'\n"
                    + "Продолжить редактирование?");

        if (result == QMessageBox::Ok)
        {
            groupEditor->updateTrainersView(trainers);
            groupEditor->updateSportsmenView(sportsmen);
        }
        else if (result == QMessageBox::No)
        {
            showGroupsPreview();
        }
    }
}

void GroupsPresenter::
resizeEvent(QResizeEvent *resizeEvent)
{
    widgetStack->resize(resizeEvent->size());
}
