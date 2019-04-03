#pragma once

#include "ui/main_window.h" // файл графического интерфейса
#include "db/db_manager.h" // файл модуля бд

// класс SportSection/СпортивнаяСекция
//
// главный класс приложения, осущесствляющий логику
// связывания остальных элементов
class SportSection : public QObject
{
    Q_OBJECT

    Ui ui;
    DbManager db;

public:
    // конструктор, выполняющийся при создании объекта
    SportSection(QObject *parent = nullptr);

private:
    void setUpConnections();
    // обновление главного окна пользовательского интерфейса
    void updateMainWindow();
};



























