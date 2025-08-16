cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(MQTT_SN_FORMAT_BUILD_TESTS "Build tests" ON)

project(mqtt-sn-format
VERSION 0.0.1
DESCRIPTION "MQTT-SN Format"
LANGUAGES C CXX
)

add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
src/format.cc
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include)

if(MQTT_SN_FORMAT_BUILD_TESTS)
    enable_testing()
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.9.0 # or a later release
    )
    FetchContent_MakeAvailable(Catch2)

    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(CTest)
    include(Catch)
    
    add_subdirectory(test)
endif()