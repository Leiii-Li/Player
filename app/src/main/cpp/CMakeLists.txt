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

add_library(avdevice-57
        SHARED
        IMPORTED)
set_target_properties(avdevice-57
        PROPERTIES IMPORTED_LOCATION
        ${ARM_DIR}/armeabi-v7a/libavdevice-57.so
        )

add_library(avfilter-6
        SHARED
        IMPORTED)
set_target_properties(avfilter-6
        PROPERTIES IMPORTED_LOCATION
        ${ARM_DIR}/armeabi-v7a/libavfilter-6.so
        )

add_library(avformat-57
        SHARED
        IMPORTED)
set_target_properties(avformat-57
        PROPERTIES IMPORTED_LOCATION
        ${ARM_DIR}/armeabi-v7a/libavformat-57.so
        )

add_library(avutil-55
        SHARED
        IMPORTED)
set_target_properties(avutil-55
        PROPERTIES IMPORTED_LOCATION
        ${ARM_DIR}/armeabi-v7a/libavutil-55.so
        )

add_library(postproc-54
        SHARED
        IMPORTED)
set_target_properties(postproc-54
        PROPERTIES IMPORTED_LOCATION
        ${ARM_DIR}/armeabi-v7a/libpostproc-54.so
        )

add_library(swresample-2
        SHARED
        IMPORTED)
set_target_properties(swresample-2
        PROPERTIES IMPORTED_LOCATION
        ${ARM_DIR}/armeabi-v7a/libswresample-2.so
        )

add_library(swscale-4
        SHARED
        IMPORTED)
set_target_properties(swscale-4
        PROPERTIES IMPORTED_LOCATION
        ${ARM_DIR}/armeabi-v7a/libswscale-4.so
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
        avdevice-57
        avfilter-6
        avformat-57
        avutil-55
        postproc-54
        swresample-2
        swscale-4

        ${log-lib})