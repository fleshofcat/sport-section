#pragma once

#include <QVBoxLayout>
#include <QPushButton>

#include "ui/widgets/records_viewer.h"

class RecordsWidget : public QWidget
{
Q_OBJECT

    RecordsViewer *table;
    QPushButton *addButton;

    QList<QString> pattern;

signals:
    void createRecordIsRequred();
    void editRecordIsRequred(int row);

public:
    RecordsWidget(QList<QString> pattern,
                      QList<QList<QString>> stringTable,
                      QWidget *parent = nullptr)
        : QWidget (parent)
    {
        setUpUi();
        setUpConnections();

        updateData(pattern, stringTable);
    }

    RecordsWidget(QWidget *parent = nullptr)
        : QWidget (parent)
    {
        setUpUi();
        setUpConnections();
    }

    void updateData(QList<QString> pattern,
                QList<QList<QString>> stringTable)
    {
        this->pattern = pattern;

        table->updateContent(stringTable);
        table->setHHeader(pattern);
    }

private:
    void setUpConnections()
    {
        connect(table, &QTableWidget::cellPressed, this, &RecordsWidget::on_tablePressed);
        connect(addButton, &QPushButton::pressed, this, &RecordsWidget::on_createButton);
    }

    void setUpUi()
    {
        addButton = new QPushButton("+");
        table = new RecordsViewer(this);

        QVBoxLayout *verticalLayout = new QVBoxLayout;
        verticalLayout->addWidget(addButton);
        verticalLayout->addWidget(table);

        setLayout(verticalLayout);
    }


private slots:
    void on_tablePressed(int row, int)
    {
        emit editRecordIsRequred(row);
    }

    void on_createButton()
    {
        emit createRecordIsRequred();
    }
};




























