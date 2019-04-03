#include "ui/support/sportsmen_result_preview.h"

SportsmenResultPreview::
SportsmenResultPreview(QString imagePath, QWidget *parent)
    : QWidget(parent)
{
    this->img_path = imagePath;
    setUpUi();
    setUpConnections();
}

SportsmenResultPreview::
SportsmenResultPreview(QWidget *parent)
    : SportsmenResultPreview("", parent) { }

QList<Person> SportsmenResultPreview::
getRangedSportsmenList()
{
    QList<Person> rangedSportsmen;

    for (int r = 0; r < participatingSportsmenView->model()->rowCount(); r++)
    {
        QModelIndex index = participatingSportsmenView->model()->index(r, 0);
        int id = participatingSportsmenView->model()->data(index, Qt::UserRole).toInt();

        for (Person pers : allSportsmen)
        {
            if (pers.id == id)
            {
                rangedSportsmen << pers;
            }
        }
    }

    return rangedSportsmen;
}

void SportsmenResultPreview::
setSportsmen(QList<Person> sportsmen)
{
    participatingSportsmenView->clear();
    declaredSportsmenView->clear();

    this->allSportsmen = sportsmen;

    for (int r = 0; r < sportsmen.count(); r++)
    {
        QString stringPers = QString::number(r+1)
                + " " + sportsmen[r].getFirstName()
                + " " + sportsmen[r].getSecondName()
                + " " + sportsmen[r].getLastName();

        auto item = new QListWidgetItem(QIcon(img_path), stringPers, declaredSportsmenView);
        item->setData(Qt::UserRole, QVariant(sportsmen[r].id));

        declaredSportsmenView->addItem(item);
    }
}

void SportsmenResultPreview::
setSportsmenDescription (QString labelText)
{
    participatingSportsmenLabel->setText(labelText);
}

void SportsmenResultPreview::
setSportsmanIconPath(QString imagePath)
{
    this->img_path = imagePath;
    repaintItems();
}

void SportsmenResultPreview::
setUpUi()
{
    participatingSportsmenLabel = new QLabel;
    declaredSportsmenLabel = new QLabel("Кликните на спортсмена чтобы его отметить");

    participatingSportsmenView = new QListWidget;
    participatingSportsmenView->setDragDropMode(QListWidget::DragDropMode::InternalMove);

    declaredSportsmenView = new QListWidget;
    declaredSportsmenView->setDragDropMode(QListWidget::DragDropMode::InternalMove);

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(participatingSportsmenLabel);
    lay->addWidget(participatingSportsmenView);
    lay->addWidget(declaredSportsmenLabel);
    lay->addWidget(declaredSportsmenView);

    setLayout(lay);
}

void SportsmenResultPreview::
setUpConnections()
{
    connect(declaredSportsmenView, &QListWidget::clicked, [=] (QModelIndex index)
    {
        auto item = declaredSportsmenView->item(index.row());

        participatingSportsmenView->addItem(new QListWidgetItem(*item));
        delete declaredSportsmenView->takeItem(index.row());

        repaintItems();
        sendState();
    });
    connect(participatingSportsmenView, &QListWidget::clicked, [=] (QModelIndex index)
    {
        auto item = participatingSportsmenView->item(index.row());

        declaredSportsmenView->addItem(new QListWidgetItem(*item));
        delete participatingSportsmenView->takeItem(index.row());

        repaintItems();
        sendState();
    });

    connect(participatingSportsmenView->model(), &QAbstractItemModel::rowsMoved, [=] ()
    {
        repaintItems();
        sendState();
    });

    connect(declaredSportsmenView->model(), &QAbstractItemModel::rowsMoved, [=] ()
    {
        repaintItems();
    });
}

void SportsmenResultPreview::
sendState()
{
    QList<Person> orderedSportsmen = getRangedSportsmenList();
    emit sportsmenOrderChanged(orderedSportsmen);
}

void SportsmenResultPreview::
repaintItems()
{
    for (int r = 0; r < participatingSportsmenView->model()->rowCount(); r++)
    {
        QModelIndex index = participatingSportsmenView->model()->index(r, 0);
        QString pos = QString::number(r+1);

        QString str = participatingSportsmenView->model()->data(index).toString();
        str.replace(0, str.indexOf(" "), pos);

        participatingSportsmenView->model()->setData(index, QIcon(img_path), Qt::DecorationRole);
        participatingSportsmenView->model()->setData(index, str, Qt::DisplayRole);
    }

    for (int r = 0; r < declaredSportsmenView->model()->rowCount(); r++)
    {
        QModelIndex index = declaredSportsmenView->model()->index(r, 0);
        QString pos = QString::number(r+1);

        QString str = declaredSportsmenView->model()->data(index).toString();
        str.replace(0, str.indexOf(" "), pos);

        declaredSportsmenView->model()->setData(index, QIcon(img_path), Qt::DecorationRole);
        declaredSportsmenView->model()->setData(index, str, Qt::DisplayRole);
    }
}




































