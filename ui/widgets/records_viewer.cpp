#include "ui/widgets/records_viewer.h"

RecordsViewer::
RecordsViewer(QWidget *parent)
    : RecordsViewer(QList<QList<QString>>(), parent) { }

RecordsViewer::
RecordsViewer(QList<QList<QString>> table, QWidget *parent)
    : QTableView(parent)
{
    setUpUi();
    updateContent(table);

    connect(this, &QTableView::clicked, [=] (QModelIndex index) { emit rowIsActivated(index.row()); });
}

void RecordsViewer::
setIconPath(QString icon_path)
{
    this->icon_path = icon_path;
}

void RecordsViewer::
updateContent(QList<QList<QString>> table, QList<QString> pattern)
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

void RecordsViewer::
updateContent(QList<QList<QVariant>> table, QList<QString> pattern)
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

void RecordsViewer::
setHHeader(QList<QString> pattern)
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

void RecordsViewer::
setUpUi()
{
    horizontalHeader()->hide();
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

template <class Obj>
bool RecordsViewer::
isTableValid(QList<QList<Obj>> table)
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

void RecordsViewer::
dropContent()
{
    QStandardItemModel *emptyModel = new QStandardItemModel;
    setModel(emptyModel);
}













