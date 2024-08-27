cmake_minimum_required(VERSION 3.13.0)
set(CMAKE_CXX_STANDARD 17)
project(spatial-decode-example)
add_executable(${PROJECT_NAME})

# Link with lib
target_link_libraries(${PROJECT_NAME} PRIVATE M1Decode)
set(M1_LIB_PATH ../../../../libmach1spatial)

# Source files
target_sources(${PROJECT_NAME}
PRIVATE
	src/main.cpp
	${M1_LIB_PATH}/api_decode/include/Mach1Decode.cpp
)

set(MACH1SPATIAL_INCLUDES ${M1_LIB_PATH}/api_decode/include ${M1_LIB_PATH}/api_common/include ${M1_LIB_PATH}/deps)
include_directories(${MACH1SPATIAL_INCLUDES})

if(PROJECT_IS_TOP_LEVEL)
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)
	target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Threads::Threads)

	install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION build)
endif()