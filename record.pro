QT += core gui widgets sql testlib
CONFIG += c++11 testcase

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    sport_section.h                 \
    common/common_objects.h         \
                                    \
    tests/main_test.h               \
    tests/test_people_storage.h     \
    tests/test_db_manager.h         \
                                    \
    db/db_manager.h                 \
    db/people_manager.h             \
                                    \
    ui/widgets/property_editor.h    \
    ui/main_window.h                \
    ui/widgets/record_viewer.h      \
    ui/people_tab.h \
    db/group_manager.h \
    tests/test_group_manager.h \
    db/group_people_relations.h \
    ui/groups_tab.h \
    ui/widgets/group_editor.h \
    ui/widgets/record_chooser.h \
    ui/widgets/string_table_widget.h \
    tests/test_group_editor.h \
    ui/widgets/editors_box.h \
    tests/test_editors_box.h

SOURCES += \
    main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




