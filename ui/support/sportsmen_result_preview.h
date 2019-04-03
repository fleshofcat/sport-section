#pragma once

#include <QListWidget>
#include <QVBoxLayout>

#include <QStandardItemModel>
#include <QHeaderView>
#include <QLabel>

#include "common/person.h"

class SportsmenResultPreview : public QWidget
{
    Q_OBJECT

    QString img_path;

    QList<Person> allSportsmen;

    QLabel *participatingSportsmenLabel;
    QLabel *declaredSportsmenLabel;

    QListWidget *participatingSportsmenView;
    QListWidget *declaredSportsmenView;

signals:
    void sportsmenOrderChanged(QList<Person> sportsmen);

public:
    explicit
    SportsmenResultPreview(QString imagePath,
                           QWidget *parent = nullptr);
    explicit
    SportsmenResultPreview(QWidget *parent = nullptr);

    QList<Person> getRangedSportsmenList();

public slots:
    void setSportsmen(QList<Person> sportsmen);
    void setSportsmenDescription (QString labelText);

    void setSportsmanIconPath(QString imagePath);

private:
    void setUpUi();
    void setUpConnections();

    void sendState();
    void repaintItems();
};




























