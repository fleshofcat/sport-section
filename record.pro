QT += quick testlib sql
CONFIG += c++11 testcase
DEFINES += QT_DEPRECATED_WARNINGS

# for work:
HEADERS += \
    src/common/person.h \
    src/db/db_manager.h

SOURCES += \
    src/main.cpp \
    src/db/db_manager.cpp

# for test:
HEADERS += \
    tests/main_test.h \
    tests/test_dbmanager.h

SOURCES += \
    tests/test_dbmanager.cpp \
    tests/main_test.cpp


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



