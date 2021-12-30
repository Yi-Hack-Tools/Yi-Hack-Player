cmake_minimum_required(VERSION 3.5)

project(YI-Hack-Player VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools Sql Network REQUIRED)

find_library(QTAV_LIB QtAV)
find_library(QTAVWIDGETS_LIB QtAVWidgets)

find_path(QTAV_INCLUDE QtAV/QtAV.h)
find_path(QTAVWIDGETS_INCLUDE QtAVWidgets/QtAVWidgets.h)

message("QTAV_INCLUDE = ${QTAV_INCLUDE}/QtAV")
message("QTAVWIDGETS_INCLUDE = ${QTAVWIDGETS_INCLUDE}/QtAVWidgets")

include_directories(${QTAV_INCLUDE}/QtAV ${QTAVWIDGETS_INCLUDE}/QtAVWidgets)

set(TS_FILES YI-Hack-Player_pt_BR.ts)

set(PROJECT_SOURCES
    src/camera.cpp
    src/cameradao.cpp
    src/camerainfowidget.cpp
    src/database.cpp
    src/ftpdirswidget.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/qt5ftp/qftp/qftp.cpp
    src/qt5ftp/qftp/qurlinfo.cpp
    src/ftpclient-cpp/FTP/FTPClient.cpp
    src/ftpclient-cpp/FTP/CurlHandle.cpp
    src/camera.h
    src/cameradao.h
    src/camerainfowidget.h
    src/database.h
    src/ftpdirswidget.h
    src/mainwindow.h
    src/noteditabledelegate.h
    src/qt5ftp/qftp/qftp.h
    src/qt5ftp/qftp/qurlinfo.h
    src/ftpclient-cpp/FTP/FTPClient.h
    src/ftpclient-cpp/FTP/CurlHandle.h
    data/resources.qrc
    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(YI-Hack-Player
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET YI-Hack-Player APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(YI-Hack-Player SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(YI-Hack-Player
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

find_package (CURL)

target_link_libraries(YI-Hack-Player PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Network ${CURL_LIBRARIES} ${QTAV_LIB} ${QTAVWIDGETS_LIB})

set_target_properties(YI-Hack-Player PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(YI-Hack-Player)
endif()
