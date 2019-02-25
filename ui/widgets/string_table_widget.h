#pragma once

#include <QHeaderView>
#include <QTableWidget>

class StringTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    StringTableWidget(QWidget *parent = nullptr)
        : QTableWidget(parent)
    {
        setUpUi();
    }

    StringTableWidget(QList<QList<QString>> &table, QWidget *parent = nullptr)
        : QTableWidget(parent)
    {
        setUpUi();
        updateContent(table);
    }


    void updateContent(QList<QList<QString>> &table)
    {
        if (isStringTableValid(table))
        {
            dropTable();

            if (table.count() == 0)
            {
                return;
            }

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

    void setHHeader(QList<QString> header = {})
    {
        if (!header.isEmpty())
        {
            for (int c = 0; c < header.count(); c++)
            {
                setHorizontalHeaderItem(
                            c, new QTableWidgetItem(header.at(c)));
            }
            horizontalHeader()->setVisible(true);
        }
        else
        {
           int count = horizontalHeader()->count();

           while (count)
           {
                count--;
                setHorizontalHeaderItem(count, new QTableWidgetItem(""));
           }

           horizontalHeader()->setVisible(false);
        }
    }

private:
    void setUpUi()
    {
        horizontalHeader()->hide();
        horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }

    bool isStringTableValid(QList<QList<QString>> &table)
    {
        if (table.count() > 0)
        {
            int length = table.at(0).count();

            for (auto list : table)
            {
                if (list.count() != length)
                {
                    return false;
                }
            }
        }
        return true;
    }

    void dropTable()
    {
        clear();
        setRowCount(0);
        setColumnCount(0);
    }

};
























