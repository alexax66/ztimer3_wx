cmake_minimum_required (VERSION 3.10)

project(ztimer3_wx)

#wxWidgets
find_package(wxWidgets COMPONENTS core base adv REQUIRED)
include( "${wxWidgets_USE_FILE}" )

#
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

set(SRCS
    src/GUIDialog.cpp
    src/OSTimer.cpp
    src/wxprec.cpp
    src/zAddReminderDlg.cpp
    src/zChooseTimeDlg.cpp
    src/zConfig.cpp
    src/zContextMenu.cpp
    src/zReminderDlg.cpp
    src/zTestWxApp.cpp
    src/zTestWxDialog.cpp
    src/ztTaskBarIcon.cpp
    src/zTimerServ/time_utils.cpp
    src/zTimerServ/zReminder.cpp
    src/zTimerServ/zTimerServ.cpp
)

add_executable(ztimer3_wx ${SRCS})

target_link_libraries(ztimer3_wx ${wxWidgets_LIBRARIES})
