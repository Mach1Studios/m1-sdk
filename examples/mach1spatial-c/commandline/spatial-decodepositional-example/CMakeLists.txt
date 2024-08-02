cmake_minimum_required(VERSION 3.1.0)
project(spatial-decodepositional-example)

include(../../../../libmach1spatial)

# collect and add source files
set(SOURCES 
    src/main.cpp
    ../../../../libmach1spatial/include/cpp/Mach1Decode.cpp
    ../../../../libmach1spatial/include/cpp/Mach1DecodePositional.cpp
    )

# create the executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Threads::Threads)

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION build)