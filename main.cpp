#include "mainwindow.h"
#include <QApplication>

#include "tests/main_test.h"

int main(int argc, char *argv[])
{
#if 0 // switch test mode

    QApplication app(argc, argv);
//    MainTest::runAllTests(argc, argv);

    MainWindow mw(nullptr, true);
    mw.show();

    return app.exec();
#else
    QApplication app(argc, argv);

    MainTest::runAllTests(argc, argv);
    return 0;
#endif
}


