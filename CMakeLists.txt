cmake_minimum_required(VERSION 3.5)

project(Backup_Utility VERSION 1.2.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets LinguistTools Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Concurrent)

set(TS_FILES Backup_Utility_en_US.ts)

set(PROJECT_SOURCES
        main.cpp
        resources.qrc
        src/utility.h
        src/utility.cpp
        src/presetandconfig.cpp
        src/presetandconfig.h
        src/backupmanager.h
        src/backupmanager.cpp
        src/qrunnables.h
        src/qrunnables.cpp
        src/MainWindow/mainwindow.cpp
        src/MainWindow/mainwindow.h
        src/MainWindow/mainwindow.ui
        src/CustomLineEdit/customlineedit.h
        src/CustomLineEdit/customlineedit.cpp
        src/InputDialog/inputdialog.h
        src/InputDialog/inputdialog.cpp
        src/InputDialog/inputdialog.ui
        src/FilesDialog/filesdialog.h
        src/FilesDialog/filesdialog.cpp
        src/FilesDialog/filesdialog.ui
        ${TS_FILES}
)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Backup_Utility
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
    )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET Backup_Utility APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else ()
    if (ANDROID)
        add_library(Backup_Utility SHARED
                ${PROJECT_SOURCES}
        )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable(Backup_Utility
                ${PROJECT_SOURCES}
        )
    endif ()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif ()

target_link_libraries(Backup_Utility PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Concurrent)

set_target_properties(Backup_Utility PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER dwyriel.backup_utility.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

install(TARGETS Backup_Utility
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Backup_Utility)
endif ()
