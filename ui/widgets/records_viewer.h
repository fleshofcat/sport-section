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
    explicit RecordsViewer(QWidget *parent = nullptr);
    explicit RecordsViewer(QList<QList<QString>> table,
                           QWidget *parent = nullptr);

    void updateContent(QList<QList<QString>> table,
                       QList<QString> pattern = {});

    void updateContent(QList<QList<QVariant>> table,
                       QList<QString> pattern = {});

    void setHHeader(QList<QString> pattern = {});
    void setIconPath(QString icon_path);

private:
    void setUpUi();
    void dropContent();

    template <class Obj>
    bool isTableValid(QList<QList<Obj>> table);
};

























