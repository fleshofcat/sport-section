#include <QApplication> // системная библиотека

#include "tests/main_test.h" // тесты
#include "sport_section.h"   // главный класс программы (SportSection)

// точка входа программы
int main(int argc, char *argv[])
{
#if 0 // переключатель режима запуска тестов или программы

    QApplication app(argc, argv);

    SportSection sect;  // сама программа

    return app.exec();
#else
    QApplication app(argc, argv);

    MainTest::runAllTests(argc, argv);
    return 0;
//    return app.exec();
#endif
}


