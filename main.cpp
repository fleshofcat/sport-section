#include <QApplication> // системная библиотека

#include "tests/main_test.h" // тесты
#include "sport_section.h"   // главный класс программы (SportSection)

#include "ui/schedule_result.h"

// точка входа программы
int main(int argc, char *argv[])
{
#if 1 // переключатель режима запуска тестов или программы

    QApplication app(argc, argv);

    SportSection sect;  // сама программа

    return app.exec();
#elif 1 // auto tests
    QApplication app(argc, argv);

    MainTest::runAllTests(argc, argv);
    return 0;

#else // manually test
    QApplication app(argc, argv);

    DbManager db("../record/res/sport_people.db");

//    Sch

    return app.exec();
#endif
}





























