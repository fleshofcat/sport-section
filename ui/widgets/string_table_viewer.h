#pragma once

#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>

class StringTableViewer : public QWidget
{
Q_OBJECT

    QTableWidget *table = nullptr;
    QPushButton *addButton = nullptr;

signals:
    void createRecordIsRequred();
    void editRecordIsRequred(int row);

public:
    StringTableViewer(QList<QString> pattern,
                      QList<QList<QString>> stringTable,
                      QWidget *parent = nullptr)
        : QWidget (parent)
    {
        setUpUi();
        setUpConnections();

        updateData(pattern, stringTable);
    }

    StringTableViewer(QWidget *parent = nullptr)
        : QWidget (parent)
    {
        setUpUi();
        setUpConnections();
    }

    void updateData(QList<QString> pattern,
                QList<QList<QString>> stringTable)
    {
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
        connect(table, &QTableWidget::cellPressed, this, &StringTableViewer::on_tablePressed);
        connect(addButton, &QPushButton::pressed, this, &StringTableViewer::createRecordIsRequred);
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
};




























