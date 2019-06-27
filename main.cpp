#include <QApplication>

#include "tests/main_test.h"
#include "sport_section.h"

int main(int argc, char *argv[])
{
#if 1 // run mode

    QApplication app(argc, argv);
    SportSection sect;

    return app.exec();
#elif 1 // test mode
    QApplication app(argc, argv);

    MainTest::runAllTests(argc, argv);
    return 0;

#else // manual test mode
    QApplication app(argc, argv);

    return app.exec();
#endif
}





























