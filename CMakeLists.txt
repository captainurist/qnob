cmake_minimum_required(VERSION 3.19.0)

project(qnob VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_DEFAULT_MAJOR_VERSION 6)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

set(src
    src/config/config_exception.h
    src/config/entity_config.h
    src/config/full_config.cpp
    src/config/full_config.h
    src/core/app/app.cpp
    src/core/app/app.h
    src/core/entity/entity.h
    src/core/entity/entity_creation_context.cpp
    src/core/entity/entity_creation_context.h
    src/core/entity/entity_creation_exception.h
    src/core/entity/entity_factory.h
    src/core/entity/entity_factory_pool.cpp
    src/core/entity/entity_factory_pool.h
    src/core/entity/entity_parsers.cpp
    src/core/entity/entity_parsers.h
    src/core/entity/entity_pool.cpp
    src/core/entity/entity_pool.h
    src/core/entity/entity_pool_builder.cpp
    src/core/entity/entity_pool_builder.h
    src/core/entity/factory_resolver.h
    src/core/knob/brightness_shaft.cpp
    src/core/knob/brightness_shaft.h
    src/core/knob/knob.cpp
    src/core/knob/knob.h
    src/core/knob/shaft.h
    src/core/knob/volume_shaft.cpp
    src/core/knob/volume_shaft.h
    src/core/osd/osd.cpp
    src/core/osd/osd.h
    src/core/osd/osd_factory.cpp
    src/core/osd/osd_factory.h
    src/core/osd/osd_fsm.cpp
    src/core/osd/osd_fsm.h
    src/core/osd/osd_window.cpp
    src/core/osd/osd_window.h
    src/core/skin/skin.cpp
    src/core/skin/skin.h
    src/core/skin/skin_factory.cpp
    src/core/skin/skin_factory.h
    src/core/sound/sound.cpp
    src/core/sound/sound.h
    src/core/sound/sound_factory.cpp
    src/core/sound/sound_factory.h
    src/core/trigger/hotkey_trigger.cpp
    src/core/trigger/hotkey_trigger.h
    src/core/trigger/hotkey_trigger_factory.cpp
    src/core/trigger/hotkey_trigger_factory.h
    src/core/trigger/trigger.h
    src/lib/metacall/bound_meta_call.cpp
    src/lib/metacall/bound_meta_call.h
    src/lib/metacall/meta_method_invoker.cpp
    src/lib/metacall/meta_method_invoker.h
    src/lib/metacall/meta_object_extension.cpp
    src/lib/metacall/meta_object_extension.h
    src/lib/monitor/monitor.cpp
    src/lib/monitor/monitor.h
    src/lib/monitor/monitor_action.h
    src/lib/monitor/monitor_manager.cpp
    src/lib/monitor/monitor_manager.h
    src/lib/monitor/monitor_queue.cpp
    src/lib/monitor/monitor_queue.h
    src/lib/monitor/monitor_server.cpp
    src/lib/monitor/monitor_server.h
    src/lib/serialization/alignment.cpp
    src/lib/serialization/alignment.h
    src/lib/serialization/enum_serialization.h
    src/lib/serialization/serialization_exceptions.cpp
    src/lib/serialization/serialization_exceptions.h
    src/lib/serialization/serialization_fwd.h
    src/main.cpp
    src/platform/platform.h
    src/platform/platform_initializer.cpp
    src/platform/platform_initializer.h
    src/platform/platform_monitor.h
    src/platform/platform_monitor_manager.h
    src/platform/platform_shortcut_notifier.h
    src/platform/platform_volume_control.h
    src/platform/win/com.cpp
    src/platform/win/com.h
    src/platform/win/com_ptr.h
    src/platform/win/win_ddc_triplet.h
    src/platform/win/win_error.cpp
    src/platform/win/win_error.h
    src/platform/win/win_monitor.cpp
    src/platform/win/win_monitor.h
    src/platform/win/win_monitor_manager.cpp
    src/platform/win/win_monitor_manager.h
    src/platform/win/win_native_event_window.h
    src/platform/win/win_platform.cpp
    src/platform/win/win_platform.h
    src/platform/win/win_shortcut_dispatcher.cpp
    src/platform/win/win_shortcut_dispatcher.h
    src/platform/win/win_shortcut_notifier.h
    src/platform/win/win_volume_control.cpp
    src/platform/win/win_volume_control.h
    src/platform/win/win_volume_event_handler.cpp
    src/platform/win/win_volume_event_handler.h
    src/util/bad_cast_exception.cpp
    src/util/bad_cast_exception.h
    src/util/debug.cpp
    src/util/debug.h
    src/util/exception.cpp
    src/util/exception.h
    src/util/human_readable_name.cpp
    src/util/human_readable_name.h
    src/util/pod_pool.cpp
    src/util/pod_pool.h
    qnob.qrc
)

add_executable(qnob ${src})

# Add console to debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_property(TARGET qnob PROPERTY WIN32_EXECUTABLE false)
endif()

if (MSVC)
    # Full path of source code file in diagnostics
    target_compile_options(qnob PRIVATE /FC)
endif()

target_compile_definitions(qnob PRIVATE
    QT_DEPRECATED_WARNINGS
    QT_DISABLE_DEPRECATED_BEFORE=0x060000
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
    QT_NO_NARROWING_CONVERSIONS_IN_CONNECT
    lit=QStringLiteral
)

target_include_directories(qnob PRIVATE
    3rdparty/tomlplusplus/include
    src
)

target_link_libraries(qnob
    Qt::Core
    Qt::Gui
    Qt::Widgets
    dxva2
    ole32
    user32
)





