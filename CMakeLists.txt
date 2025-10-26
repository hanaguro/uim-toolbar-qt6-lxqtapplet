cmake_minimum_required(VERSION 3.17)
project(uim-toolbar-qt6-lxqtwidget LANGUAGES C CXX)

set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY ON)

# ==========================
#  uim autogen + configure 実行
# ==========================
set(UIM_SRC_DIR    ${CMAKE_SOURCE_DIR}/extern/uim)
set(UIM_BUILD_DIR  ${CMAKE_BINARY_DIR}/extern/uim)
set(UIM_COPY_STAMP ${UIM_BUILD_DIR}/.sources-copied)
set(UIM_CONFIGURE_SCRIPT ${UIM_BUILD_DIR}/configure)
set(UIM_CONFIG_STATUS ${UIM_BUILD_DIR}/config.status)
set(UIM_CONFIG_HEADER ${UIM_BUILD_DIR}/config.h)
set(UIM_QT4_TOOLBAR_CONFIG ${UIM_BUILD_DIR}/qt4/toolbar/config.h)

add_custom_command(
    OUTPUT ${UIM_COPY_STAMP}
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${UIM_BUILD_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${UIM_SRC_DIR} ${UIM_BUILD_DIR}
    COMMAND ${CMAKE_COMMAND} -E touch ${UIM_COPY_STAMP}
    DEPENDS ${UIM_SRC_DIR}/autogen.sh
    COMMENT "Copying uim sources into build tree"
    VERBATIM
)

add_custom_command(
    OUTPUT ${UIM_CONFIGURE_SCRIPT}
    COMMAND ${CMAKE_COMMAND} -E env NOCONFIGURE=1 ./autogen.sh
    WORKING_DIRECTORY ${UIM_BUILD_DIR}
    DEPENDS ${UIM_COPY_STAMP}
    COMMENT "Running uim autogen.sh"
    VERBATIM
)

add_custom_target(uim_autogen DEPENDS ${UIM_CONFIGURE_SCRIPT})

add_custom_command(
    OUTPUT ${UIM_CONFIG_STATUS} ${UIM_CONFIG_HEADER} ${UIM_QT4_TOOLBAR_CONFIG}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${UIM_BUILD_DIR}
    COMMAND ${CMAKE_COMMAND} -E env
            CC=${CMAKE_C_COMPILER}
            CXX=${CMAKE_CXX_COMPILER}
            ./configure
                --prefix=/usr
		--libdir=/usr/lib
		--sysconfdir=/etc
		--localstatedir=/var
		--mandir=/usr/share/man
		--disable-static
		--enable-shared
		--with-qt6
		--with-qt6-immodule
    WORKING_DIRECTORY ${UIM_BUILD_DIR}
    DEPENDS ${UIM_CONFIGURE_SCRIPT}
    BYPRODUCTS ${UIM_BUILD_DIR}/Makefile
    COMMENT "Configuring uim (extern/uim)"
    VERBATIM
)

add_custom_target(uim_config ALL DEPENDS ${UIM_CONFIG_STATUS})
add_dependencies(uim_config uim_autogen)

# ==========================
#  ビルド設定
# ==========================
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==========================
#  Debugマクロ設定
# ==========================
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_BUILD)
    message(STATUS "Debug build: DEBUG_BUILD macro defined")
else()
    message(STATUS "Release build: Debug logs disabled")
endif()

# ==========================
#  依存関係
# ==========================
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)
if (Qt6_VERSION VERSION_LESS "6.4.0")
    find_package(Qt6 REQUIRED COMPONENTS Core5Compat)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(LXQT REQUIRED lxqt)
pkg_check_modules(UIM REQUIRED uim)

# ==========================
#  パネルプラグイン設定
# ==========================
set(LXQT_PANEL_PLUGIN_DIR "/usr/lib/lxqt-panel")

# ==========================
#  Step 6: ソース
# ==========================
add_library(uim-toolbar-qt6-lxqtwidget SHARED
    src/main.cpp
    src/uimtoolbarwidget.cpp
    src/uimtoolbarplugin.cpp
    ${CMAKE_SOURCE_DIR}/extern/uim/qt4/toolbar/common-quimhelpertoolbar.cpp
    ${CMAKE_SOURCE_DIR}/extern/uim/qt4/toolbar/common-uimstateindicator.cpp
)

# ==========================
#  Step 7: インクルードパス
# ==========================
target_include_directories(uim-toolbar-qt6-lxqtwidget PRIVATE
    ${UIM_BUILD_DIR}
    ${UIM_BUILD_DIR}/replace
    ${UIM_BUILD_DIR}/uim
    ${UIM_BUILD_DIR}/uim/uim
    ${UIM_BUILD_DIR}/qt4
    ${UIM_BUILD_DIR}/qt4/toolbar
    ${CMAKE_CURRENT_BINARY_DIR}
    ${LXQT_INCLUDE_DIRS}
    ${UIM_INCLUDE_DIRS}
    /usr/include/lxqt
    /usr/include/lxqt/LXQt
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

add_dependencies(uim-toolbar-qt6-lxqtwidget uim_config)

# ==========================
#  Step 9: インストール
# ==========================
install(TARGETS uim-toolbar-qt6-lxqtwidget
    LIBRARY DESTINATION ${LXQT_PANEL_PLUGIN_DIR}
)

set(LXQT_PANEL_DESKTOP_DIR "/usr/share/lxqt/lxqt-panel")
install(FILES uim-toolbar-qt6-lxqtwidget.desktop
    DESTINATION ${LXQT_PANEL_DESKTOP_DIR}
)

# ==========================
#  make uninstall ターゲット
# ==========================
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY
    )

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
        COMMENT "Uninstalling files installed by 'make install'"
    )
endif()
