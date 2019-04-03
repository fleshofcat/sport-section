#pragma once

#include <QLabel>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

#include "common/group.h"
#include "common/person.h"

#include "ui/widgets/records_viewer.h"
#include "ui/widgets/record_chooser.h"

class GroupEditor : public QWidget
{
    Q_OBJECT

    friend class TestGroupEditor;

    QString trainerIconPath;
    QString sportsmanIconPath;

    // data
    QList<Person> allSportsmen;
    QList<Person> allTrainers;
    Group oldGroup;
    Group group;

    // standart buttons
    QPushButton *saveButton;
    QPushButton *removeButton;
    QPushButton *exitButton;

    // editors
    QLineEdit *groupNameField;

    // not editable
    QLineEdit *ratingField;
    QLineEdit *eventCountField;
    QLineEdit *sportTypeField;

    // people in group
    QPushButton *addTrainerButton;
    QPushButton *addSportsmanButton;
    RecordsViewer *trainersViewer;
    RecordsViewer *sportsmenViewer;

signals:
    void needSave(Group group);
    void needRemove(int id);
    void needExit();

public:
    explicit GroupEditor(QWidget *parent = nullptr);
    explicit GroupEditor(QList<Person> trainers,
                         QList<Person> sportsmen,
                         Group group,
                         QWidget *parent = nullptr);

    void setTrainerIconPath(QString trainerIconPath);
    void setSportsmanIconPath(QString sportsmanIconPath);

    void updateContent(QList<Person> trainers,
                       QList<Person> sportsmen,
                       Group group = Group());

    void updateTrainersView(QList<Person> trainers);
    void updateSportsmenView(QList<Person> sportsmen);

    Group getCurrentGroup();
    Group getOldGroup();

    void setScheduleLimit(QString scheduleName = "");

private:
    void setUpUi();
    void setUpConnects();
    Person choosePerson(QList<Person> peopleToShow, QString iconPath);
    bool askQuestion(QString question);
};




























