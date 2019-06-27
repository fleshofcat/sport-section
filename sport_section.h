#pragma once

#include "ui/main_window.h"
#include "db/db_manager.h"

class SportSection : public QObject
{
    Q_OBJECT

    Ui ui;
    DbManager db;

public:
    explicit SportSection(QObject *parent = nullptr);

private:
    void setUpConnections();
    void updateMainWindow();
};



























