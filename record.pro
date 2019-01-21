QT += quick sql testlib #TODO
CONFIG += c++11 testcase #TODO
DEFINES += QT_DEPRECATED_WARNINGS

# for work:
HEADERS += \
    person.h \
    db_manager.h \
    relation.h \
    tests/main_test.h \
    people_storage.h \
    relation_storage.h \
    tests/test_people_storage.h \
    tests/test_relation_storage.h

SOURCES += \
    main.cpp



RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




