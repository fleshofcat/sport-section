#pragma once

#include <QHeaderView>
#include <QTableWidget>

class RecordsViewer : public QTableWidget
{
    Q_OBJECT

signals:
    void rowIsActivated(int row);

public:
    RecordsViewer(QWidget *parent = nullptr)
        : QTableWidget(parent)
    {
        setUpUi();
    }

    RecordsViewer(QList<QList<QString>> &table, QWidget *parent = nullptr)
        : QTableWidget(parent)
    {
        setUpUi();
        updateContent(table);
    }


    void updateContent(QList<QList<QString>> &table)
    {
        dropContent();

        if (isTableValid(table))
        {
            setRowCount(table.count());
            setColumnCount(table.at(0).count());

            for (int r = 0; r < table.count(); r++)
            {
                for (int c = 0; c < table.at(0).count(); c++)
                {
                    this->setItem(r, c, new QTableWidgetItem(
                                      table.at(r).at(c)));
                }
            }
        }
    }

    void setHHeader(QList<QString> pattern = {})
    {
        if (!pattern.isEmpty())
        {
            for (int c = 0; c < pattern.count(); c++)
            {
                this->setHorizontalHeaderItem(
                            c, new QTableWidgetItem(pattern.at(c)));
            }
            horizontalHeader()->setVisible(true);
        }
        else
        {
           int count = horizontalHeader()->count();

           while (count)
           {
                count--;
                this->setHorizontalHeaderItem(count, new QTableWidgetItem(""));
           }

           horizontalHeader()->setVisible(false);
        }
    }

private:
    void setUpUi()
    {
        horizontalHeader()->hide();
        horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        connect(this, &QTableWidget::cellClicked, this, &RecordsViewer::on_itemClicked);
    }

    bool isTableValid(QList<QList<QString>> &table)
    {
        if (table.count() <= 0)
            return false;


        int length = table.at(0).count();

        for (auto list : table)
        {
            if (list.count() != length)
            {
                return false;
            }
        }
        return true;
    }

    void dropContent()
    {
        clear();
        setRowCount(0);
        setColumnCount(0);
    }

private slots:
    void on_itemClicked(int row, int)
    {
        emit rowIsActivated(row);
    }
};
























