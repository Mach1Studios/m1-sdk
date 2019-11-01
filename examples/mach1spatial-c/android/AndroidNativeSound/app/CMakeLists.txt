# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)


set(lib_src_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})
include_directories(
        ${CMAKE_SOURCE_DIR}/include
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")

add_library(libavcodec_lib SHARED IMPORTED)
set_target_properties(libavcodec_lib PROPERTIES IMPORTED_LOCATION ${lib_src_DIR}/libavcodec-57.so)

add_library(libavfilter_lib SHARED IMPORTED)
set_target_properties(libavfilter_lib PROPERTIES IMPORTED_LOCATION ${lib_src_DIR}/libavfilter-6.so)

add_library(libavformat_lib SHARED IMPORTED)
set_target_properties(libavformat_lib PROPERTIES IMPORTED_LOCATION  ${lib_src_DIR}/libavformat-57.so)

add_library(libavutil_lib SHARED IMPORTED)
set_target_properties(libavutil_lib PROPERTIES IMPORTED_LOCATION ${lib_src_DIR}/libavutil-55.so)

add_library(libswresample_lib SHARED IMPORTED)
set_target_properties(libswresample_lib PROPERTIES IMPORTED_LOCATION ${lib_src_DIR}/libswresample-2.so)

add_library(libMach1DecodeCAPI_lib STATIC IMPORTED)
set_target_properties(libMach1DecodeCAPI_lib PROPERTIES IMPORTED_LOCATION ${lib_src_DIR}/libMach1DecodeCAPI.a)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

file(GLOB_RECURSE SOURCES "/src/main/cpp/*.h")

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).

             src/main/cpp/Mach1Decode.cpp
             src/main/cpp/AudioPlayer.cpp
             src/main/cpp/native-lib.cpp
             src/main/cpp/M1AudioPlayer.cpp
             ${SOURCES}
            )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                       android
                       log
                       OpenSLES

                       libavcodec_lib
                       libavfilter_lib
                       libavformat_lib
                       libavutil_lib
                       libswresample_lib

                       libMach1DecodeCAPI_lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )