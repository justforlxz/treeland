if(QT_KNOWN_POLICY_QTP0001)
    qt_policy(SET QTP0001 NEW)
    # the RESOURCE_PREFIX argument for qt_add_qml_module() defaults to ":/qt/qml/"
endif()
if(POLICY CMP0071)
    # https://cmake.org/cmake/help/latest/policy/CMP0071.html
    cmake_policy(SET CMP0071 NEW)
endif()

# Include translation utilities
include(${CMAKE_SOURCE_DIR}/cmake/TranslationUtils.cmake)

find_package(Dtk6 REQUIRED COMPONENTS Core Declarative SystemSettings)
if (NOT DISABLE_DDM)
    find_package(DDM REQUIRED COMPONENTS Auth Common)
endif()
find_package(QT NAMES Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 CONFIG REQUIRED ShaderTools Concurrent)
find_package(Qt6 COMPONENTS Quick QuickControls2 REQUIRED)
pkg_search_module(PIXMAN REQUIRED IMPORTED_TARGET pixman-1)
pkg_search_module(WAYLAND REQUIRED IMPORTED_TARGET wayland-server)
pkg_search_module(LIBINPUT REQUIRED IMPORTED_TARGET libinput)
pkg_search_module(XCB REQUIRED IMPORTED_TARGET xcb)
pkg_check_modules(PAM REQUIRED IMPORTED_TARGET pam)
# qt_finalize_target will collect all executable's private dependencies that are CMake targets

qt_add_dbus_adaptor(DBUS_ADAPTOR "${CMAKE_SOURCE_DIR}/misc/dbus/org.deepin.compositor1.xml" "core/treeland.h" Treeland::Treeland)

set_source_files_properties("${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.DisplayManager.xml" PROPERTIES
    NO_NAMESPACE ON
    CLASSNAME DisplayManager
)
set_source_files_properties("${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.DisplayManager.Seat.xml" PROPERTIES
    NO_NAMESPACE ON
    CLASSNAME DisplaySeat
)

set_source_files_properties("${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.DisplayManager.Session.xml" PROPERTIES
    NO_NAMESPACE ON
    CLASSNAME DisplaySession
)

set_source_files_properties("${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.login1.Manager.xml" PROPERTIES
   INCLUDE "loginddbustypes.h"
)
set_source_files_properties("${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.login1.Seat.xml" PROPERTIES
   INCLUDE "loginddbustypes.h"
)

set_source_files_properties("${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.login1.Session.xml" PROPERTIES
   INCLUDE "loginddbustypes.h"
)

set_source_files_properties("${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.login1.User.xml" PROPERTIES
   INCLUDE "loginddbustypes.h"
)

qt_add_dbus_interface(DDM_DBUS_SOURCES "${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.DisplayManager.xml"          DisplayManager)
qt_add_dbus_interface(DDM_DBUS_SOURCES "${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.DisplayManager.Seat.xml"     DisplayManagerSeat)
qt_add_dbus_interface(DDM_DBUS_SOURCES "${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.DisplayManager.Session.xml"  DisplayManagerSession)
qt_add_dbus_interface(DDM_DBUS_SOURCES "${CMAKE_SOURCE_DIR}/misc/interfaces/org.deepin.DisplayManager.xml"               DDMDisplayManager)
qt_add_dbus_interface(DBUS_INTERFACE   "${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.login1.Manager.xml"          Login1Manager)
qt_add_dbus_interface(DBUS_INTERFACE   "${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.login1.Seat.xml"             Login1Seat)
qt_add_dbus_interface(DBUS_INTERFACE   "${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.login1.Session.xml"          Login1Session)
qt_add_dbus_interface(DBUS_INTERFACE   "${CMAKE_SOURCE_DIR}/misc/interfaces/org.freedesktop.login1.User.xml"             Login1User)

qt_add_library(libtreeland SHARED)

set_target_properties(libtreeland PROPERTIES
    OUTPUT_NAME "treeland"
)

qt_add_qml_module(libtreeland
    URI Treeland
    VERSION "2.0"

    SOURCES
        ${DBUS_ADAPTOR}
        ${DDM_DBUS_SOURCES}
        ${DBUS_INTERFACE}
        config/treelandconfig.cpp
        config/treelandconfig.h
        core/layersurfacecontainer.cpp
        core/layersurfacecontainer.h
        core/popupsurfacecontainer.cpp
        core/popupsurfacecontainer.h
        core/qmlengine.cpp
        core/qmlengine.h
        core/rootsurfacecontainer.cpp
        core/rootsurfacecontainer.h
        core/shellhandler.cpp
        core/shellhandler.h
        core/treeland.cpp
        core/treeland.h
        core/windowpicker.cpp
        core/windowpicker.h
        effects/tquickradiuseffect.cpp
        effects/tquickradiuseffect.h
        effects/tquickradiuseffect_p.h
        effects/tsgradiusimagenode.cpp
        effects/tsgradiusimagenode.h
        $<$<NOT:$<BOOL:${DISABLE_DDM}>>:core/lockscreen.h>
        $<$<NOT:$<BOOL:${DISABLE_DDM}>>:core/lockscreen.cpp>
        $<$<NOT:$<BOOL:${DISABLE_DDM}>>:greeter/global.h>
        $<$<NOT:$<BOOL:${DISABLE_DDM}>>:greeter/greeterproxy.cpp>
        $<$<NOT:$<BOOL:${DISABLE_DDM}>>:greeter/greeterproxy.h>
        $<$<NOT:$<BOOL:${DISABLE_DDM}>>:greeter/sessionmodel.cpp>
        $<$<NOT:$<BOOL:${DISABLE_DDM}>>:greeter/sessionmodel.h>
        $<$<NOT:$<BOOL:${DISABLE_DDM}>>:greeter/user.cpp>
        $<$<NOT:$<BOOL:${DISABLE_DDM}>>:greeter/user.h>
        $<$<NOT:$<BOOL:${DISABLE_DDM}>>:greeter/usermodel.cpp>
        $<$<NOT:$<BOOL:${DISABLE_DDM}>>:greeter/usermodel.h>
        input/gestures.cpp
        input/gestures.h
        input/inputdevice.cpp
        input/inputdevice.h
        input/togglablegesture.cpp
        input/togglablegesture.h
        interfaces/baseplugininterface.h
        interfaces/lockscreeninterface.h
        interfaces/multitaskviewinterface.h
        interfaces/plugininterface.h
        interfaces/proxyinterface.h
        output/output.cpp
        output/output.h
        seat/helper.cpp
        seat/helper.h
        surface/surfacecontainer.cpp
        surface/surfacecontainer.h
        surface/surfacefilterproxymodel.cpp
        surface/surfacefilterproxymodel.h
        surface/surfaceproxy.cpp
        surface/surfaceproxy.h
        surface/surfacewrapper.cpp
        surface/surfacewrapper.h
        utils/cmdline.cpp
        utils/cmdline.h
        utils/propertymonitor.cpp
        utils/propertymonitor.h
        utils/loginddbustypes.h
        utils/loginddbustypes.cpp
        wallpaper/wallpapercontroller.cpp
        wallpaper/wallpapercontroller.h
        wallpaper/wallpaperimage.cpp
        wallpaper/wallpaperimage.h
        wallpaper/wallpapermanager.cpp
        wallpaper/wallpapermanager.h
        workspace/workspace.cpp
        workspace/workspace.h
        workspace/workspaceanimationcontroller.cpp
        workspace/workspaceanimationcontroller.h
        workspace/workspacemodel.cpp
        workspace/workspacemodel.h

    QML_FILES
        core/qml/PrimaryOutput.qml
        core/qml/CopyOutput.qml
        core/qml/TitleBar.qml
        core/qml/Decoration.qml
        core/qml/WindowMenu.qml
        core/qml/TaskBar.qml
        core/qml/SurfaceContent.qml
        core/qml/XdgShadow.qml
        core/qml/Border.qml
        core/qml/OutputMenuBar.qml
        core/qml/WorkspaceSwitcher.qml
        core/qml/WorkspaceProxy.qml
        core/qml/Animations/GeometryAnimation.qml
        core/qml/Animations/NewAnimation.qml
        core/qml/Animations/MinimizeAnimation.qml
        core/qml/Animations/ShowDesktopAnimation.qml
        core/qml/Animations/LaunchpadAnimation.qml
        core/qml/Animations/LayerShellAnimation.qml
        core/qml/Effects/Blur.qml
        core/qml/Effects/LaunchpadCover.qml
        core/qml/TaskSwitcher.qml
        core/qml/TaskWindowPreview.qml
        core/qml/SwitchViewDelegate.qml
        core/qml/SwitchViewHighlightDelegate.qml
        core/qml/DockPreview.qml
        core/qml/FadeBehavior.qml
        core/qml/CaptureSelectorLayer.qml
        core/qml/WindowPickerLayer.qml
    RESOURCE_PREFIX
        /qt/qml
    OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/qt/qml/Treeland
)

target_compile_definitions(libtreeland
    PRIVATE
    WLR_USE_UNSTABLE
)

qt_add_shaders(libtreeland "treeland_shaders_ng"
    BATCHABLE
    PRECOMPILE
    PREFIX
        "/shaders"
    BASE
        ${PROJECT_RESOURCES_DIR}/shaders
    FILES
        ${PROJECT_RESOURCES_DIR}/shaders/radiussmoothtexture.vert
        ${PROJECT_RESOURCES_DIR}/shaders/radiussmoothtexture.frag
)

qt_add_resources(libtreeland "treeland_assets"
    PREFIX "/dsg/icons"
    BASE ${PROJECT_RESOURCES_DIR}/icons
    FILES
        ${PROJECT_RESOURCES_DIR}/icons/close.dci
        ${PROJECT_RESOURCES_DIR}/icons/add.dci
        ${PROJECT_RESOURCES_DIR}/icons/select_output.dci
        ${PROJECT_RESOURCES_DIR}/icons/select_window.dci
        ${PROJECT_RESOURCES_DIR}/icons/select_region.dci
        ${PROJECT_RESOURCES_DIR}/icons/login_capslock.dci
        ${PROJECT_RESOURCES_DIR}/icons/login_display_password.dci
        ${PROJECT_RESOURCES_DIR}/icons/login_hidden_password.dci
        ${PROJECT_RESOURCES_DIR}/icons/login_keyboard.dci
        ${PROJECT_RESOURCES_DIR}/icons/login_lock.dci
        ${PROJECT_RESOURCES_DIR}/icons/login_open.dci
        ${PROJECT_RESOURCES_DIR}/icons/login_power.dci
        ${PROJECT_RESOURCES_DIR}/icons/login_reboot.dci
        ${PROJECT_RESOURCES_DIR}/icons/login_hibernate.dci
        ${PROJECT_RESOURCES_DIR}/icons/login_user.dci
        ${PROJECT_RESOURCES_DIR}/icons/login_hint.dci
        ${PROJECT_RESOURCES_DIR}/icons/login_logout.dci
        ${PROJECT_RESOURCES_DIR}/icons/login_shutdown.dci
        ${PROJECT_RESOURCES_DIR}/icons/login_suspend.dci
        ${PROJECT_RESOURCES_DIR}/icons/login_switchuser.dci
        ${PROJECT_RESOURCES_DIR}/icons/login_update_reboot.dci
        ${PROJECT_RESOURCES_DIR}/icons/login_update_shutdown.dci
        ${PROJECT_RESOURCES_DIR}/icons/backToLightdm.dci
        ${PROJECT_RESOURCES_DIR}/icons/logo.svg
)

# Use translation utilities to setup main translations
setup_main_translations(libtreeland)

target_include_directories(libtreeland
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/config>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/core>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/effects>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/greeter>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/input>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/interfaces>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/output>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/seat>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/surface>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/workspace>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/utils>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/wallpaper>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/treeland>
)

target_link_libraries(libtreeland
    PUBLIC
        Dtk6::Core
        Dtk6::Declarative
        Dtk6::SystemSettings
        Waylib::WaylibServer
    PRIVATE
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::QuickPrivate
        Qt6::DBus
        Qt6::Concurrent
        PkgConfig::PIXMAN
        PkgConfig::WAYLAND
        PkgConfig::LIBINPUT
        $<$<NOT:$<BOOL:${DISABLE_DDM}>>:DDM::Auth>
        $<$<NOT:$<BOOL:${DISABLE_DDM}>>:DDM::Common>
        $<$<NOT:$<BOOL:${DISABLE_DDM}>>:PkgConfig::PAM>
)

add_subdirectory(plugins)
add_subdirectory(modules)
add_subdirectory(treeland-shortcut)

set(BIN_NAME treeland)

qt_add_executable(${BIN_NAME}
    main.cpp
)

target_include_directories(${BIN_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/interfaces/>
)

target_link_libraries(${BIN_NAME} PRIVATE
    libtreeland
)

install(TARGETS ${BIN_NAME} RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(PROGRAMS treeland.sh DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(TARGETS libtreeland LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
# TODO: The qml plugin installation directory still needs to be adjusted
install(DIRECTORY ${PROJECT_BINARY_DIR}/qt/qml/Treeland DESTINATION "${TREELAND_DATA_DIR}/qml/Treeland")

## systemd socket

pkg_check_modules(Systemd REQUIRED IMPORTED_TARGET libsystemd)

qt_add_executable(treeland-sd
    systemd-socket.cpp
)

target_link_libraries(treeland-sd
    PRIVATE
        Qt6::DBus
        PkgConfig::Systemd
)

install(TARGETS treeland-sd RUNTIME DESTINATION "${CMAKE_INSTALL_LIBEXECDIR}")
