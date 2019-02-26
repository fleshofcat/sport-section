#include <QApplication> // системная библиотека

#include "tests/main_test.h" // тесты
#include "sport_section.h"   // главный класс программы (SportSection)
#include "ui/widgets/record_chooser.h"

// точка входа программы
int main(int argc, char *argv[])
{
#if 1 // переключатель режима запуска тестов или программы

    QApplication app(argc, argv);

    SportSection sect;  // сама программа

    return app.exec();
#else
    QApplication app(argc, argv);

    TestEditorsBox tst;
    tst.manualTesting();

//    MainTest::runAllTests(argc, argv);
//    return 0;
    return app.exec();
#endif
}


