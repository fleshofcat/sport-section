QT += core gui widgets sql testlib
CONFIG += c++11 testcase

DEFINES += QT_DEPRECATED_WARNINGS

# for work:
HEADERS += \
    db/db_manager.h                 \
                                    \                                    \
    tests/main_test.h               \
    tests/test_people_storage.h     \
    tests/test_db_manager.h         \
    tests/test_schedule_manager.h   \
                                    \
    sport_section.h                 \
    db/schedule_manager.h           \
    db/people_manager.h             \
                                    \
    common/common_objects.h         \
    ui/mainwindow.h                 \
    ui/personeditor.h \
    tests/test_shortshowwidget.h \
    ui/stringlistviewer.h \
    ui/propertyeditor.h

SOURCES += \
    main.cpp

FORMS += \
        mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




