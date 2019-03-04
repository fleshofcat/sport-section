#pragma once

#include <QStackedWidget>

#include <QStandardItemModel>

#include "ui/schedule_editor.h"

class SchedulePresenter : public QWidget
{
    Q_OBJECT
    friend class TestSchedulePresenter;

    QString icon_path;
    QList<Schedule> schedules;
    QList<Group> groups;

    QPushButton *createButton;
    QTableView *scheduleViewer;
//    RecordsWidget *schedulesViewer;
    QStackedWidget *widgetStack;
    ScheduleEditor *scheduleEditor;

signals:
    void needSave(Schedule schedule);
    void needRemove(int id);

public:
    SchedulePresenter(QWidget *parent = nullptr)
        : SchedulePresenter("", parent) { }

    SchedulePresenter(QString icon_path, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        this->icon_path = icon_path;
        setUpUi();
        setUpConnections();
    }

    void showData(QList<Schedule> schedules, QList<Group> groups)
    {
        this->groups = groups;
        this->schedules = schedules;

        updateView(schedules);

        if (widgetStack->currentIndex() == 1)
        {
            if (schedules.contains(scheduleEditor->oldSchedule()))
            {
                scheduleEditor->updateGroups(groups);
            }
            else
            {
                int result = QMessageBox::question(this, " ",
                        "Данное расписание было изменено извне. "
                        "Продолжить редактирование?");
                if (result == QMessageBox::Ok)
                {
                    scheduleEditor->updateGroups(groups);
                }
                else if (result == QMessageBox::No)
                {
                     showSchedules();
                }
            }
        }
    }

private:
    void setUpUi()
    {
        createButton = new QPushButton("+");

        scheduleViewer = new QTableView;
        scheduleViewer->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        QVBoxLayout *viewerLayout = new QVBoxLayout;
        viewerLayout->addWidget(createButton);
        viewerLayout->addWidget(scheduleViewer);

        QWidget *viewerWidget = new QWidget;
        viewerWidget->setLayout(viewerLayout);

        scheduleEditor = new ScheduleEditor;

        widgetStack = new QStackedWidget(this);
        widgetStack->addWidget(viewerWidget);
        widgetStack->addWidget(scheduleEditor);
    }

    void setUpConnections()
    {
        connect(createButton, &QPushButton::clicked, [=] ()
        {
            showEditor();
        });
        connect(scheduleViewer, &QTableView::clicked, [=] (QModelIndex index)
        {
            showEditor(schedules.at(index.row()));
        });

        connect(scheduleEditor, &ScheduleEditor::needSave, [=] (Schedule sch)
        {
            emit needSave(sch);
            showSchedules();
        });
        connect(scheduleEditor, &ScheduleEditor::needRemove, [=] (int id)
        {
            emit needRemove(id);
            showSchedules();
        });
        connect(scheduleEditor, &ScheduleEditor::needExit,
                this, &SchedulePresenter::showSchedules);
    }

    void updateView(QList<Schedule> schedules)
    {
        int columns = Schedule::pattern().count();
        int rows = schedules.count();
        auto stringTable = Schedule::toStringTable(schedules);

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

        model->setHorizontalHeaderLabels(Schedule::pattern());
        scheduleViewer->setModel(model);
    }

private slots:
    void showEditor(Schedule sched = Schedule())
    {
        scheduleEditor->showData(sched, groups);
        widgetStack->setCurrentIndex(1);
    }

    void showSchedules()
    {
        scheduleEditor->showData(Schedule(), {});
        widgetStack->setCurrentIndex(0);
    }

public:
    void resizeEvent(QResizeEvent *resizeEvent)
    {
        widgetStack->resize(resizeEvent->size());
    }
};





















