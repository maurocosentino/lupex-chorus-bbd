cmake_minimum_required(VERSION 3.24)

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" CHORUS_VERSION LIMIT_COUNT 1)

project(ChorusBBD VERSION ${CHORUS_VERSION} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(CHORUS_ENABLE_TESTS "Enable ChorusBBD test targets" OFF)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/CMakeLists.txt")
    message(FATAL_ERROR "JUCE submodule is missing. Run: git submodule update --init --recursive")
endif()

add_subdirectory(JUCE)

juce_add_plugin(ChorusBBD
        COMPANY_NAME "Fauz Audio"
        BUNDLE_ID "com.fauzaudio.chorusbbd"
        PLUGIN_MANUFACTURER_CODE Faud
        PLUGIN_CODE Chbb
        FORMATS VST3
        PRODUCT_NAME "Lupex Chorus"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        VST3_CATEGORIES Fx Modulation Stereo
        VST3_AUTO_MANIFEST FALSE
)

target_sources(ChorusBBD
        PRIVATE
        source/dsp/DelayLine.cpp
        source/dsp/DelayLine.h
        source/dsp/BBDFilter.cpp
        source/dsp/BBDFilter.h
        source/dsp/TapeEmulator.cpp
        source/dsp/TapeEmulator.h
        source/dsp/WowFlutter.cpp
        source/dsp/WowFlutter.h
        source/dsp/ChorusEngine.cpp
        source/dsp/ChorusEngine.h
        source/parameters/ChorusParameters.cpp
        source/parameters/ChorusParameters.h
        source/parameters/ParameterIDs.h
        source/plugin/PluginProcessor.cpp
        source/plugin/PluginProcessor.h
        source/plugin/PluginEditor.cpp
        source/plugin/PluginEditor.h
        source/ui/LupexKnob.cpp
        source/ui/LupexKnob.h
        source/ui/LupexToggle.cpp
        source/ui/LupexToggle.h
        source/ui/PedalLookAndFeel.cpp
        source/ui/PedalLookAndFeel.h
        source/ui/BypassButton.cpp
        source/ui/BypassButton.h
)

juce_add_binary_data(ChorusAssets
        SOURCES
        assets/images/background.png
        assets/images/knob.png
        assets/images/switch_toggle.png
        assets/images/footswitch_up.png
        assets/images/footswitch_down.png
        assets/images/screenshot.png
)

target_include_directories(ChorusBBD
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/source"
)

target_compile_definitions(ChorusBBD
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(ChorusBBD
        PRIVATE
        ChorusAssets
        juce::juce_audio_utils
        juce::juce_dsp
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

if(CHORUS_ENABLE_TESTS)
    enable_testing()

    add_executable(ChorusTests
            tests/DelayLineTest.cpp
            tests/BBDFilterTest.cpp
            tests/ChorusEngineTest.cpp
            tests/ParametersTest.cpp
    )

    target_include_directories(ChorusTests
            PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/source"
    )

    target_link_libraries(ChorusTests
            PRIVATE
            juce::juce_core
    )

    add_test(NAME ChorusTests COMMAND ChorusTests)
endif()