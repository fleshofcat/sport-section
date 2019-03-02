QT += sql testlib widgets
CONFIG += c++11 testcase

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS +=                          \
    sport_section.h                 \
    common/person.h                 \
    common/group.h                  \
    common/schedule.h               \
                                    \
    tests/main_test.h                   \
    tests/test_db/test_people_storage.h \
    tests/test_db/test_db_manager.h     \
    tests/test_db/test_group_manager.h  \
    tests/test_group_editor.h           \
    tests/test_editors_box.h            \
                                        \
    db/db_manager.h                 \
    db/people_manager.h             \
    db/group_manager.h              \
    db/group_people_relations.h     \
                                    \
    ui/main_window.h                \
    ui/group_editor.h               \
    ui/widgets/record_chooser.h     \
    ui/widgets/editors_box.h        \
    ui/widgets/records_widget.h     \
    ui/widgets/records_viewer.h \
    ui/person_editor.h \
    tests/test_db/test_group_links.h \
    ui/people_presentor.h \
    ui/groups_presentor.h \
    db/schedule_manager.h \
    tests/test_db/test_schedule_manager.h

SOURCES += \
    main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




