#include "mainwindow.h"
#include <QApplication>

#include "tests/main_test.h"

int main(int argc, char *argv[])
{
#if 0 // switch test mode

    QApplication app(argc, argv);

    // TODO make app class

    MainTest::runAllTests(argc, argv);
    return app.exec();
#else

    MainTest::runAllTests(argc, argv);
    return 0;
#endif
}


