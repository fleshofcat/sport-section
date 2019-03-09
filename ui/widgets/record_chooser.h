#pragma once

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>

#include "ui/widgets/records_viewer.h"

class RecordChooser : public QDialog
{
    Q_OBJECT

    RecordsViewer *viewer;
    QPushButton *rejectButton = new QPushButton("Отмена");
    int *result;

public:
    RecordChooser(QList<QList<QString>> stringTable,
                  int *result, QWidget *parent = nullptr,
                  QString title = "", QString rowIconPath = "") : QDialog(parent)
    {
        this->result = result;

        setUpUi(stringTable, title, rowIconPath);
    }

    static int getChoosedRow(QList<QList<QString>> stringTable,
                             QWidget *parent = nullptr,
                             QString windowTitle = "", QString rowIconPath = "")
    {
        int row = -1;

        RecordChooser chooseDialog(stringTable, &row, parent, windowTitle, rowIconPath);

        if (chooseDialog.exec() == QDialog::Accepted)
        {
            return row;
        }

        return -1;
    }

private:
    void setUpUi(QList<QList<QString>> stringTable, QString title, QString rowIconPath)
    {
        setWindowTitle(title);

        viewer = new RecordsViewer(this);
        viewer->setIconPath(rowIconPath);
        viewer->updateContent(stringTable);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(viewer);
        layout->addWidget(rejectButton);
        setLayout(layout);

        connect(rejectButton, &QPushButton::pressed, this, &QDialog::reject);
        connect(viewer, &RecordsViewer::rowIsActivated, this, &RecordChooser::on_tablePressed);
    }

private slots:
    void on_tablePressed(int row)
    {
        *this->result = row;

        accept();
    }

};


















