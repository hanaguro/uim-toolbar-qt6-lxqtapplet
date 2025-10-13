cmake_minimum_required(VERSION 3.16)
project(uim-toolbar-qt6-lxqtapplet LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LXQT REQUIRED lxqt)
include_directories(${LXQT_INCLUDE_DIRS})

set(LXQT_PANEL_PLUGIN_DIR "/usr/lib/lxqt-panel")

# JSONをビルドディレクトリにコピーして moc が参照可能にする
configure_file(${CMAKE_SOURCE_DIR}/src/uim-toolbar-qt6-lxqtapplet.json
               ${CMAKE_CURRENT_BINARY_DIR}/uim-toolbar-qt6-lxqtapplet.json
               COPYONLY)

add_library(uim-toolbar-qt6-lxqtapplet SHARED
    src/main.cpp
    src/uimtoolbarwidget.cpp
    src/uimtoolbarplugin.cpp
    src/uimhelperclient.cpp         # ← 追加
    src/uim-toolbar-qt6-lxqtapplet.json
)

target_include_directories(uim-toolbar-qt6-lxqtapplet PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${LXQT_INCLUDE_DIRS}
    /usr/include/lxqt
    /usr/include/lxqt/LXQt
)

target_link_libraries(uim-toolbar-qt6-lxqtapplet
    Qt6::Core Qt6::Widgets Qt6::Network
    ${LXQT_LIBRARIES}
)

install(TARGETS uim-toolbar-qt6-lxqtapplet
    LIBRARY DESTINATION ${LXQT_PANEL_PLUGIN_DIR})

install(FILES src/uim-toolbar-qt6-lxqtapplet.json
    DESTINATION ${LXQT_PANEL_PLUGIN_DIR})

