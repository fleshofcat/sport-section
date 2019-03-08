#pragma once

#include <QListWidget>
#include <QVBoxLayout>

#include <QStandardItemModel>
#include <QHeaderView>
#include <QLabel>

#include "common/person.h"

class SportsmenViewForEventResult : public QWidget
{
    Q_OBJECT

    QString img_path;

    QList<Person> allSportsmen;

    QLabel *rangeLabel;
    QLabel *outersLabel;

    QListWidget *rangeView;
    QListWidget *outersView;

signals:
    void sportsmenOrderChanged(QList<Person> sportsmen);

public:
    SportsmenViewForEventResult(QString imagePath, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        this->img_path = imagePath;
        setUpUi();
        setUpConnections();
    }

    SportsmenViewForEventResult(QWidget *parent = nullptr)
        : SportsmenViewForEventResult("", parent) { }

    QList<Person> getRangePeopleList()
    {
        QList<Person> rangetSportsmen;

        for (int r = 0; r < rangeView->model()->rowCount(); r++)
        {
            QModelIndex index = rangeView->model()->index(r, 0);
            int id = rangeView->model()->data(index, Qt::UserRole).toInt();

            for (Person pers : allSportsmen)
            {
                if (pers.id == id)
                {
                    rangetSportsmen << pers;
                }
            }
        }

        return rangetSportsmen;
    }

public slots:
    void setSportsmen(QList<Person> sportsmen)
    {
        rangeView->clear();
        outersView->clear();

        this->allSportsmen = sportsmen;


        for (int r = 0; r < sportsmen.count(); r++)
        {
            QString stringPers = QString::number(r+1)
                    + " " + sportsmen.at(r).firstName
                    + " " + sportsmen.at(r).secondName
                    + " " + sportsmen.at(r).lastName;

            auto item = new QListWidgetItem(QIcon(img_path), stringPers, outersView);
            item->setData(Qt::UserRole, QVariant(sportsmen[r].id));

            outersView->addItem(item);
        }
    }

    void setDescribeLabel(QString labelText)
    {
        rangeLabel->setText(labelText);
    }

    void setPersIconPath(QString imagePath)
    {
        this->img_path = imagePath;
        repaintItems();
    }

private:
    void setUpUi()
    {
        rangeLabel = new QLabel;
        outersLabel = new QLabel("Кликните на спортсмена чтобы его отметить");

        rangeView = new QListWidget;
        rangeView->setDragDropMode(QListWidget::DragDropMode::InternalMove);

        outersView = new QListWidget;
        outersView->setDragDropMode(QListWidget::DragDropMode::InternalMove);

        QVBoxLayout *lay = new QVBoxLayout;
        lay->addWidget(rangeLabel);
        lay->addWidget(rangeView);
        lay->addWidget(outersLabel);
        lay->addWidget(outersView);

        setLayout(lay);
    }

    void setUpConnections()
    {
        connect(outersView, &QListWidget::clicked, [=] (QModelIndex index)
        {
            auto item = outersView->item(index.row());

            rangeView->addItem(new QListWidgetItem(*item));
            delete outersView->takeItem(index.row());

            repaintItems();
            sendState();
        });
        connect(rangeView, &QListWidget::clicked, [=] (QModelIndex index)
        {
            auto item = rangeView->item(index.row());

            outersView->addItem(new QListWidgetItem(*item));
            delete rangeView->takeItem(index.row());

            repaintItems();
            sendState();
        });

        connect(rangeView->model(), &QAbstractItemModel::rowsMoved, [=] ()
        {
            repaintItems();
            sendState();
        });

        connect(outersView->model(), &QAbstractItemModel::rowsMoved, [=] ()
        {
            repaintItems();
        });
    }

    void sendState()
    {
        auto orderedSportsmen = getRangePeopleList();
        emit sportsmenOrderChanged(orderedSportsmen);
    }

    void repaintItems()
    {
        for (int r = 0; r < rangeView->model()->rowCount(); r++)
        {
            QModelIndex index = rangeView->model()->index(r, 0);
            QString pos = QString::number(r+1);

            QString str = rangeView->model()->data(index).toString();
            str.replace(0, str.indexOf(" "), pos);

            rangeView->model()->setData(index, QIcon(img_path), Qt::DecorationRole);
            rangeView->model()->setData(index, str, Qt::DisplayRole);
        }

        for (int r = 0; r < outersView->model()->rowCount(); r++)
        {
            QModelIndex index = outersView->model()->index(r, 0);
            QString pos = QString::number(r+1);

            QString str = outersView->model()->data(index).toString();
            str.replace(0, str.indexOf(" "), pos);

            outersView->model()->setData(index, QIcon(img_path), Qt::DecorationRole);
            outersView->model()->setData(index, str, Qt::DisplayRole);
        }
    }


};




























