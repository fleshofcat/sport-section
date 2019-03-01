#pragma once

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>

#include "ui/widgets/records_viewer.h"

class RecordChooser : public QDialog
{
    Q_OBJECT

    RecordsViewer *table;
    QPushButton *rejectButton = new QPushButton("Отмена");
    int *result;

public:
    RecordChooser(QList<QList<QString>> stringTable, int *result,
                  QWidget *parent = nullptr) : QDialog(parent)
    {
        this->result = result;

        setUpUi(stringTable);
    }

    static int getChoosedRow(QList<QList<QString>> stringTable,
                             QWidget *parent = nullptr)
    {
        int row = -1;
        RecordChooser chooseDialog(stringTable, &row, parent);

        if (chooseDialog.exec() == QDialog::Accepted)
        {
            return row;
        }

        return -1;
    }

private:
    void setUpUi(QList<QList<QString>> stringTable)
    {
        table = new RecordsViewer(stringTable, this);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(table);
        layout->addWidget(rejectButton);
        setLayout(layout);


        connect(rejectButton, &QPushButton::pressed, this, &QDialog::reject);
        connect(table, &RecordsViewer::rowIsActivated, this, &RecordChooser::on_tablePressed);
//        connect(table, &QTableWidget::cellPressed, this, &RecordChooser::on_tablePressed);
    }

private slots:
    void on_tablePressed(int row)
    {
        *this->result = row;

        accept();
    }

};


















