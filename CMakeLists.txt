cmake_minimum_required(VERSION 3.16)
project(uim-toolbar-qt6-lxqtwidget LANGUAGES CXX)

# ==========================
#  Step 1: Dummy config.h 生成
# ==========================
set(UIM_DUMMY_CONFIG_MAIN ${CMAKE_SOURCE_DIR}/extern/uim/config.h)
set(UIM_DUMMY_CONFIG_QT4  ${CMAKE_SOURCE_DIR}/extern/uim/qt4/toolbar/config.h)

foreach(_cfg ${UIM_DUMMY_CONFIG_MAIN} ${UIM_DUMMY_CONFIG_QT4})
    message(STATUS "Creating minimal dummy ${_cfg} ...")
    get_filename_component(_cfg_dir ${_cfg} DIRECTORY)
    file(MAKE_DIRECTORY ${_cfg_dir})
    file(WRITE ${_cfg} "
#ifndef UIM_CONFIG_H
#define UIM_CONFIG_H

#define ENABLE_NLS 1
#define PACKAGE \"uim\"
#define VERSION \"1.9.6\"
#define GETTEXT_PACKAGE \"uim\"
#define UIM_PIXMAPSDIR \"/usr/share/uim/pixmaps\"
#define KDE4_ICONDIR \"/usr/share/icons\"

#endif /* UIM_CONFIG_H */
")
endforeach()

# ==========================
#  Step 2: uim/version.h 自動生成
# ==========================
set(UIM_VERSION_MAJOR 1)
set(UIM_VERSION_MINOR 9)
set(UIM_VERSION_PATCHLEVEL 6)

set(UIM_VERSION_IN  ${CMAKE_SOURCE_DIR}/extern/uim/uim/version.h.in)
set(UIM_VERSION_OUT ${CMAKE_BINARY_DIR}/generated/uim/version.h)

if (EXISTS ${UIM_VERSION_IN})
    message(STATUS "Generating uim/version.h in build directory")
    configure_file(${UIM_VERSION_IN} ${UIM_VERSION_OUT} @ONLY)
endif()

# ==========================
#  Step 3: ビルド設定
# ==========================
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==========================
#  Step 3.5: Debugマクロ設定
# ==========================
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_BUILD)
    message(STATUS "Debug build: DEBUG_BUILD macro defined")
else()
    message(STATUS "Release build: Debug logs disabled")
endif()

# ==========================
#  Step 4: 依存関係
# ==========================
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)
if (Qt6_VERSION VERSION_LESS "6.4.0")
    find_package(Qt6 REQUIRED COMPONENTS Core5Compat)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(LXQT REQUIRED lxqt)
pkg_check_modules(UIM REQUIRED uim)

include_directories(
    ${LXQT_INCLUDE_DIRS}
    ${UIM_INCLUDE_DIRS}
)
link_directories(
    ${LXQT_LIBRARY_DIRS}
    ${UIM_LIBRARY_DIRS}
)

# ==========================
#  Step 5: LXQt パネルプラグイン設定
# ==========================
set(LXQT_PANEL_PLUGIN_DIR "/usr/lib/lxqt-panel")

configure_file(${CMAKE_SOURCE_DIR}/src/uim-toolbar-qt6-lxqtwidget.json
               ${CMAKE_CURRENT_BINARY_DIR}/uim-toolbar-qt6-lxqtwidget.json
               COPYONLY)

# ==========================
#  Step 6: ソース
# ==========================
add_library(uim-toolbar-qt6-lxqtwidget SHARED
    src/main.cpp
    src/uimtoolbarwidget.cpp
    src/uimtoolbarplugin.cpp
    src/uimhelperclient.cpp
    src/uim-toolbar-qt6-lxqtwidget.json
    ${CMAKE_SOURCE_DIR}/extern/uim/qt4/toolbar/common-quimhelpertoolbar.cpp
    ${CMAKE_SOURCE_DIR}/extern/uim/qt4/toolbar/common-uimstateindicator.cpp
)

# ==========================
#  Step 7: インクルードパス
# ==========================
target_include_directories(uim-toolbar-qt6-lxqtwidget PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/generated/uim
    ${LXQT_INCLUDE_DIRS}
    ${UIM_INCLUDE_DIRS}
    /usr/include/lxqt
    /usr/include/lxqt/LXQt
    ${CMAKE_SOURCE_DIR}/extern/uim
    ${CMAKE_SOURCE_DIR}/extern/uim/uim
    ${CMAKE_SOURCE_DIR}/extern/uim/qt4
    ${CMAKE_SOURCE_DIR}/extern/uim/qt4/toolbar
)

# ==========================
#  Step 8: リンク
# ==========================
target_link_libraries(uim-toolbar-qt6-lxqtwidget
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    ${LXQT_LIBRARIES}
    ${UIM_LIBRARIES}
)

if (TARGET Qt6::Core5Compat)
    target_link_libraries(uim-toolbar-qt6-lxqtwidget Qt6::Core5Compat)
endif()

# ==========================
#  Step 9: インストール
# ==========================
install(TARGETS uim-toolbar-qt6-lxqtwidget
    LIBRARY DESTINATION ${LXQT_PANEL_PLUGIN_DIR}
)
install(FILES src/uim-toolbar-qt6-lxqtwidget.json
    DESTINATION ${LXQT_PANEL_PLUGIN_DIR}
)

set(LXQT_PANEL_DESKTOP_DIR "/usr/share/lxqt/lxqt-panel")
install(FILES uim-toolbar-qt6-lxqtwidget.desktop
    DESTINATION ${LXQT_PANEL_DESKTOP_DIR}
)

