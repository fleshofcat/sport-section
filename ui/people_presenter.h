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
    explicit
    PeoplePresenter(QString icon_path,
                    QWidget *parent = nullptr);
    explicit
    PeoplePresenter(QWidget *parent = nullptr);

    void updateContent(QList<Person> people);
    void editPerson(Person pers = Person(),
                    QString personsGroupName = "");
private:
    void setUpUi();
    void setUpConnections();

    void updateRunningEditor(QList<Person> people);

private slots:
    void showEditor();
    void showPeoplePreview();

public:
    void resizeEvent(QResizeEvent *resizeEvent);
};




























