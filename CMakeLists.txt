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
    "src/config/config_exception.h"
    "src/config/qnob_args.h"
    "src/config/qnob_config.cpp"
    "src/config/qnob_config.h"
    "src/core/app/app.cpp"
    "src/core/app/app.h"
    "src/core/entity/entity.h"
    "src/core/entity/entity_creation_context.cpp"
    "src/core/entity/entity_creation_context.h"
    "src/core/entity/entity_creation_exception.h"
    "src/core/entity/entity_factory.h"
    "src/core/entity/entity_factory_pool.cpp"
    "src/core/entity/entity_factory_pool.h"
    "src/core/entity/entity_parsers.cpp"
    "src/core/entity/entity_parsers.h"
    "src/core/entity/entity_pool.cpp"
    "src/core/entity/entity_pool.h"
    "src/core/entity/entity_pool_builder.cpp"
    "src/core/entity/entity_pool_builder.h"
    "src/core/entity/factory_resolver.h"
    "src/core/knob/knob.cpp"
    "src/core/knob/knob.h"
    "src/core/knob/knob_factory.cpp"
    "src/core/knob/knob_factory.h"
    "src/core/osd/osd.cpp"
    "src/core/osd/osd.h"
    "src/core/osd/osd_factory.cpp"
    "src/core/osd/osd_factory.h"
    "src/core/osd/osd_fsm.cpp"
    "src/core/osd/osd_fsm.h"
    "src/core/osd/osd_window.cpp"
    "src/core/osd/osd_window.h"
    "src/core/setting/brightness_setting_backend.cpp"
    "src/core/setting/brightness_setting_backend.h"
    "src/core/setting/setting_backend.h"
    "src/core/setting/setting.h"
    "src/core/setting/setting.cpp"
    "src/core/setting/volume_setting_backend.cpp"
    "src/core/setting/volume_setting_backend.h"
    "src/core/skin/horizontal_bar_skin.h"
    "src/core/skin/numeric_skin.h"
    "src/core/skin/skin.h"
    "src/core/skin/skin_factory.cpp"
    "src/core/skin/skin_factory.h"
    "src/core/skin/skin_type.h"
    "src/core/skin/skin_type.cpp"
    "src/core/sound/sound.cpp"
    "src/core/sound/sound.h"
    "src/core/sound/sound_factory.cpp"
    "src/core/sound/sound_factory.h"
    "src/core/tray_event/mouse_key.h"
    "src/core/tray_event/mouse_key.cpp"
    "src/core/tray_event/tray_event.h"
    "src/core/tray_event/tray_event.cpp"
    "src/core/tray_event/tray_event_factory.h"
    "src/core/tray_event/tray_event_factory.cpp"
    "src/core/tray_icon/standard_tray_icon.h"
    "src/core/tray_icon/standard_tray_icon.cpp"
    "src/core/tray_icon/tray_icon.h"
    "src/core/tray_icon/tray_icon.cpp"
    "src/core/tray_icon/tray_icon_factory.h"
    "src/core/tray_icon/tray_icon_factory.cpp"
    "src/core/hotkey/hotkey.cpp"
    "src/core/hotkey/hotkey.h"
    "src/core/hotkey/hotkey_factory.cpp"
    "src/core/hotkey/hotkey_factory.h"
    "src/lib/metacall/bound_meta_call.cpp"
    "src/lib/metacall/bound_meta_call.h"
    "src/lib/metacall/meta_method_invoker.cpp"
    "src/lib/metacall/meta_method_invoker.h"
    "src/lib/metacall/meta_object_extension.cpp"
    "src/lib/metacall/meta_object_extension.h"
    "src/lib/monitor/monitor.cpp"
    "src/lib/monitor/monitor.h"
    "src/lib/monitor/monitor_action.h"
    "src/lib/monitor/monitor_manager.cpp"
    "src/lib/monitor/monitor_manager.h"
    "src/lib/monitor/monitor_queue.cpp"
    "src/lib/monitor/monitor_queue.h"
    "src/lib/monitor/monitor_server.cpp"
    "src/lib/monitor/monitor_server.h"
    "src/lib/serializable_types/alignment.cpp"
    "src/lib/serializable_types/alignment.h"
    "src/lib/serializable_types/color.cpp"
    "src/lib/serializable_types/color.h"
    "src/lib/serialization/enum_serialization.h"
    "src/lib/serialization/serialization_exceptions.cpp"
    "src/lib/serialization/serialization_exceptions.h"
    "src/lib/serialization/serialization_fwd.h"
    "src/main.cpp"
    "src/platform/platform.h"
    "src/platform/platform_control.h"
    "src/platform/platform_initializer.cpp"
    "src/platform/platform_initializer.h"
    "src/platform/platform_monitor.h"
    "src/platform/platform_monitor_manager.h"
    "src/platform/platform_shortcut_notifier.h"
    "src/platform/platform_shortcut_manager.h"
    "src/platform/platform_volume_control.h"
    "src/platform/platform_wheel_event_manager.h"
    "src/platform/win/com.cpp"
    "src/platform/win/com.h"
    "src/platform/win/com_ptr.h"
    "src/platform/win/win_ddc_triplet.h"
    "src/platform/win/win_error.cpp"
    "src/platform/win/win_error.h"
    "src/platform/win/win_global_mouse_hook.cpp"
    "src/platform/win/win_global_mouse_hook.h"
    "src/platform/win/win_monitor.cpp"
    "src/platform/win/win_monitor.h"
    "src/platform/win/win_monitor_manager.cpp"
    "src/platform/win/win_monitor_manager.h"
    "src/platform/win/win_shared_event_window.h"
    "src/platform/win/win_platform.cpp"
    "src/platform/win/win_platform.h"
    "src/platform/win/win_shortcut_manager.cpp"
    "src/platform/win/win_shortcut_manager.h"
    "src/platform/win/win_shortcut_notifier.h"
    "src/platform/win/win_standard_control.cpp"
    "src/platform/win/win_standard_control.h"
    "src/platform/win/win_wheel_event_manager.cpp"
    "src/platform/win/win_wheel_event_manager.h"
    "src/platform/win/win_volume_control.cpp"
    "src/platform/win/win_volume_control.h"
    "src/platform/win/win_volume_event_handler.cpp"
    "src/platform/win/win_volume_event_handler.h"
    "src/util/bad_cast_exception.cpp"
    "src/util/bad_cast_exception.h"
    "src/util/debug.cpp"
    "src/util/debug.h"
    "src/util/exception.cpp"
    "src/util/exception.h"
    "src/util/human_readable_name.cpp"
    "src/util/human_readable_name.h"
    "src/util/pod_pool.cpp"
    "src/util/pod_pool.h"
    "qnob.qrc"
    "src/lib/command_line/command_line_option.h"
    "src/lib/command_line/command_line_section.h"
    "src/lib/command_line/command_line_option_builder.h"
    "src/lib/command_line/option_parsers.h"
    "src/lib/command_line/command_line_parser.cpp"
    "src/lib/command_line/command_line_exception.h"
    "src/lib/serialization/builtin_serialization.h"
    "src/lib/serialization/builtin_serialization.cpp"
    "src/lib/command_line/command_line_help_options.h"
    "src/platform/win/win_crtio.h"
 "src/platform/platform_metrics.h"
 "src/platform/win/win_metrics.h"
 "src/platform/win/win_metrics.cpp"
 "src/platform/win/win_shared_event_window.cpp"
 "src/core/tray_icon/fixed_size_icon_engine.h"
 "src/platform/win/win_thread_message_hook.h"
 "src/platform/win/win_thread_message_hook.cpp")

set(tests
    "src/util/exception_ut.cpp"
)

set(defines
    QT_DEPRECATED_WARNINGS
    QT_DISABLE_DEPRECATED_BEFORE=0x060000
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
    QT_NO_NARROWING_CONVERSIONS_IN_CONNECT
    lit=QStringLiteral
)


#
# Set up qnob executable
#
add_executable(qnob ${src})

set_property(TARGET qnob PROPERTY WIN32_EXECUTABLE true) # No console by default

if (MSVC)
    target_compile_options(qnob PRIVATE /FC) # Full path of source code file in diagnostics
    # target_compile_options(qnob PRIVATE /MT) # Link statically with CRT
endif()

target_compile_definitions(qnob PRIVATE ${defines})

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
    shcore
)


#
# Set up tests
#
enable_testing()
add_subdirectory(3rdparty/googletest)

add_executable(qnob_test ${tests})

target_compile_definitions(qnob_test PRIVATE ${defines})

target_include_directories(qnob_test PRIVATE
    3rdparty/googletest/googletest/include
    src
)

target_link_libraries(qnob_test
    gtest_main
    Qt::Core
)

add_test(NAME qnob_test COMMAND qnob_test)




