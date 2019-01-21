#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "tests/main_test.h"

int main(int argc, char *argv[])
{
#if 0 // switch test mode
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    MainTest::runAllTests(argc, argv);
    return app.exec();
#else

    MainTest::runAllTests(argc, argv);
    return 0;
#endif
}


