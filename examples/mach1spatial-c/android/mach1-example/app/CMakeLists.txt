# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(lib_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})
include_directories(
        ${CMAKE_SOURCE_DIR}/src/main/cpp/Mach1/include
)

add_library(libMach1DecodeCAPI_lib STATIC IMPORTED)
set_target_properties(libMach1DecodeCAPI_lib PROPERTIES IMPORTED_LOCATION ${lib_DIR}/libMach1DecodeCAPI.a)

add_library(libMach1DecodePositionalCAPI_lib STATIC IMPORTED)
set_target_properties(libMach1DecodePositionalCAPI_lib PROPERTIES IMPORTED_LOCATION ${lib_DIR}/libMach1DecodePositionalCAPI.a)

add_library(libMach1EncodeCAPI_lib STATIC IMPORTED)
set_target_properties(libMach1EncodeCAPI_lib PROPERTIES IMPORTED_LOCATION ${lib_DIR}/libMach1EncodeCAPI.a)

add_library(
        Mach1DecodeModule_lib
        SHARED
        ${CMAKE_SOURCE_DIR}/src/main/cpp/Mach1/Mach1DecodeModule_wrap.cxx
)

add_library(
        Mach1EncodeModule_lib
        SHARED
        ${CMAKE_SOURCE_DIR}/src/main/cpp/Mach1/Mach1EncodeModule_wrap.cxx
)

add_library(
        Mach1DecodePositionalModule_lib
        SHARED
        ${CMAKE_SOURCE_DIR}/src/main/cpp/Mach1/Mach1DecodePositionalModule_wrap.cxx
)

add_library(
        Mach1Point3DModule_lib
        SHARED
        ${CMAKE_SOURCE_DIR}/src/main/cpp/Mach1/Mach1Point3DModule_wrap.cxx
)

add_library(
        Mach1Point4DModule_lib
        SHARED
        ${CMAKE_SOURCE_DIR}/src/main/cpp/Mach1/Mach1Point4DModule_wrap.cxx
)

add_library(
        Mach1FloatArrayModule_lib
        SHARED
        ${CMAKE_SOURCE_DIR}/src/main/cpp/Mach1/Mach1FloatArrayModule_wrap.cxx
)

add_library(
        Mach1UtilsModule_lib
        SHARED
        ${CMAKE_SOURCE_DIR}/src/main/cpp/Mach1/Mach1UtilsModule_wrap.cxx
)

target_link_libraries(
        Mach1Point3DModule_lib
        Mach1Point4DModule_lib
        Mach1FloatArrayModule_lib
        Mach1UtilsModule_lib
)

target_link_libraries(
        Mach1DecodeModule_lib
        android
        log
        libMach1DecodeCAPI_lib
)

target_link_libraries(
        Mach1EncodeModule_lib
        android
        log
        libMach1EncodeCAPI_lib
)

target_link_libraries(
        Mach1DecodePositionalModule_lib
        android
        log
        libMach1DecodePositionalCAPI_lib
)

