set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR armv7)

add_compile_options(-mfloat-abi=hard)
add_link_options(-mfloat-abi=hard)

SET(CMAKE_BUILD_FLAGS "-march=armv7-a -mfpu=neon-vfpv4 -funsafe-math-optimizations -mfp16-format=ieee")
SET(CMAKE_C_FLAGS "${CMAKE_BUILD_FLAGS}")
SET(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})