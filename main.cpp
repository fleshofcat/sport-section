#include <QApplication> // системная библиотека

#include "tests/main_test.h" // тесты
#include "sport_section.h"   // главный класс программы (SportSection)

// точка входа программы
int main(int argc, char *argv[])
{
#if 1 // включить режим запуска программы

    QApplication app(argc, argv);

    SportSection sect;  // сама программа

    return app.exec();
#elif 1 // режим запуска тестов
    QApplication app(argc, argv);

    MainTest::runAllTests(argc, argv);
    return 0;

#else // ручное тестирование
    QApplication app(argc, argv);

    return app.exec();
#endif
}





























