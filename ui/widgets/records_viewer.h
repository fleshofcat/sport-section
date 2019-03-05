#pragma once

#include <QHeaderView>
#include <QStringListModel>
#include <QTableWidget>
#include <QStandardItemModel>

class RecordsViewer : public QTableView
{
    Q_OBJECT

    QString icon_path;

signals:
    void rowIsActivated(int row);

public:
    RecordsViewer(QWidget *parent = nullptr)
        : RecordsViewer(QList<QList<QString>>(), parent) { }

    RecordsViewer(QList<QList<QString>> table, QWidget *parent = nullptr)
        : QTableView(parent)
    {
        setUpUi();
        updateContent(table);
    }

    void setIconPath(QString icon_path)
    {
        this->icon_path = icon_path;
    }

    void updateContent(QList<QList<QString>> table, QList<QString> pattern = {})
    {
        dropContent();

        if (isTableValid(table))
        {
            int columns = table.at(0).count();
            int rows = table.count();

            QStandardItemModel *model = new QStandardItemModel(rows, columns);

            for (int r = 0; r < rows; r++)
            {
                for (int c = 0; c < columns; c++)
                {

                    QModelIndex index = model->index(r, c);
                    model->setData(index, table.at(r).at(c));
                }

                if (!icon_path.isEmpty())
                {
                    model->setVerticalHeaderItem(
                                r, new QStandardItem(QIcon(icon_path), ""));
                }
            }
            setModel(model);
            setHHeader(pattern);
        }
    }

    void setHHeader(QList<QString> pattern = {})
    {
        if (!pattern.isEmpty())
        {
            for (int c = 0; c < pattern.count(); c++)
            {
                model()->setHeaderData(c, Qt::Horizontal, pattern.at(c));
            }
        }

        horizontalHeader()->setVisible(!pattern.isEmpty());
    }

private:
    void setUpUi()
    {
        horizontalHeader()->hide();
        horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        connect(this, &QTableView::clicked, [=] (QModelIndex index) { emit rowIsActivated(index.row()); });
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
        QStandardItemModel *emptyModel = new QStandardItemModel;
        setModel(emptyModel);
    }
};







//class RecordsViewer : public QTableWidget
//{
//    Q_OBJECT

//    QString icon_path;

//signals:
//    void rowIsActivated(int row);

//public:
//    RecordsViewer(QWidget *parent = nullptr)
//        : QTableWidget(parent)
//    {
//        setUpUi();
//    }

//    RecordsViewer(QList<QList<QString>> &table, QWidget *parent = nullptr)
//        : QTableWidget(parent)
//    {
//        setUpUi();
//        updateContent(table);
//    }

//    void setIconPath(QString icon_path)
//    {
//        this->icon_path = icon_path;
//    }

//    void updateContent(QList<QList<QString>> &table)
//    {
//        dropContent();

//        if (isTableValid(table))
//        {
//            setRowCount(table.count());
//            setColumnCount(table.at(0).count());

//            for (int r = 0; r < table.count(); r++)
//            {
//                for (int c = 0; c < table.at(0).count(); c++)
//                {
//                    this->setItem(r, c, new QTableWidgetItem(
//                                      table.at(r).at(c)));
//                }
//            }
//        }
//    }

//    void setHHeader(QList<QString> pattern = {})
//    {
//        if (!pattern.isEmpty())
//        {
//            for (int c = 0; c < pattern.count(); c++)
//            {
//                this->setHorizontalHeaderItem(
//                            c, new QTableWidgetItem(pattern.at(c)));
//            }
//            horizontalHeader()->setVisible(true);
//        }
//        else
//        {
//           int count = horizontalHeader()->count();

//           while (count)
//           {
//                count--;
//                this->setHorizontalHeaderItem(count, new QTableWidgetItem(""));
//           }

//           horizontalHeader()->setVisible(false);
//        }
//    }

//private:
//    void setUpUi()
//    {
//        horizontalHeader()->hide();
//        horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

//        connect(this, &QTableWidget::cellClicked, this, &RecordsViewer::on_itemClicked);
//    }

//    bool isTableValid(QList<QList<QString>> &table)
//    {
//        if (table.count() <= 0)
//            return false;


//        int length = table.at(0).count();

//        for (auto list : table)
//        {
//            if (list.count() != length)
//            {
//                return false;
//            }
//        }
//        return true;
//    }

//    void dropContent()
//    {
//        clear();
//        setRowCount(0);
//        setColumnCount(0);
//    }

//private slots:
//    void on_itemClicked(int row, int)
//    {
//        emit rowIsActivated(row);
//    }
//};
























