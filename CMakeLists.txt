cmake_minimum_required(VERSION 3.21)

project(cryptKX
    VERSION 0.1.0
    DESCRIPTION "Desktop password generator using Qt and libsodium"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(Sodium REQUIRED)

add_library(cryptkx_core
    src/core/PasswordGenerator.cpp
)

target_include_directories(cryptkx_core
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
        "$<INSTALL_INTERFACE:include>"
)

target_link_libraries(cryptkx_core
    PUBLIC
        Sodium::Sodium
)

set(CRYPTKX_APP_ENTRY "${PROJECT_SOURCE_DIR}/src/main.cpp")
if(EXISTS "${CRYPTKX_APP_ENTRY}")
    set(CRYPTKX_BUILD_APP_DEFAULT ON)
else()
    set(CRYPTKX_BUILD_APP_DEFAULT OFF)
endif()

option(CRYPTKX_BUILD_APP "Build the Qt desktop application target" "${CRYPTKX_BUILD_APP_DEFAULT}")

if(CRYPTKX_BUILD_APP)
    find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml QuickControls2)

    qt_standard_project_setup(REQUIRES 6.5)
    qt_policy(SET QTP0004 NEW)

    qt_add_executable(cryptkx WIN32
        src/main.cpp
        src/app/PasswordController.cpp
        src/app/PasswordController.hpp
    )

    target_link_libraries(cryptkx
        PRIVATE
            cryptkx_core
            Qt6::Core
            Qt6::Gui
            Qt6::Quick
            Qt6::Qml
            Qt6::QuickControls2
    )

    set(CRYPTKX_QML_FILES
        src/qml/Main.qml
        src/qml/components/PasswordCard.qml
        src/qml/components/OptionCheckBox.qml
        src/qml/components/PrimaryButton.qml
    )

    set_source_files_properties(src/qml/Main.qml PROPERTIES QT_RESOURCE_ALIAS Main.qml)
    set_source_files_properties(src/qml/components/PasswordCard.qml PROPERTIES QT_RESOURCE_ALIAS components/PasswordCard.qml)
    set_source_files_properties(src/qml/components/OptionCheckBox.qml PROPERTIES QT_RESOURCE_ALIAS components/OptionCheckBox.qml)
    set_source_files_properties(src/qml/components/PrimaryButton.qml PROPERTIES QT_RESOURCE_ALIAS components/PrimaryButton.qml)

    qt_add_qml_module(cryptkx
        URI CryptKX
        VERSION 1.0
        QML_FILES
            ${CRYPTKX_QML_FILES}
    )
endif()

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
