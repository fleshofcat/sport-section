#include <QApplication> // системная библиотека

#include "tests/main_test.h" // тесты
#include "sport_section.h"   // главный класс программы (SportSection)

#include "ui/support/schedule_close.h"

// точка входа программы
int main(int argc, char *argv[])
{
    QString img_path = "/home/fleshofcat/Projects/sandbox/record/record/res/img/schedule.png";

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

    auto schedule = db.getSchedules().at(0);

    for (int g = 0; g < schedule.groups.count(); g++)
    {
        for (int s = 0; s < schedule.groups[g].sportsmen.count(); s++)
        {
            schedule.groups[g].sportsmen[s].rating += rand() % 50;
        }

        int additional = rand() % 50;
        schedule.groups[g].increaseTrainersRating(additional);
    }


    ScheduleClose closer(schedule);
    closer.show();

    return app.exec();
#endif
}





























