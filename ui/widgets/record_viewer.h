#pragma once

#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>

class RecordsViewer : public QWidget
{
Q_OBJECT

    QTableWidget *table = nullptr;
    QPushButton *addButton = nullptr;

    QList<QString> pattern;

signals:
    void createRecordIsRequred(QList<QString> pattern);
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

        table->clear();

        table->setRowCount(0);
        table->setColumnCount(0);

        table->setRowCount(stringTable.count());
        table->setColumnCount(pattern.count());


        for (int c = 0; c < pattern.count(); c++)
        {
            table->setHorizontalHeaderItem(
                        c, new QTableWidgetItem(pattern.at(c)));
        }


        for (int r = 0; r < stringTable.count(); r++)
        {
            for (int c = 0; c < pattern.count(); c++)
            {
                table->setItem(r, c, new QTableWidgetItem(
                                   stringTable.at(r).at(c)));
            }
        }
    }

private:
    void setUpConnections()
    {
        connect(table, &QTableWidget::cellPressed, this, &RecordsViewer::on_tablePressed);
        connect(addButton, &QPushButton::pressed, this, &RecordsViewer::on_createButton);
    }

    void setUpUi()
    {
        // устанавливается режим показа таблиц
        // они будет показывать элементы в режиме авторастягивания

        addButton = new QPushButton("+");
        table = new QTableWidget();

        QVBoxLayout *verticalLayout = new QVBoxLayout;
        verticalLayout->addWidget(addButton);
        verticalLayout->addWidget(table);

        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        setLayout(verticalLayout);
        show();
    }


private slots:
    void on_tablePressed(int row, int)
    {
        emit editRecordIsRequred(row);
    }

    void on_createButton()
    {
        emit createRecordIsRequred(this->pattern);
    }
};




























