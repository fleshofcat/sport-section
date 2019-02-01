QT += core gui widgets sql testlib
CONFIG += c++11 testcase

DEFINES += QT_DEPRECATED_WARNINGS

# for work:
HEADERS += \
    person.h                        \
    db_manager.h                    \
    people_storage.h                \
    relation_storage.h              \
                                    \                                    \
    tests/main_test.h               \
    tests/test_people_storage.h     \
    tests/test_relation_storage.h   \
    tests/test_db_manager.h         \
                                    \
    mainwindow.h \
    tests/test_mainwindow.h \
    sport_section.h

SOURCES += \
    main.cpp

FORMS += \
        mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




