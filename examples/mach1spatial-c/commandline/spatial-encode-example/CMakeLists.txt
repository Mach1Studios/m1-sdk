cmake_minimum_required(VERSION 3.13.0)
project(spatial-encode-example)
add_executable(${PROJECT_NAME})

if(M1ENCODE_INLINE_DECODE)
    message(STATUS "ENCODE API: Supports inline decoding")
else()
    message(STATUS "ENCODE API: Does not support inline decoding")
endif()

# Link with lib
target_link_libraries(${PROJECT_NAME} PRIVATE M1Encode)
set(M1_LIB_PATH ../../../../libmach1spatial)

# Source files
target_sources(${PROJECT_NAME}
PRIVATE
    src/main.cpp
    ${M1_LIB_PATH}/api_encode/include/Mach1Encode.cpp
)

set(MACH1SPATIAL_INCLUDES ${M1_LIB_PATH}/api_encode/include ${M1_LIB_PATH}/deps)
include_directories(${MACH1SPATIAL_INCLUDES})

if(PROJECT_IS_TOP_LEVEL)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Threads::Threads)

    install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION build)
endif()