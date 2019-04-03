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
    ui/support/preview_result_calculation.h         \
    ui/support/schedule_closer.h    \
                                    \
    ui/widgets/record_chooser.h     \
    ui/widgets/records_viewer.h     \
    ui/support/sportsmen_result_preview.h

SOURCES += \
    main.cpp \
    common/schedule.cpp \
    common/person.cpp \
    common/group.cpp \
    db/db_manager.cpp \
    db/group_manager.cpp \
    db/people_manager.cpp \
    db/relations_in_db.cpp \
    db/schedule_manager.cpp \
    ui/support/group_editor.cpp \
    ui/support/person_editor.cpp \
    ui/support/preview_result_calculation.cpp \
    ui/support/schedule_closer.cpp \
    ui/support/schedule_editor.cpp \
    ui/support/sportsmen_result_preview.cpp \
    ui/widgets/record_chooser.cpp \
    ui/widgets/records_viewer.cpp \
    ui/groups_presenter.cpp \
    ui/main_window.cpp \
    ui/people_presenter.cpp \
    ui/schedule_presenter.cpp \
    ui/stats_widget.cpp \
    sport_section.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




