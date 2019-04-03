#include "ui/widgets/record_chooser.h"

RecordChooser::
RecordChooser(QList<QList<QString>> stringTable,
              int *result, QWidget *parent,
              QString title, QString rowIconPath)
    : QDialog(parent)
{
    this->result = result;

    setUpUi(stringTable, title, rowIconPath);
}

int RecordChooser::
getChoosedRow(QList<QList<QString>> stringTable,
              QWidget *parent,
              QString windowTitle, QString rowIconPath)
{
    int row = -1;

    RecordChooser chooseDialog(stringTable, &row, parent, windowTitle, rowIconPath);

    if (chooseDialog.exec() == QDialog::Accepted)
    {
        return row;
    }

    return -1;
}

void RecordChooser::
setUpUi(QList<QList<QString>> stringTable, QString title, QString rowIconPath)
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

void RecordChooser::
on_tablePressed(int row)
{
    *this->result = row;

    accept();
}



























