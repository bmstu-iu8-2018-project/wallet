cmake_minimum_required(VERSION 3.1)

project(Shadowsocks-Qt5
        VERSION 3.0.0
        LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Network)
if(UNIX AND NOT APPLE)
    find_package(Qt5DBus)
endif()
find_package(PkgConfig)


pkg_check_modules(QSS REQUIRED QtShadowsocks>=2.0.0)
find_library(QSS_LIBRARY_VAR
    NAMES ${QSS_LIBRARIES}
    HINTS ${QSS_LIBRARY_DIRS} ${QSS_LIBDIR})

pkg_check_modules(QRENCODE REQUIRED libqrencode)
find_library(QRENCODE_LIBRARY_VAR
    NAMES ${QRENCODE_LIBRARIES}
    HINTS ${QRENCODE_LIBRARY_DIRS} ${QRENCODE_LIBDIR})

pkg_check_modules(ZBAR REQUIRED zbar)
find_library(ZBAR_LIBRARY_VAR
    NAMES ${ZBAR_LIBRARIES}
    HINTS ${ZBAR_LIBRARY_DIRS} ${ZBAR_LIBDIR})


if(WIN32 OR APPLE)
    add_definitions(-DFD_SETSIZE=1024)
endif()
set(APP_NAME "ss-qt5")

add_definitions(-DAPP_VERSION="${PROJECT_VERSION}")

set(INCLUDES
	includes/
)

set(SOURCE
	src/
)

if (WIN32)
    list(APPEND SOURCE ss-qt5.rc)
endif()

add_executable(${APP_NAME} ${SOURCE})


target_link_libraries(${APP_NAME}
    PUBLIC Qt5::Core
    PUBLIC Qt5::Gui
    PUBLIC Qt5::Widgets
    PUBLIC Qt5::Network
    PRIVATE ${QSS_LIBRARY_VAR}
    PRIVATE ${QRENCODE_LIBRARY_VAR}
    PRIVATE ${ZBAR_LIBRARY_VAR})

target_include_directories(${APP_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${QSS_INCLUDE_DIRS}
    PRIVATE ${QRENCODE_INCLUDE_DIRS}
    PRIVATE ${ZBAR_INCLUDE_DIRS})


if (UNIX AND NOT APPLE)
    target_link_libraries(${APP_NAME} PRIVATE Qt5::DBus)
endif()


install(TARGETS ${APP_NAME}
    RUNTIME DESTINATION bin)
