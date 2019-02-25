#pragma once

#include <QVBoxLayout>
#include <QPushButton>

#include "ui/widgets/string_table_widget.h"

class RecordsViewer : public QWidget
{
Q_OBJECT

    StringTableWidget *table = nullptr;
    QPushButton *addButton = nullptr;

    QList<QString> pattern;

signals:
    void createRecordIsRequred();
    void editRecordIsRequred(int row);

public:
    RecordsViewer(QList<QString> pattern,
                      QList<QList<QString>> stringTable,
                      QWidget *parent = nullptr)
        : QWidget (parent)
    {
        setUpUi();
        setUpConnections();

        updateData(pattern, stringTable);
    }

    RecordsViewer(QWidget *parent = nullptr)
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
        connect(table, &QTableWidget::cellPressed, this, &RecordsViewer::on_tablePressed);
        connect(addButton, &QPushButton::pressed, this, &RecordsViewer::on_createButton);
    }

    void setUpUi()
    {
        addButton = new QPushButton("+");
        table = new StringTableWidget();

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




























