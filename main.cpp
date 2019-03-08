#include <QApplication> // системная библиотека

#include "tests/main_test.h" // тесты
#include "sport_section.h"   // главный класс программы (SportSection)

#include "ui/support/schedule_close.h"

// точка входа программы
int main(int argc, char *argv[])
{
    QString img_path = "/home/fleshofcat/Projects/sandbox/record/record/res/img/schedule.png";

#if 0 // переключатель режима запуска тестов или программы

    QApplication app(argc, argv);

    SportSection sect;  // сама программа

    return app.exec();
#elif 0 // auto tests
    QApplication app(argc, argv);

    MainTest::runAllTests(argc, argv);
    return 0;

#else // manually test
    QApplication app(argc, argv);

//    DbManager db("../record/res/sport_people.db");

//    ScheduleClose closer(db.getSchedules().at(0));
//    closer.show();

    PreviewResultCalculation resultPreview;
    resultPreview.show();

    DbManager db("../record/res/sport_people.db");

    resultPreview.setGroupIconPath("../record/res/img/group.png");
    resultPreview.setTrainerIconPath("../record/res/img/trainer.png");
    resultPreview.setSportsmanIconPath("../record/res/img/sportsman.png");
    resultPreview.setGroups(db.getGroups());

    return app.exec();
#endif
}





























