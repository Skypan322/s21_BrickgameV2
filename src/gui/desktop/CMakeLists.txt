cmake_minimum_required(VERSION 3.16)

# Проект
project(desktop VERSION 0.1 LANGUAGES CXX C)

# Установки Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Установка стандартов языка
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Сбор исходников с помощью GLOB (оставляем, но используем аккуратнее)
file(GLOB TETRIS_SOURCES "../../brick_game/tetris/*.c" "../../brick_game/tetris/*.h")
file(GLOB SNAKE_SOURCES "../../brick_game/snake/*.cpp" "../../brick_game/snake/*.h")
file(GLOB UTILS_SOURCES "../../brick_game/common/*.c" "../../brick_game/common/*.h")

# Исходники C++
set(CPP_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ${SNAKE_SOURCES}
)

# Исходники C
set(C_SOURCES
    ${TETRIS_SOURCES}
    ${UTILS_SOURCES}
)

include_directories(
    ../../brick_game/snake
    ../../brick_game/tetris
    ../../brick_game/common
)

# Все исходники проекта
set(PROJECT_SOURCES
    ${CPP_SOURCES}
    ${C_SOURCES}
)

# Добавление исполняемого файла
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(desktop
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(desktop SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(desktop
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Установка библиотек
target_link_libraries(
    desktop PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    PRIVATE brick_game
)

# Установка свойств для iOS/macOS/Windows
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.desktop)
endif()

set_target_properties(desktop PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

add_library(brick_game STATIC
        ../../brick_game/tetris/figures.c
        ../../brick_game/tetris/tetris.c
        ../../brick_game/common/utils.c
)

# Установка путей для сборки
include(GNUInstallDirs)
install(TARGETS desktop
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Финализация Qt (для Qt 6)
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(desktop)
endif()

add_custom_target(run
    COMMAND desktop
    DEPENDS desktop
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
    COMMENT "Running the application..."
)

# Установка пользовательской цели как зависимость от основной цели
add_dependencies(run desktop)