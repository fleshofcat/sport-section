QT += core gui widgets sql testlib
CONFIG += c++11 testcase

DEFINES += QT_DEPRECATED_WARNINGS

# for work:
HEADERS += \
    sport_section.h                 \
                                    \
    tests/main_test.h               \
    tests/test_people_storage.h     \
    tests/test_db_manager.h         \
    tests/test_schedule_manager.h   \
    tests/test_shortshowwidget.h    \
                                    \
    db/db_manager.h                 \
    db/schedule_manager.h           \
    db/people_manager.h             \
                                    \
    common/common_objects.h         \
                                    \
    ui/widgets/property_editor.h \
    ui/main_window.h \
    ui/person_editor.h \
    ui/widgets/record_viewer.h \
    ui/widgets/records_widget.h \
    ui/people_tab.h

SOURCES += \
    main.cpp

FORMS += \
        mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




