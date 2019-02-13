#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QHeaderView>

class StringTableViewer : public QWidget
{
Q_OBJECT

    QTableWidget *table = nullptr;
    QList<QString> pattern;
    QList<QList<QString>> stringTable;

signals:
    void createRecordIsRequred(QList<QString> pattern);
    void editRecordIsRequre(QList<QString> pattern, QList<QString> record);

public:
    StringTableViewer(QList<QString> pattern,
                      QList<QList<QString>> stringTable,
                      QWidget *parent = nullptr)
        : QWidget (parent)
    {
        table = new QTableWidget(this);

        this->pattern = pattern;
        this->stringTable = stringTable;

        update(pattern, stringTable);

        connect(table, &QTableWidget::cellPressed,
                this, &StringTableViewer::on_tablePressed);

        setUpUi();
    }

    void update(QList<QString> pattern,
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

        table->setSpan(table->rowCount() - 1, 0, 1, table->columnCount());

        table->setItem(table->rowCount() - 1, 0,
                                   new QTableWidgetItem("+"));
        table->item(table->rowCount() - 1, 0)->setTextAlignment(Qt::AlignCenter);
    }

private:
    void setUpUi()
    {
        table->horizontalHeader()->setVisible(true);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        table->verticalHeader()->setVisible(false);

        table->resize(size());
        show();
    }


private slots:
    void on_tablePressed(int row, int)
    {
        if (table->rowCount() == row + 1)
        {
            emit createRecordIsRequred(pattern);
        }
        else
        {
            emit editRecordIsRequre(pattern, stringTable.at(row));
        }
    }
};




























