#include "mainwindow.h"
#include <QApplication>

#include "tests/main_test.h"
#include "sport_section.h"

int main(int argc, char *argv[])
{
#if 1 // switch test mode

    QApplication app(argc, argv);
//    MainTest::runAllTests(argc, argv);

    SportSection sect;

    return app.exec();
#else
    QApplication app(argc, argv);

    MainTest::runAllTests(argc, argv);
    return 0;
#endif
}


