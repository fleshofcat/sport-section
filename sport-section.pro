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
    tests/test_ui/test_group_editor.h   \
    tests/test_ui/test_editors_box.h    \
    tests/test_db/test_group_links.h    \
    tests/test_db/test_schedule_manager.h   \
    tests/test_ui/test_schedule_editor.h    \
                                            \
    db/db_manager.h                 \
    db/people_manager.h             \
    db/group_manager.h              \
    db/schedule_manager.h           \
    db/relations_in_db.h            \
                                    \
    ui/main_window.h                \
    ui/schedule_presenter.h         \
    ui/groups_presenter.h           \
    ui/people_presenter.h           \
    ui/stats_widget.h               \
                                    \
    ui/support/schedule_editor.h    \
    ui/support/group_editor.h       \
    ui/support/person_editor.h      \
    ui/support/sportsmen_view_for_event_result.h    \
    ui/support/preview_result_calculation.h         \
    ui/support/schedule_closer.h    \
                                    \
    ui/widgets/record_chooser.h     \
    ui/widgets/records_viewer.h     \
    ui/widgets/list_view.h

SOURCES += \
    main.cpp \
    common/schedule.cpp \
    common/person.cpp \
    common/group.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




