cmake_minimum_required(VERSION 3.13.0)
project(spatial-decodepositional-example)
add_library(${PROJECT_NAME} STATIC)

# Link with lib
target_link_libraries(${PROJECT_NAME} PRIVATE Mach1Spatial)

# Source files
target_sources(${PROJECT_NAME}
PRIVATE
    src/main.cpp
    ../../../../libmach1spatial/include/cpp/Mach1Decode.cpp
    ../../../../libmach1spatial/include/cpp/Mach1DecodePositional.cpp
)

if(PROJECT_IS_TOP_LEVEL)
    # create the executable
    add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Threads::Threads)

    install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION build)
endif()