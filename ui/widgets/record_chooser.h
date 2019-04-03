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
    explicit
    RecordChooser(QList<QList<QString>> stringTable,
                  int *result, QWidget *parent = nullptr,
                  QString title = "", QString rowIconPath = "");
    static
    int getChoosedRow(QList<QList<QString>> stringTable,
                      QWidget *parent = nullptr,
                      QString windowTitle = "",
                      QString rowIconPath = "");
private:
    void setUpUi(QList<QList<QString>> stringTable,
                 QString title, QString rowIconPath);
private slots:
    void on_tablePressed(int row);
};


















