cmake_minimum_required(VERSION 3.4.1)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(ARM_DIR E:/AndroidWorkSpace/Player/app/src/main/cpp/libs)

add_library(avcodec-57
        SHARED
        IMPORTED)
set_target_properties(avcodec-57
        PROPERTIES IMPORTED_LOCATION
        ${ARM_DIR}/armeabi-v7a/libavcodec-57.so
        )

add_library(
        native-lib

        SHARED

        native-lib.cpp)

find_library(
        log-lib

        log)

target_link_libraries(
        native-lib

        avcodec-57

        ${log-lib})