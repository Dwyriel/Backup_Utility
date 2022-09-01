cmake_minimum_required(VERSION 3.23)
project(Backup_Utility)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Backup_Utility main.cpp)
target_link_libraries(Backup_Utility
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

