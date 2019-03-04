#pragma once

#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QStringListModel>

#include "ui/widgets/records_viewer.h"

class ListView : public QWidget
{
Q_OBJECT

    QListWidget *view;
    QPushButton *addButton;

    QList<QString> pattern;
    QString icon_path = "../record/res/img/trainer.png";

signals:
    void createRecordActivated();
    void recordActivated(int row);

public:
    ListView(QList<QString> pattern,
                      QList<QList<QString>> stringTable,
                      QWidget *parent = nullptr)
        : QWidget (parent)
    {
        setUpUi();
        setUpConnections();

        updateData(pattern, stringTable);
    }

    ListView(QWidget *parent = nullptr)
        : QWidget (parent)
    {
        setUpUi();
        setUpConnections();
    }

    void updateData(QList<QString> pattern,
                QList<QList<QString>> stringTable)
    {
        this->pattern = pattern;

        for (int r = 0; r < stringTable.count(); r++)
        {
            QString str = QString("%1 %2 %3")
                    .arg(stringTable[r].at(0))
                    .arg(stringTable[r].at(1))
                    .arg(stringTable[r].at(2));

            QListWidgetItem *item = new QListWidgetItem(str, view);
            item->setIcon(QPixmap(icon_path));
        }
//        QStringListModel *model = new QStringListModel(qqq);
    }

private:
    void setUpConnections()
    {
        connect(view, &QListView::activated, [=] (QModelIndex index)
                                { emit recordActivated(index.row()); });
        connect(addButton, &QPushButton::pressed, this, &ListView::createRecordActivated);
    }

    void setUpUi()
    {
        addButton = new QPushButton("+");
        view = new QListWidget;
        view->setIconSize(QSize(20, 20));
//        view->setFont(QFont("", 22));

        QVBoxLayout *verticalLayout = new QVBoxLayout;
        verticalLayout->addWidget(addButton);
        verticalLayout->addWidget(view);

        setLayout(verticalLayout);
    }

};




























